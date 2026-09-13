//
// Created by X-ray on 13/09/2026.
//

#include "asjit.hpp"

#include "jit_compiler.hpp"

namespace base::asjit {
  // No registry of attached engines here, deliberately. The first version kept a set of engine
  // pointers so that IsAttached could answer without touching the engine, and that set outlived the
  // engines in it: a new engine allocated at the address of a destroyed one was reported as already
  // attached. The engine can be asked directly, which is both correct and a good deal less state.

  void Attach(AngelScript::asIScriptEngine* engine, const Options& options) {
    if (engine == nullptr)
      return;

    // Setting the property first, and refusing to attach if it is rejected. The engine picks which
    // JIT interface it calls by reading this value at compile time, so an engine left on interface
    // version 1 would call CompileFunction/ReleaseJITFunction through the vtable slot that holds
    // NewFunction/CleanFunction here - a mismatch that would run as a plausible-looking call rather
    // than as a failure.
    if (engine->SetEngineProperty(AngelScript::asEP_JIT_INTERFACE_VERSION, 2) < 0)
      return;

    // Set before the compiler is installed, so that an engine which is attached and then immediately
    // built has the options in place for the first function it compiles - there is no window in which
    // the compiler is reachable and the options are not.
    JitCompiler::Instance().SetOptions(options);

    engine->SetJITCompiler(&JitCompiler::Instance());
  }

  void Attach(AngelScript::asIScriptEngine* engine) {
    Attach(engine, Options{});
  }

  void Detach(AngelScript::asIScriptEngine* engine) {
    if (engine == nullptr)
      return;

    if (engine->GetJITCompiler() != &JitCompiler::Instance())
      return;

    JitCompiler::Instance().DetachEngine(engine);
  }

  bool IsAttached(const AngelScript::asIScriptEngine* engine) {
    return engine != nullptr && engine->GetJITCompiler() == &JitCompiler::Instance();
  }

  Stats GetStats() {
    return JitCompiler::Instance().GetStats();
  }

  void ResetStats() {
    JitCompiler::Instance().ResetStats();
  }

} // namespace base::asjit
