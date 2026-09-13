//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>

#include <array>
#include <cstdint>

// The only header a consumer needs. Deliberately free of AsmJit: menu_core already carries
// PolyHook's AsmJit 1.17 on its include path, so pulling in a second AsmJit here would put two
// copies of `asmjit/...` on the same include path and make every `#include <asmjit/...>` in the
// project ambiguous. AsmJit is used behind the implementation, never in an interface.
//
// Requires AS_USE_NAMESPACE, which asjit re-exports from AngelScript's add-ons, so
// AngelScript::asIScriptEngine here is the same type menu_core names.
namespace base::asjit {
  // Counters since the process started, rather than a live size. The questions worth asking of a
  // JIT from a test are "did it engage at all" and "did it keep engaging after a reload" - neither
  // of which a snapshot answers, and both of which are answered by watching these move.
  struct Stats {
    // NewFunction calls: every script function the engine offered us.
    std::uint64_t functions_seen = 0;
    // Functions handed a native entry point. Zero here means the JIT is doing nothing, which is
    // otherwise indistinguishable from a JIT that is working perfectly.
    std::uint64_t functions_compiled = 0;
    // Functions we chose to leave to the interpreter.
    std::uint64_t functions_declined = 0;
    // Basic blocks turned into native code across all functions.
    std::uint64_t blocks_emitted = 0;
    // CleanFunction calls, i.e. compiled functions the engine has thrown away.
    std::uint64_t functions_cleaned = 0;

    // Instructions translated, and instructions the functions offered to us had in total. The ratio
    // is what explains `functions_compiled`: a JIT that compiles everything and translates a tenth of
    // it is not doing the same thing as one that translates nine tenths, and the two are
    // indistinguishable from the counters above.
    //
    // The total counts every function the engine offered, including the ones we declined - a coverage
    // figure over only the functions that were easy to translate would report the wrong number about
    // the program.
    //
    // Both are static counts over bytecode, not a record of what ran. They answer "how much of this
    // was turned into machine code", not "how much time was spent in it" - answering the second would
    // need the interpreter to report back, which the interface does not provide.
    std::uint64_t instructions_translated = 0;
    std::uint64_t instructions_total = 0;

    // Bytes of machine code emitted across all functions.
    std::uint64_t code_size = 0;

    // How often each opcode was seen and how often it was translated, indexed by opcode byte.
    //
    // Worth reading as a pair: an opcode with a high `walked` and a zero `translated` is one that
    // went unimproved, and the ones with the highest counts are where a script's time is going. In
    // practice this is one or two entries and usually a call, which is what makes it the most direct
    // answer to "why is this script not faster".
    //
    // The pair alone cannot say *why* an instruction was not translated, and the two reasons call for
    // different work, so `opcodes_unimplemented` separates them:
    //
    //   walked > translated, unimplemented > 0  - the translator has no case for this opcode. This is
    //                                             the one that is a gap, and closing it is a change
    //                                             to the emitter.
    //   walked > translated, unimplemented == 0 - the translator does know this opcode, but this
    //                                             instance was never handed to it, because the block
    //                                             it would have been in ended earlier. Closing this is
    //                                             not a matter of translating the opcode; it is
    //                                             already translated wherever the emitter is asked.
    //
    // A call is the commonest gap and is a case of the *first* kind, which is easy to get backwards
    // because a call is also what ends a block. The compiler puts an entry point after every call and
    // after every line instruction, so the code past a call is compiled as a block of its own rather
    // than being skipped. What produces the second kind is the same refusal at an instruction with no
    // entry point after it - an allocation, a cast - where the rest of that statement has nowhere to
    // resume from.
    //
    // Both are counted per opcode rather than as a single "unsupported" total, because the useful
    // question is which opcodes, not how many instructions.
    //
    // Indexing by opcode byte rather than by asEBCInstr is deliberate: the enum is not guaranteed
    // dense, and a 256-entry table indexed by the byte the VM itself dispatches on is exact by
    // construction.
    std::array<std::uint64_t, 256> opcodes_walked{};
    std::array<std::uint64_t, 256> opcodes_translated{};
    std::array<std::uint64_t, 256> opcodes_unimplemented{};
  };

  // Which instructions the translator takes, where taking one is a choice rather than a translation.
  //
  // Exactly two opcodes in the bytecode set are in that position, and they are the two the emitter
  // has no case for: `asBC_RET` and `asBC_Thiscall1`. Both *can* be translated - each was written,
  // run against the differential suite and found to agree with the interpreter everywhere the suite
  // reaches, including four frames of recursion for the return - and both cost more translated than
  // handed back. The measurements are in the `asBC_RET` case in `function_compiler.cpp` and in the
  // `asBC_Thiscall1` paragraph in `emitter.cpp`, and they are what the defaults below are set from.
  //
  // So the honest description of the shipped configuration is not "every opcode implemented", it is
  // "199 of the 201 the VM can dispatch, 197 of them enabled by default, and the two disabled on
  // measurement rather than on capability". Each term in that sentence has a source:
  //
  //   208  `asBCInfo` records in the engine's table (angelscript.h), of which one is the `asBCINFO`
  //        macro's own parameter list and not an opcode, leaving 207.
  //   201  of those reach the VM's dispatch table (as_context.cpp:2219). The other six - Block, LABEL,
  //        LINE, ObjInfo, TryBlock, VarDecl - are metadata the engine consumes while finalizing a
  //        module and never dispatches, so "translating" one is not a thing that can be done.
  //   199  have a translation: 179 as cases in `emitter.cpp` and 20 reached before it in
  //        `function_compiler.cpp`, which owns the control flow, the calls and the two options.
  //     2   do not, which is the whole of the gap: `STR`, whose interpreter handler is
  //        `asASSERT(false)`, and `CALLBND`, which needs an `import` the menu never writes. Both
  //        arguments are in the default case of `emitter.cpp`, where the two are refused.
  //
  // Counting it this way rather than asserting a total is the point: every number above is one command
  // against the source, not a claim about the translator. Keeping the two gated instructions behind a
  // flag rather than deleting them is the same instinct at the other end - the differential suite runs
  // under both configurations, so "they cost more than they save" is a test result, and a future engine
  // version that moves either measurement is a default to change rather than a translation to rewrite.
  //
  // Nothing in the menu sets either flag. They exist for the tests and for the benchmark, which is
  // where the numbers above come from.
  struct Options {
    // Translate `asBC_RET`, the return from a script function.
    //
    // Off by default. A return is the one instruction a call-heavy script is mostly made of, and the
    // translation pays a call into the engine for the frame-pop predicate where the interpreter pays a
    // load and a compare. See `function_compiler.cpp` for the table.
    bool translate_returns = false;

    // Translate `asBC_Thiscall1`, the method call whose return value is written through a pointer.
    //
    // Off by default, and for a second reason the return does not have: the only faithful route runs
    // the same routine the interpreter's handler runs, so the translation is correct rather than
    // merely close, but it does not remove the call - it removes the dispatch around it, which on this
    // instruction is a smaller share of the cost than on the calls that *are* translated.
    bool translate_object_method_calls = false;
  };

  // Registers the compiler with `engine` and selects JIT interface version 2.
  //
  // Sets asEP_JIT_INTERFACE_VERSION itself because that property and the registration are one
  // decision: the engine picks which interface it calls from the property (as_scriptfunction.cpp),
  // so setting one without the other leaves the engine calling through the wrong vtable. Attaching
  // twice is harmless.
  //
  // Does *not* set asEP_INCLUDE_JIT_INSTRUCTIONS. Whether the optimizer emits asBC_JitEntry is the
  // host engine's codegen policy, not something a JIT should decide, so it is set where the rest of
  // the engine's properties are. Attaching to an engine built without it is not an error - the JIT
  // simply never runs, since a function with no entry points is declined - and that case is left to
  // be noticed through the counters rather than warned about here: the property may legitimately be
  // set after this call, and nothing has been compiled yet at the point it is made.
  //
  // The options are read when a function is compiled, which happens while its module is built, so a
  // caller that wants them honoured has to set them before building the module rather than before
  // running it. They are held by the compiler rather than by the engine, because there is one compiler
  // per process - see `JitCompiler` - and the last Attach to set them wins.
  void Attach(AngelScript::asIScriptEngine* engine, const Options& options);

  // The defaults above, which is the configuration the menu runs.
  void Attach(AngelScript::asIScriptEngine* engine);

  // Unregisters the compiler, releasing everything it compiled for this engine's functions first.
  //
  // The release has to come first. A script function's destructor calls back into the engine's
  // compiler whenever it still holds compiled code, and it does so without checking that the pointer
  // is there - so clearing the engine's compiler while compiled functions are alive would turn a
  // later module teardown into a null dereference. Handing the code back while the compiler is still
  // reachable is what makes this safe, and it is why Detach is not simply SetJITCompiler(null).
  //
  // Not calling it is not what leaves code behind, though: an engine shut down while still registered
  // runs the same callbacks from its own destructor, so its entries go back either way. What this is
  // for is the engine that outlives the decision - unregistering a compiler from one that is still
  // running is exactly the case where the ordering above has to be right, and doing it by hand means
  // knowing that the functions have to be emptied before the pointer goes.
  //
  // Detaching an engine that was never attached does nothing.
  void Detach(AngelScript::asIScriptEngine* engine);

  [[nodiscard]] bool IsAttached(const AngelScript::asIScriptEngine* engine);

  [[nodiscard]] Stats GetStats();

  // Test hook. Not called by the library itself.
  void ResetStats();
} // namespace base::asjit
