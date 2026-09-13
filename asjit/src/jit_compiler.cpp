//
// Created by X-ray on 13/09/2026.
//

#include "jit_compiler.hpp"

#include <utility>

#include "function_compiler.hpp"

namespace base::asjit {

  JitCompiler& JitCompiler::Instance() {
    // A function-local static rather than a namespace-scope object: the engine stores the pointer we
    // hand it without taking a reference and never releases it (asCScriptEngine::SetJITCompiler is a
    // plain assignment), so this has to outlive every engine. Construction on first use is what makes
    // that true without depending on the order translation units are initialised in.
    static JitCompiler instance;
    return instance;
  }

  void JitCompiler::NewFunction(AngelScript::asIScriptFunction* scriptFunc) {
    if (scriptFunc == nullptr)
      return;

    FunctionCompiler::Result compiled;

    {
      const std::lock_guard lock(mutex_);

      ++stats_.functions_seen;

      compiled = FunctionCompiler::Compile(runtime_, scriptFunc, options_);

      // Counted before the decline, because a function we could not compile still has instructions and
      // leaving them out of the denominator would report the translated fraction of the functions that
      // were easy rather than of the program.
      //
      // The gaps are recorded here with the walked counts rather than with the translations below,
      // because they are a fact about the walk: an instruction the emitter had no case for is one
      // regardless of whether the code around it was ever published. A function that was declined
      // outright reports none of them, so nothing in these two arrays is attributed to a function that
      // never ran.
      stats_.instructions_total += compiled.instructions_total;
      for (std::size_t opcode = 0; opcode < stats_.opcodes_walked.size(); ++opcode) {
        stats_.opcodes_walked[opcode] += compiled.opcodes_walked[opcode];
        stats_.opcodes_unimplemented[opcode] += compiled.opcodes_unimplemented[opcode];
      }

      if (compiled.entry == nullptr) {
        ++stats_.functions_declined;
        return;
      }

      // Recorded before it is handed out, so that the moment the engine can call CleanFunction with
      // it there is already something to compare against. Registering it after would leave a window
      // in which the engine's own bookkeeping could free a pointer this does not yet know about.
      live_functions_.insert_or_assign(reinterpret_cast<const void*>(compiled.entry), scriptFunc);
      ++stats_.functions_compiled;
      stats_.blocks_emitted += compiled.blocks_emitted;
      stats_.instructions_translated += compiled.instructions_translated;
      stats_.code_size += compiled.code_size;
      for (std::size_t opcode = 0; opcode < stats_.opcodes_translated.size(); ++opcode)
        stats_.opcodes_translated[opcode] += compiled.opcodes_translated[opcode];
    }

    // Outside the lock, deliberately. SetJITFunction calls back into CleanFunction whenever the
    // function already had a compiled entry - which is every recompilation - and CleanFunction takes
    // this same mutex. Calling it while holding the lock would deadlock on the second compile of any
    // function rather than on the first, so it would survive every simple test.
    const int published = scriptFunc->SetJITFunction(compiled.entry);

    if (published < 0) {
      // The engine refused the entry point - it does that for anything that is not a script function.
      // The code is already built and already recorded, so it has to be taken back rather than left
      // behind, and the counters have to agree with the set.
      //
      // Every counter the successful path added is undone here, the per-opcode arrays included. They
      // are what the coverage diagnostic reads, so leaving them to count a compile that was thrown
      // away would make a rejected function look like a translated one for as long as the process
      // lives - which is exactly the kind of drift these counters exist to rule out.
      const std::lock_guard lock(mutex_);
      live_functions_.erase(reinterpret_cast<const void*>(compiled.entry));
      --stats_.functions_compiled;
      stats_.blocks_emitted -= compiled.blocks_emitted;
      stats_.instructions_translated -= compiled.instructions_translated;
      stats_.code_size -= compiled.code_size;
      for (std::size_t opcode = 0; opcode < stats_.opcodes_translated.size(); ++opcode) {
        stats_.opcodes_translated[opcode] -= compiled.opcodes_translated[opcode];
        stats_.opcodes_unimplemented[opcode] -= compiled.opcodes_unimplemented[opcode];
      }
      runtime_.release(compiled.entry);
    }
  }

  void JitCompiler::DetachEngine(AngelScript::asIScriptEngine* engine) {
    if (engine == nullptr)
      return;

    // Collected under the lock and then acted on outside it, because every SetJITFunction below
    // re-enters CleanFunction, which takes the same lock.
    std::vector<AngelScript::asIScriptFunction*> owned;

    {
      const std::lock_guard lock(mutex_);
      for (const auto& [entry, function] : live_functions_) {
        if (function->GetEngine() == engine)
          owned.push_back(function);
      }
    }

    for (auto* function : owned) {
      // Passing null is what makes this more than bookkeeping: the function sees that it already has
      // an entry point, hands it back through CleanFunction, and clears its own. After this no
      // function of this engine is holding code, so the pointer below can safely go.
      function->SetJITFunction(nullptr);
    }

    engine->SetJITCompiler(nullptr);
  }

  void JitCompiler::CleanFunction(AngelScript::asIScriptFunction* scriptFunc, AngelScript::asJITFunction jitFunc) {
    (void)scriptFunc;

    if (jitFunc == nullptr)
      return;

    const std::lock_guard lock(mutex_);

    ++stats_.functions_cleaned;

    // Releasing a pointer that was never handed out would hand the allocator an address it does not
    // own, and the argument alone cannot distinguish that case - the engine has no obligation to only
    // ever call this with a pointer that came from here. The map is what makes the check possible.
    if (live_functions_.erase(reinterpret_cast<const void*>(jitFunc)) == 0)
      return;

    runtime_.release(jitFunc);
  }

  void JitCompiler::SetOptions(const Options& options) {
    // Not `stats_`-like state that outlives a Detach: the options describe how to translate, and a
    // translation already made is not revisited. Leaving them in place across a Detach is deliberate -
    // detaching an engine says nothing about which instructions the next one should translate.
    const std::lock_guard lock(mutex_);
    options_ = options;
  }

  Stats JitCompiler::GetStats() const {
    const std::lock_guard lock(mutex_);
    return stats_;
  }

  void JitCompiler::ResetStats() {
    const std::lock_guard lock(mutex_);
    stats_ = Stats{};
  }

} // namespace base::asjit
