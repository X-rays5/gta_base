//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>
#include <asmjit/core/jit_runtime.h>

#include <cstdint>
#include <mutex>
#include <unordered_map>

#include "asjit.hpp"

namespace base::asjit {

  // The asIJITCompilerV2 the engine calls into. One instance serves every engine in the process.
  //
  // Ownership stays here rather than with the engine: asCScriptEngine::SetJITCompiler stores the
  // pointer and takes no reference, and nothing releases it, so the engine is holding a borrowed
  // pointer. That is what makes the shared instance the right shape - and why it must outlive every
  // engine, which is what Instance() being a function-local static buys.
  class JitCompiler final : public AngelScript::asIJITCompilerV2 {
  public:
    static JitCompiler& Instance();

    // Called by the engine once per script function, at the end of the module build that produced
    // it (asCScriptFunction::JITCompile). Compiles what it can and publishes the result by way of
    // SetJITFunction; anything it declines simply stays interpreted.
    void NewFunction(AngelScript::asIScriptFunction* scriptFunc) override;

    // Called when compiled code is being thrown away - a module being discarded, a function being
    // recompiled, or an engine shutting down. Releases the executable memory for `jitFunc`.
    void CleanFunction(AngelScript::asIScriptFunction* scriptFunc, AngelScript::asJITFunction jitFunc) override;

    // Hands back every function `engine` owns and then takes the compiler away from it.
    //
    // The order is the whole of it. A script function's destructor calls CleanFunction whenever it
    // still holds compiled code, and it does so by dereferencing the engine's compiler pointer with no
    // null check (as_scriptfunction.cpp) - so taking the compiler away while any compiled function is
    // still alive would turn a later module teardown into a null dereference. Clearing the functions'
    // entries first means there is nothing left for the destructor to hand back.
    void DetachEngine(AngelScript::asIScriptEngine* engine);

    [[nodiscard]] Stats GetStats() const;
    void ResetStats();

    // Which optional translations a function compiled from here on takes. Set by `Attach`, and read
    // under the same mutex as everything else. Held here rather than passed to `NewFunction` because
    // the interface that calls it - `asIJITCompilerV2` - has no parameter to carry it and no way to
    // ask a function which engine it belongs to: `asIScriptFunction` publishes `GetModule`, and a
    // module publishes `GetEngine`, but a function compiled outside any module has neither.
    void SetOptions(const Options& options);

  private:
    JitCompiler() = default;
    ~JitCompiler() override = default;

    JitCompiler(const JitCompiler&) = delete;
    JitCompiler& operator=(const JitCompiler&) = delete;

    mutable std::mutex mutex_;

    // The allocator for every function we compile together, rather than one per script: the menu
    // creates and discards an engine per script, so a runtime per engine would churn executable
    // pages and their quotas for no benefit.
    asmjit::JitRuntime runtime_;

    // Every entry point currently handed out, mapped to the function that owns it. CleanFunction
    // releases the pointer the engine passes back, and this is what makes that safe: releasing a
    // pointer we did not hand out would free someone else's code, and there is no way to tell from the
    // argument alone. The function on the right is what DetachEngine needs in order to find the ones
    // belonging to a particular engine.
    std::unordered_map<const void*, AngelScript::asIScriptFunction*> live_functions_;

    // The configuration the next compiled function is translated under. Process-wide for the same
    // reason the compiler is: there is one of these, it serves every engine, and a per-engine copy
    // would need a way to get from a function to its engine that the interface does not provide.
    Options options_;

    Stats stats_;
  };

} // namespace base::asjit
