//
// Created by X-ray on 13/09/2026.
//

// The tests that are about the compiler rather than about the translated instructions: that it
// engages at all, that it releases what it allocated, that a script it cannot translate still runs,
// and that compiling a loop does not take away the context's ability to interrupt it.

#include "test_support.hpp"

#include <angelscript.h>
#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <asjit.hpp>
#include <gtest/gtest.h>

// Reached for `JitCompiler::Instance` alone, which the test below needs in order to ask for a second
// compile of a function the engine has already compiled once. The engine has no way to be asked for
// one - see the comment there - so the alternative was to leave the path untested.
#include <jit_compiler.hpp>

#include <chrono>
#include <cstdio>
#include <string>

namespace base::asjit::test {
  namespace {
    [[nodiscard]] AngelScript::asIScriptModule* BuildModule(AngelScript::asIScriptEngine* engine, const std::string& source) {
      AngelScript::CScriptBuilder builder;
      builder.StartNewModule(engine, "asjit_compiler_test");
      if (builder.AddSectionFromMemory("test.as", source.c_str(), static_cast<unsigned int>(source.size())) < 0)
        return nullptr;
      if (builder.BuildModule() < 0)
        return nullptr;
      return builder.GetModule();
    }

    int g_line_callbacks = 0;
    constexpr int kAbortAfterLineCallbacks = 200;

    void AbortingLineCallback(AngelScript::asIScriptContext* context, void* /*userData*/) {
      if (++g_line_callbacks >= kAbortAfterLineCallbacks)
        context->Abort();
    }
  } // namespace

  TEST(JitCompiler, CompilesScriptFunctions) {
    const auto outcome = RunScript("void main() { int a = 1; int b = 2; show(a + b); }", true);

    ASSERT_TRUE(outcome.built) << outcome.errors;
    EXPECT_TRUE(outcome.entry_found);
    EXPECT_GT(outcome.functions_seen, 0u) << "the engine never offered the compiler a function";
    EXPECT_TRUE(outcome.entry_has_jit) << "the entry point carries no compiled code";
    EXPECT_GT(outcome.functions_compiled, 0u);
    EXPECT_GT(outcome.blocks_emitted, 0u);
  }

  TEST(JitCompiler, StaysOutOfTheWayWhenNotAttached) {
    // The same engine configuration with no compiler registered. Nothing should be compiled, and the
    // script should run exactly as it does with one - which is what the differential tests rest on.
    const auto outcome = RunScript("void main() { int a = 1; int b = 2; show(a + b); }", false);

    ASSERT_TRUE(outcome.built) << outcome.errors;
    EXPECT_TRUE(outcome.entry_found);
    EXPECT_FALSE(outcome.entry_has_jit);
    EXPECT_EQ(outcome.functions_compiled, 0u);
    EXPECT_EQ(outcome.output, "3\n");
  }

  TEST(JitCompiler, AttachIsReportedAndIdempotent) {
    auto* engine = CreateEngine(EngineConfig{});
    ASSERT_NE(engine, nullptr);

    EXPECT_FALSE(asjit::IsAttached(engine));

    asjit::Attach(engine);
    EXPECT_TRUE(asjit::IsAttached(engine));

    // Attaching twice is documented as harmless, and it has to be: the engine holds one borrowed
    // pointer, so the second call sets the same value and nothing is leaked or double-released.
    asjit::Attach(engine);
    EXPECT_TRUE(asjit::IsAttached(engine));

    asjit::Detach(engine);
    EXPECT_FALSE(asjit::IsAttached(engine));

    engine->ShutDownAndRelease();
  }

  TEST(JitCompiler, ReleasesCompiledCodeWhenTheModuleIsDiscarded) {
    auto* engine = CreateEngine(EngineConfig{.use_jit = true});
    ASSERT_NE(engine, nullptr);
    asjit::Attach(engine);

    asjit::ResetStats();

    auto* module = BuildModule(engine, "int Helper(int n) { return n * 2; }\nvoid main() { show(Helper(21)); }");
    ASSERT_NE(module, nullptr);

    const auto compiled = asjit::GetStats();
    ASSERT_GT(compiled.functions_compiled, 0u) << "nothing was compiled, so there is nothing to release";

    // Discard moves the module to the engine's discarded pile but only frees it once the last
    // reference goes, so the release is completed by the shutdown below rather than by the Discard.
    module->Discard();
    engine->ShutDownAndRelease();

    const auto released = asjit::GetStats();
    EXPECT_GT(released.functions_cleaned, 0u) << "the code compiled for the module was never handed back";
    EXPECT_EQ(released.functions_cleaned, compiled.functions_compiled)
        << "a compiled function was never released";

    // The engine is gone and has already released everything it held, so a second pass over the same
    // functions would show up as counters still climbing. That is the failure the live set exists to
    // prevent: one of those releases would be a free of memory the allocator had already taken back.
    EXPECT_EQ(asjit::GetStats().functions_cleaned, released.functions_cleaned);
  }

  TEST(JitCompiler, DetachHandsBackEverythingAnEngineThatIsStillRunningHolds) {
    // Detach, as opposed to the shutdown above - which never detaches and releases everything anyway,
    // because the engine's own destructor runs the same callbacks while the compiler is still
    // registered.
    //
    // So what this is about is the ordering, which is the one thing in this component whose failure is
    // not a wrong answer but a crash: a script function's destructor dereferences the engine's
    // compiler with no null check whenever it still holds code, so an engine whose compiler was taken
    // away *first* turns its own teardown into a null dereference. The detach has to empty the
    // functions before it empties the pointer, and the end of this test is the only place that says
    // so in the language of the suite rather than in a comment.
    asjit::ResetStats();

    auto* engine = CreateEngine(EngineConfig{.use_jit = true});
    ASSERT_NE(engine, nullptr);

    const auto script = support::Build(engine, "asjit_detach", R"AS(
      int Triple(int n) { return n * 3; }
      void main() { show(Triple(14)); }
    )AS");
    ASSERT_TRUE(script.built) << support::Errors();
    ASSERT_NE(script.entry, nullptr);
    ASSERT_NE(script.entry->GetJITFunction(), nullptr)
        << "nothing was compiled, so there is nothing for the detach to hand back";

    const auto compiled = asjit::GetStats();
    ASSERT_GT(compiled.functions_compiled, 0u);

    asjit::Detach(engine);

    EXPECT_FALSE(asjit::IsAttached(engine));

    const auto detached = asjit::GetStats();
    EXPECT_EQ(detached.functions_cleaned, compiled.functions_compiled)
        << "the detach left compiled code behind, so the engine is still holding something the "
           "compiler it no longer has a pointer to would have to release";
    EXPECT_EQ(script.entry->GetJITFunction(), nullptr)
        << "a function of a detached engine still holds an entry point";

    // And the engine is still usable, which is the difference between detaching and shutting down: it
    // runs the module and answers the same, on the interpreter.
    support::ClearOutput();
    const auto execution = support::Execute(script.entry);
    EXPECT_EQ(execution.result, AngelScript::asEXECUTION_FINISHED) << execution.exception;
    EXPECT_EQ(support::Output(), "42\n");

    // The teardown the header describes. There is nothing left for the destructor to call back for, so
    // it does not reach the compiler - and if the detach had cleared the pointer before the functions,
    // this is the line that would dereference null.
    script.module->Discard();
    engine->ShutDownAndRelease();

    EXPECT_EQ(asjit::GetStats().functions_cleaned, detached.functions_cleaned)
        << "the shutdown handed back code that the detach had already released, so it was released "
           "twice";
  }

  TEST(JitCompiler, RecompilingOverALiveEntryHandsBackWhatItReplaced) {
    // The engine compiles a function once per module build, and its own recompile path hands the old
    // entry back *before* offering the function again: `asCScriptFunction::JITCompile` gives the
    // previous pointer to CleanFunction and nulls it (as_scriptfunction.cpp:1626) and only then calls
    // `NewFunction`. So through the engine, `SetJITFunction` never finds a live entry to replace.
    //
    // Replacing one is still a path this class has to survive, because it is the only path that
    // re-enters the compiler from inside the publish - `SetJITFunction` calls `CleanFunction` whenever
    // the function already had a different entry (as_scriptfunction.cpp:1567), and `CleanFunction`
    // takes the same mutex `NewFunction` holds across the compile. That is what the placement of the
    // publish outside the lock is for, and it is untested by everything above, which only ever
    // recompiles the way the engine does.
    //
    // Worth pinning even though the failure is a hang rather than a failed assertion. A hang is a poor
    // way to report a regression, but it is a much better one than the deadlock arriving in a host that
    // compiles a function twice, where nothing would be watching for it.
    asjit::ResetStats();

    auto* engine = CreateEngine(EngineConfig{.use_jit = true});
    ASSERT_NE(engine, nullptr);

    const auto script = support::Build(engine, "asjit_recompile", R"AS(
      int Double(int n) { return n * 2; }
      void main() { show(Double(21)); }
    )AS");
    ASSERT_TRUE(script.built) << support::Errors();
    ASSERT_NE(script.entry, nullptr);

    const auto after_build = asjit::GetStats();
    const auto* built_entry = script.entry->GetJITFunction();
    ASSERT_NE(built_entry, nullptr)
        << "the module build left the entry point interpreted, so the compile below would be a first "
           "compile rather than the publish-over-a-live-entry path this test is about";

    // Named separately from the pointer above, because it is the counter the closing assertion is
    // about rather than the pointer: an equality between two zeros proves nothing about either.
    ASSERT_GT(after_build.functions_compiled, 0u) << "nothing was compiled at all";

    auto& compiler = JitCompiler::Instance();

    // One compile, one release. Both are asserted as deltas rather than against zero, because the
    // module build above is already counted and the engine compiled more than the entry point.
    compiler.NewFunction(script.entry);
    const auto* first = script.entry->GetJITFunction();
    const auto after_first = asjit::GetStats();

    EXPECT_NE(first, nullptr) << "the recompile published nothing, so the entry was left unmodifiable";
    EXPECT_EQ(after_first.functions_compiled, after_build.functions_compiled + 1);
    EXPECT_EQ(after_first.functions_cleaned, after_build.functions_cleaned + 1)
        << "publishing over a live entry never handed the entry it replaced back";

    // The old entry is still allocated at the moment the new one is obtained - the release happens
    // after the publish, from inside it - so asmjit cannot have handed the same block out twice. Two
    // equal pointers would mean the release came *before* the reuse, which is the arrangement the live
    // set would then get wrong: the map would hold one key for two generations of code.
    EXPECT_NE(first, built_entry);

    // The cycle a second time, which is what makes this a cycle rather than a one-off. A set that
    // recorded the wrong key would still look right once and only then start freeing twice or never.
    compiler.NewFunction(script.entry);
    const auto* second = script.entry->GetJITFunction();
    const auto after_second = asjit::GetStats();

    EXPECT_NE(second, nullptr);
    EXPECT_NE(second, first);
    EXPECT_EQ(after_second.functions_compiled, after_first.functions_compiled + 1);
    EXPECT_EQ(after_second.functions_cleaned, after_first.functions_cleaned + 1);

    // And the code the last compile published is the code that runs. This is the assertion the
    // counters cannot make: if the release had been of the *new* entry rather than the one it
    // replaced, the counts would be identical and the entry point would be a pointer into memory the
    // allocator had already taken back - which is a wrong answer or a crash rather than a number that
    // disagrees.
    support::ClearOutput();
    const auto execution = support::Execute(script.entry);
    EXPECT_EQ(execution.result, AngelScript::asEXECUTION_FINISHED) << execution.exception;
    EXPECT_EQ(support::Output(), "42\n");

    // Every entry the engine still holds goes back at shutdown, and the teardown knows nothing about
    // the extra release the recompiles already performed. So the totals have to meet exactly: one
    // clean per compile, no more and no fewer. A release the map dropped shows up here as cleaned
    // trailing compiled, and one it performed twice as cleaned running ahead - which is the pair the
    // set exists to make impossible.
    script.module->Discard();
    engine->ShutDownAndRelease();

    const auto released = asjit::GetStats();
    EXPECT_EQ(released.functions_cleaned, released.functions_compiled)
        << "compiles and releases disagree after a recompile and a shutdown, so something was freed "
           "twice or never";
  }

  TEST(JitCompiler, ARefusedPublishLeavesNoCodeBehind) {
    // The other half of publishing: the engine is allowed to refuse the entry point, and the compiler
    // then has to take back everything it built for it. `SetJITFunction` refuses in two cases
    // (as_scriptfunction.cpp:1561) - a function that is not a script function, and an engine that is
    // not talking to interface version 2 - and the second is the one that can be arranged from the
    // public API, by setting the property the engine reads.
    //
    // Nothing else reaches it, which is why the arrangement is worth the comment. The engine's own
    // guards are the same two tests: `JITCompile` returns before offering a function that is not a
    // script function, and under version 1 it calls a method of the *other* interface rather than
    // NewFunction. So a refused publish is a state the engine never creates; it is what the compiler
    // does if one ever does.
    //
    // The window in which the property reads 1 is exactly one call wide and is closed again on the
    // next line, because the engine dispatches on that property and `asIJITCompilerV2` does not
    // inherit from `asIJITCompiler` - a build or a run inside the window would have the engine calling
    // a method this class does not have.
    asjit::ResetStats();

    auto* engine = CreateEngine(EngineConfig{.use_jit = true});
    ASSERT_NE(engine, nullptr);

    const auto script = support::Build(engine, "asjit_refused_publish", R"AS(
      int Double(int n) { return n * 3; }
      void main() { show(Double(14)); }
    )AS");
    ASSERT_TRUE(script.built) << support::Errors();
    ASSERT_NE(script.entry, nullptr);

    const auto* published = script.entry->GetJITFunction();
    ASSERT_NE(published, nullptr) << "the module build published nothing, so the refusal below would "
                                     "have nothing to leave undisturbed";

    const auto before = asjit::GetStats();
    ASSERT_GT(before.opcodes_translated[AngelScript::asBC_CALLSYS], 0u)
        << "no call was translated in this script, so the counter asserted unchanged below is zero on "
           "both sides and the assertion says nothing";

    engine->SetEngineProperty(AngelScript::asEP_JIT_INTERFACE_VERSION, 1);
    JitCompiler::Instance().NewFunction(script.entry);
    engine->SetEngineProperty(AngelScript::asEP_JIT_INTERFACE_VERSION, 2);

    const auto after = asjit::GetStats();

    // That the refusal was the engine's rather than the compiler's. A function the translator could
    // not compile would have been declined instead - the path that increments this counter and returns
    // before anything is published - and every comparison below would then be a claim about a compile
    // that never happened.
    EXPECT_EQ(after.functions_declined, before.functions_declined)
        << "the translator declined the function, so no publish was ever attempted";
    EXPECT_EQ(after.functions_seen, before.functions_seen + 1) << "the function was never offered";

    // Taken back in full: the counters the successful path added, the per-opcode arrays included.
    // Those are what the coverage diagnostic reads, so one left standing would make a compile that was
    // thrown away look translated for as long as the process lives - and it could never be corrected,
    // because the entry is out of the live set by then and nothing will hand it back a second time.
    EXPECT_EQ(after.functions_compiled, before.functions_compiled);
    EXPECT_EQ(after.blocks_emitted, before.blocks_emitted);
    EXPECT_EQ(after.instructions_translated, before.instructions_translated);
    EXPECT_EQ(after.code_size, before.code_size);
    EXPECT_EQ(after.opcodes_translated[AngelScript::asBC_CALLSYS],
              before.opcodes_translated[AngelScript::asBC_CALLSYS]);

    // And the refusal did not disturb what the module build had already published. `SetJITFunction`
    // returns before its replace logic when the interface version is wrong, so the entry stays the
    // function's - and this is the half the rollback could get wrong, since it erases the entry it
    // just recorded out of a set that still has to hold this one.
    EXPECT_EQ(script.entry->GetJITFunction(), published);

    // Not rolled back, and not an oversight: the walked, total and unimplemented counts are facts
    // about the *walk*, which is why they are recorded before the publish. The function had
    // instructions whether or not any of them became code, and the denominator is what makes the
    // translated fraction mean "of the program" rather than "of what was easy".
    EXPECT_GT(after.instructions_total, before.instructions_total);

    // The accounting still closes after the refusal, which is the part of this a counter alone cannot
    // express: the entry the refusal released must not be released a second time at teardown, and the
    // one the build published must still be.
    script.module->Discard();
    engine->ShutDownAndRelease();

    const auto released = asjit::GetStats();
    EXPECT_EQ(released.functions_cleaned, released.functions_compiled)
        << "a refused publish left the live set and the counters disagreeing about what was freed";
  }

  TEST(JitCompiler, DivisionByZeroRaisesTheSameWayCompiled) {
    const std::string source = R"AS(
      void main() {
        int divisor = 0;
        int value = 10;
        show(value / divisor);
        show(999);
      }
    )AS";

    // The division is translated, and the divisor is a variable the JIT cannot fold - so this
    // exercises the guard's fault route rather than a hand-off at an untranslated instruction. The
    // guard branches out of line to an exit that re-enters the engine at the division itself, and
    // what has to survive that is the exception: the same one, raised from the same instruction.
    //
    // The raising comparison rather than the matching one, because this script is *supposed* to raise
    // - and the matching one requires the interpreted run to finish, which is exactly the property a
    // script like this cannot have.
    ExpectJitRaisesLikeInterpreter(source, "division by zero");

    const auto outcome = RunScript(source, true);
    EXPECT_EQ(outcome.execution_result, AngelScript::asEXECUTION_EXCEPTION);
    EXPECT_EQ(outcome.output, "") << "the statement after the faulting one still ran";
  }

  TEST(JitCompiler, ABoundedScriptCanStillBeAborted) {
    // The behaviour that makes the compiled case safe. The compiler places a suspend instruction
    // before every loop's entry point, and the JIT is expected to run past it only when the context
    // says there is nothing to process - so a script sitting in a compiled loop still reaches the
    // line callback that a timeout is built on.
    //
    // The loop is bounded on purpose. A JIT that ignored suspensions would run it to completion
    // rather than hanging here, and the test would fail on the execution result instead of needing a
    // timeout to kill it.
    const std::string source = R"AS(
      void main() {
        int total = 0;
        for (int i = 0; i < 100000000; i++) {
          total += i;
        }
        show(total);
      }
    )AS";

    for (const bool use_jit : { false, true }) {
      // Built without the compiler and attached below, so that the two passes differ in exactly one
      // thing. Attaching in the constructor and then asserting on it would leave the interpreted pass
      // meaning something subtly different.
      auto* engine = CreateEngine(EngineConfig{});
      ASSERT_NE(engine, nullptr);

      if (use_jit)
        asjit::Attach(engine);

      auto* module = BuildModule(engine, source);
      ASSERT_NE(module, nullptr);

      auto* function = module->GetFunctionByDecl("void main()");
      ASSERT_NE(function, nullptr);

      if (use_jit) {
        ASSERT_NE(function->GetJITFunction(), nullptr) << "the loop was not compiled, so this proves nothing";
      }

      g_line_callbacks = 0;

      auto* context = engine->CreateContext();
      ASSERT_NE(context, nullptr);
      context->SetLineCallback(AngelScript::asFUNCTION(AbortingLineCallback), nullptr, AngelScript::asCALL_CDECL);

      ASSERT_GE(context->Prepare(function), 0);

      const int result = context->Execute();

      EXPECT_EQ(result, AngelScript::asEXECUTION_ABORTED)
          << (use_jit ? "compiled" : "interpreted") << " loop ignored the abort";
      EXPECT_GE(g_line_callbacks, kAbortAfterLineCallbacks);

      context->Release();
      module->Discard();
      engine->ShutDownAndRelease();
    }
  }

  TEST(JitCompiler, CompilesEnoughToBeWorthAttaching) {
    // Everything else here checks that the compiled path is *correct*. None of it would fail if the
    // compiler produced nothing but immediate exits, because a function that hands straight back to
    // the interpreter is indistinguishable from an interpreted one by its results alone.
    //
    // This is the other half: that the translation covers a meaningful part of a function and that
    // running it is actually faster. The margin is deliberately loose - it is asserting an order of
    // magnitude of difference, not a benchmark, and a number this far from 1.0 cannot come from noise
    // on a loop this size.
    const std::string source = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += i * 3 - (i >> 2);
        }
        return total;
      }

      void main() {
        int total = Work(30000000);
        show(total);
      }
    )AS";

    const auto interpreted = RunScript(source, false);
    ASSERT_TRUE(interpreted.built) << interpreted.errors;

    const auto start_interpreted = std::chrono::steady_clock::now();
    const auto plain = RunScript(source, false);
    const auto start_compiled = std::chrono::steady_clock::now();
    const auto compiled = RunScript(source, true);
    const auto end = std::chrono::steady_clock::now();

    ASSERT_TRUE(compiled.built) << compiled.errors;
    EXPECT_EQ(plain.output, compiled.output);

    const auto interpreted_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(start_compiled - start_interpreted).count();
    const auto compiled_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_compiled).count();

    // Printed rather than only asserted, because the useful thing to see when this fails is the
    // actual ratio rather than the fact that it crossed a threshold.
    std::printf("[asjit] loop of 30M: interpreted %.1f ms, compiled %.1f ms, %.2fx\n",
                static_cast<double>(interpreted_ns) / 1e6,
                static_cast<double>(compiled_ns) / 1e6,
                static_cast<double>(interpreted_ns) / static_cast<double>(compiled_ns));

    EXPECT_GT(compiled.blocks_emitted, 0u) << "no basic blocks were translated";
    EXPECT_LT(compiled_ns * 2, interpreted_ns)
        << "the compiled loop was not meaningfully faster, so the translated blocks are probably not running";
  }

  TEST(JitCompiler, SeparatesTranslationGapsFromUnreachedCode) {
    // An instruction is missing from the translated count for one of two reasons: the translator has
    // no case for it, or it sits past the point where translation of its block stopped. The two need
    // opposite responses - one is a gap to close, the other is already handled everywhere the emitter
    // is asked - so the counters have to distinguish them, and this is what asserts they do.
    //
    // The script is shaped to produce both kinds in one function, and to keep producing them as the
    // translator grows. `array<int> a = {1, 2, 3, 4}` compiles to an allocation followed by a run of
    // list-construction instructions, `show(a.length())` is a registered call, and `a[0]` compiles to
    // `asBC_Thiscall1`. So one function holds all three states: instructions the translator has a case
    // for (`AllocMem`, `SetListSize`, `PshListElmnt`, `CALLSYS`, `CALL`), one it was asked for and
    // refused (`Thiscall1`, which the emitter has no case for), and occurrences it was never handed at
    // all.
    //
    // The refusal is last on purpose. It is what ends the block, so everything before it is translated
    // and everything the function does afterwards is unreached by construction rather than by accident.
    //
    // The refusal subject has moved five times, and every move was one of two mistakes: choosing a
    // subject for having the property today rather than by construction, and - three times now -
    // choosing one that the next piece of work translated. `CALL` was the first of those. `Cast` was
    // the next, and it was translated: the reason it was refused - that its operands were four layout
    // members of two internal classes - was wrong, and every one of the four turned out to have a
    // published equivalent.
    //
    // `Thiscall1` is the subject both before and after the fifth move, and the move is worth writing
    // down because it is the only one so far that ended where it started. It was refused on a
    // prediction - that translating it would mean routing an `int` method call through the engine's
    // general system-function path instead of the hand-written fast path the instruction exists to be -
    // and a prediction is not a reason to refuse an instruction, so it was translated. The translation
    // is legitimate on the semantics, which is what made the measurement possible: the general path
    // performs the same null check, sets the same exception, and writes the reference into the same
    // value register. The prediction then held: on the benchmark's `array_index` row - the only row in
    // the suite that reaches the instruction - translating it moved the compiled arm from 36.1 ms to
    // 81.4 ms, so the row's speedup fell from 1.79x to 0.80x, while that row's interpreted arm moved by
    // 0.9% between the two runs. The paragraph and the table are in `emitter.cpp`, and the case is
    // refused again on those numbers.
    //
    // What the numbers were not, it turned out, is the whole reason, and the other one is stronger
    // because it is not a measurement. The fast path is not merely faster, it is unreachable with its
    // semantics intact: the interpreter pairs `CallObjectMethodRetPtr` with a private context field that
    // `SetException` reads, and a translation calling that routine from outside the interpreter runs
    // methods whose raises are silently dropped. `Differential.AThiscall1ShapedMethodThatRaisesAgreesWith
    // TheInterpreter` is the test that says so in the language of the suite - interpreted
    // `asEXECUTION_EXCEPTION` against compiled `asEXECUTION_FINISHED`, and `11\n` against `11\n9\n999\n`
    // - and it is why the general path is the only faithful route. The cost table above is then the
    // reason not to take it, which is what makes this the one refusal in this file's history that is
    // both forced and measured rather than one or the other.
    //
    // What the measurement cannot do is stay true by itself: it is one machine's, and a cheaper general
    // path in a later engine would move it. That is the argument for keeping *this* subject rather than
    // one that cannot change: it is the refusal most likely to be worth re-measuring, in a test whose
    // whole job is to notice when a fact about the translator does.
    //
    // It is also no longer a refusal in the only sense that would matter here. `asBC_Thiscall1` is
    // translated behind `Options::translate_object_method_calls`, so what this test asserts is the
    // *default* - and the flag is what the re-measuring above would be done with rather than a
    // translation that would have to be written again. The benchmark's `fullcov` variant is that
    // configuration and the differential test for the raise runs under both, which together are what
    // make "off by default" a decision rather than a gap.
    //
    // The subject that was *expected* to be here is `asBC_ALLOC`'s value-type branch, and it is not here
    // because it was translated - which makes it the same mistake this history keeps recording, committed
    // one step earlier. The reason for expecting it was that nothing this suite or the benchmark compiles
    // hands the branch over: the instruction is only ever produced on the heap - `IsVariableOnHeap` in
    // as_compiler.cpp:1683 is what decides, and for a value type it is true only for a module global, a
    // reference member or the generated factory stub - and neither the module-init function nor a factory
    // stub is a function the JIT is asked to compile.
    //
    // The two cases it names are real; the generalisation over them was not. A **value type passed by
    // value is constructed on the heap at the call site**, so an ordinary `Take(Vec3(1.0f, 2.0f, 3.0f))`
    // walks one and a value type forwarded through two calls walks two. One probe settled that, and the
    // probe is the part worth keeping: a claim that no script produces an instruction is a claim a script
    // can settle, and this one had stood here unmeasured for three sessions.
    // `JitCompiler.HeapValueTypeConstructionIsReachedAndTranslated` is the probe,
    // `runtime::ConstructValueObject` is the translation, and the paragraph in `emitter.cpp` records both
    // halves of what the earlier refusal got wrong - the reachability, and a tail it treated as
    // inseparable when only one of its two questions is unavailable.
    //
    // The third state is `asBC_JitEntry`, whose property *is* structural and cannot be translated
    // away: the emit loop begins the block for an entry at the entry's *successor*
    // (`function_compiler.cpp`, pass one), because the instruction exists only so the interpreter has
    // somewhere to hand off from and the entry into compiled code has already done everything it says.
    // So an entry that no branch happens to target is walked and never handed over, once per compiled
    // function, whatever the translator supports.
    //
    // What survives is the shape, and it is asserted positively at each of the three states so that
    // the test fails rather than silently passing if the script stops producing one.
    const std::string source = R"AS(
      void main() {
        array<int> a = {1, 2, 3, 4};
        show(a.length());

        // Indexing the array, which is `asBC_Thiscall1` - `int &opIndex(uint)` is exactly the shape
        // the compiler turns into it - and the emitter has no case for it, so this is where the block
        // ends.
        int first = a[0];
        show(first);
      }
    )AS";

    const auto compiled = RunScript(source, true);
    ASSERT_TRUE(compiled.built) << compiled.errors;
    ASSERT_TRUE(compiled.entry_has_jit) << "the script was not compiled, so this proves nothing";

    // Translated, and asked about: the ordinary case, which the other two are defined against. The
    // three list instructions are the allocation and the header stores that precede the constructor
    // call, and both kinds of call are here because both have been the gap this test was naming at
    // some point - the group is where a newly translated instruction is meant to land.
    for (const auto opcode : {AngelScript::asBC_AllocMem, AngelScript::asBC_SetListSize, AngelScript::asBC_PshListElmnt,
                              AngelScript::asBC_CALLSYS, AngelScript::asBC_CALL}) {
      ASSERT_GT(compiled.Walked(opcode), 0u)
          << "the script did not compile to " << AngelScript::asBCInfo[opcode].name
          << ", so this test is not exercising the shape it names";
      EXPECT_GT(compiled.Translated(opcode), 0u) << AngelScript::asBCInfo[opcode].name;
      EXPECT_EQ(compiled.Unimplemented(opcode), 0u) << AngelScript::asBCInfo[opcode].name;
    }

    // A gap: the emitter was asked for the array index and had no case for it, which is why the block
    // ends there. `Unimplemented` is the counter that says so; `Walked - Translated` would count this
    // one, but it would also count everything in the third group below.
    //
    // The equality on `Translated` is what says the refusal is still the default, and it is not a
    // formality: this is the assertion the fifth move of the subject is about - the instruction was
    // translated, measured, and turned back off - and a future default that changed would have to
    // change this line as well as the paragraph above. It is asserted on `RunScript`, which attaches
    // with no options, so what it pins is the configuration the menu runs.
    ASSERT_GT(compiled.Walked(AngelScript::asBC_Thiscall1), 0u)
        << "the script did not compile to an indexed array read, so this test proves nothing";
    EXPECT_EQ(compiled.Translated(AngelScript::asBC_Thiscall1), 0u);
    EXPECT_GT(compiled.Unimplemented(AngelScript::asBC_Thiscall1), 0u)
        << "the array index was neither translated nor reported as refused";

    // Not a gap: an instruction that is in the bytecode and that the emitter has a case for, sitting
    // past the point where translation of its block stopped. It is walked, it is never offered to the
    // translator, and nothing refused it - so it is missing from the translated count for a reason
    // that translating harder would not fix, and the second counter has to be able to say so or
    // `Unimplemented` would be `Walked - Translated` under another name.
    //
    // This is the one group whose assertion is an inequality between the counters rather than an
    // equality, because the state it names *is* that inequality: the entries no branch targets are
    // neither translated nor refused, and `Walked - Translated - Unimplemented` counts exactly them.
    ASSERT_GT(compiled.Walked(AngelScript::asBC_JitEntry), 0u)
        << "the script did not compile to anything the JIT was given an entry into, so this test "
           "proves nothing";
    EXPECT_GT(compiled.Translated(AngelScript::asBC_JitEntry), 0u)
        << "no entry marker was ever handed to the translator, so this opcode is in the same state "
           "as the previous group and does not demonstrate the third";
    EXPECT_EQ(compiled.Unimplemented(AngelScript::asBC_JitEntry), 0u);
    EXPECT_GT(compiled.Walked(AngelScript::asBC_JitEntry),
              compiled.Translated(AngelScript::asBC_JitEntry) +
                  compiled.Unimplemented(AngelScript::asBC_JitEntry))
        << "every walked entry marker was either translated or refused, so the two counters have "
           "collapsed into `Walked - Translated` and no longer tell a gap from an unhanded "
           "occurrence";
  }

  TEST(JitCompiler, IntegerExponentIsTranslatedNotApproximated) {
    // The integer exponent opcodes are translated by calling the interpreter's own routine, so this
    // asserts the two halves of that: they reach the translator as instructions rather than as a
    // hand-back, and they are accepted rather than refused.
    //
    // What the test cannot assert here is that the answers agree - that is differential_test.cpp's
    // job, and it is the one that matters, because the reason these are a *call* rather than a loop
    // of multiplies is the overflow rule. The interpreter decides it against a table of maximum bases
    // and the table is not the one its name suggests; a translation that reproduced the arithmetic
    // instead would return a value where the interpreter raises. Calling `as_powi` is what makes the
    // two agree by construction rather than by a table copied and kept in step.
    const std::string source = R"AS(
      void main() {
        int base = 3;
        int exponent = 4;
        double dbase = 2.5;
        float fbase = 2.0f;

        show(base ** exponent);
        show(dbase ** 2);
        show(dbase ** 2.0);
        show(fbase ** 2.0f);
      }
    )AS";

    const auto compiled = RunScript(source, true);
    ASSERT_TRUE(compiled.built) << compiled.errors;
    ASSERT_TRUE(compiled.entry_has_jit) << "the script was not compiled, so this proves nothing";

    ASSERT_GT(compiled.Walked(AngelScript::asBC_POWi), 0u)
        << "the script did not compile to an integer exponentiation, so this test is not exercising "
           "the instruction it is about";
    EXPECT_GT(compiled.Translated(AngelScript::asBC_POWi), 0u)
        << "an integer exponentiation was left to the interpreter";
    EXPECT_EQ(compiled.Unimplemented(AngelScript::asBC_POWi), 0u);

    // The rest of the family, so that a change to the shared operand reading breaks the tests that
    // cover it rather than passing as "the whole family is declined".
    EXPECT_GT(compiled.Translated(AngelScript::asBC_POWd), 0u);
    EXPECT_GT(compiled.Translated(AngelScript::asBC_POWdi), 0u);
    EXPECT_GT(compiled.Translated(AngelScript::asBC_POWf), 0u);
  }

  TEST(JitCompiler, CountedHandleAssignmentIsTranslated) {
    // The counted half of the reference copies: the counterpart of
    // `Differential.NoCountHandleAssignmentIsTranslated`, which reaches the other half - the one
    // branch of the shared handler that is a plain pointer store.
    //
    // `asBC_REFCPY` and `asBC_RefCpyV` each assign one object handle to another, which is the most
    // common thing a menu script does with a `Player@` or a `Vehicle@`. Their handler bodies branch on
    // the object type - `!(objType->flags & (asOBJ_NOCOUNT | asOBJ_VALUE))` - and the counted branch is
    // the expensive one: it releases what the destination held and takes a reference on the source, so
    // it is the branch a `@player.vehicle = other;` in a per-tick script takes a hundred times a
    // minute. Every type here is a script class, which is `asOBJ_SCRIPT_OBJECT` under `asOBJ_REF`,
    // carrying neither `asOBJ_NOCOUNT` nor `asOBJ_VALUE`, so the counted branch is the one that runs.
    //
    // What it used to be, and why it is not: the translation was refused on the grounds that the
    // release and the addref live in `asCObjectType::beh`, a layout no published header exposes. That
    // was true of reaching them *directly* and false as a conclusion, because the engine exposes the
    // whole operation - `asIScriptEngine::ReleaseScriptObject` and `AddRefScriptObject` are public.
    // Their bodies are not literally these two branches; they are a dispatch that ends in the same
    // `objType->beh.release` and `objType->beh.addref` for a reference type, with branches for the
    // shapes that cannot reach a handle assignment at all. The type test that decides between the two
    // halves of *this* handler is `asITypeInfo::GetFlags` - public - resolved once at compile time
    // rather than once per execution, so the split is the interpreter's own condition read from the
    // interpreter's own data and there is no type the two arms can disagree about. Which is the
    // property a partial translation has to keep, and the reason this test asserts the opcodes rather
    // than the numbers.
    //
    // Asserted rather than merely commented on, because the interesting failure is not this being
    // wrong but this becoming *stale*: a translation that quietly stopped being reached would leave
    // the tests passing and the sentence above describing code that no longer exists, which is what
    // the three opcodes below are here to catch.
    //
    // Every assignment carries the `@` operator, and the script is run rather than only walked, for a
    // reason worth writing down: without the `@` the compiler does not assign the handle. It converts
    // the source to an object reference, checks it for null and copies the object into the destination
    // through the engine's default copy operator for script classes - which raises `Null pointer
    // access` when the destination is a null handle, so the script does not run at all, and compiles
    // to `asBC_CALLSYS` rather than to `asBC_REFCPY`, so a test that only counted opcodes would pass
    // while proving nothing about the instruction it names.
    const std::string source = R"AS(
      class Entity {
        int id = 0;
      }

      class Holder {
        Entity@ entity;
      }

      Entity@ g_entity;

      Entity@ Pass(Entity@ source) { return source; }

      void main() {
        Holder holder;
        @holder.entity = Entity();
        @g_entity = holder.entity;

        Entity@ local = Pass(g_entity);
        show(local.id);
      }
    )AS";

    const auto compiled = RunScript(source, true);
    ASSERT_TRUE(compiled.built) << compiled.errors;
    ASSERT_TRUE(compiled.entry_has_jit) << "the script was not compiled, so this proves nothing";
    ASSERT_EQ(compiled.execution_result, AngelScript::asEXECUTION_FINISHED)
        << "the script did not run: " << compiled.exception_string;

    ASSERT_GT(compiled.Walked(AngelScript::asBC_REFCPY), 0u)
        << "the script did not compile to a handle assignment, so this test is not exercising the "
           "instruction it is about";
    EXPECT_GT(compiled.Translated(AngelScript::asBC_REFCPY), 0u)
        << "a counted handle assignment was left to the interpreter";
    EXPECT_EQ(compiled.Unimplemented(AngelScript::asBC_REFCPY), 0u);

    // The same from the other side: `asBC_RefCpyV` is the assignment into a named local, and the
    // release it owes is of whatever the local held *before* - the counted branch drops the old
    // reference on its way to storing the new one. The two differ only in where the destination is, a
    // pointer popped off the stack against a frame variable named by the instruction, so a change to
    // one that missed the other is the failure this pair is here to catch.
    ASSERT_GT(compiled.Walked(AngelScript::asBC_RefCpyV), 0u);
    EXPECT_GT(compiled.Translated(AngelScript::asBC_RefCpyV), 0u);
    EXPECT_EQ(compiled.Unimplemented(AngelScript::asBC_RefCpyV), 0u);

    // And the third of the family, which is not an assignment at all: `asBC_FREE` is the release at
    // scope exit, and it goes through the engine's routine unconditionally rather than through the
    // branch these two take - there is no cheap case for it to fall back to, which is why it stayed
    // refused for longer than they did.
    ASSERT_GT(compiled.Walked(AngelScript::asBC_FREE), 0u);
    EXPECT_GT(compiled.Translated(AngelScript::asBC_FREE), 0u);
    EXPECT_EQ(compiled.Unimplemented(AngelScript::asBC_FREE), 0u);
  }

  TEST(JitCompiler, TryBoundariesAreNeverHandedToTheTranslator) {
    // `asBC_TryBlock` looks like an instruction - it has an opcode, an argument, and an entry in
    // asBCInfo - but it never reaches the bytecode array. asCByteCode::TryBlock allocates it with
    // size 0 (as_bytecode.cpp:1805) and asCByteCode::Output copies only instructions whose GetSize()
    // is non-zero (as_bytecode.cpp:1948), so the marker is dropped on the way out and the walker
    // here never sees one. The catch target reaches the VM separately, through
    // scriptData->tryCatchInfo, which asCByteCode::ExtractTryCatchInfo builds from the compiler's
    // linked list before Output runs (as_bytecode.cpp:1567). `asBC_Block` and `asBC_VarDecl` are
    // zero-sized and leave the array the same way.
    //
    // So a try boundary is not something the translator can be handed, and therefore not something
    // it can refuse - there is no instruction in the walked stream for it to have no case for. An
    // earlier version of this test asserted the opposite, that the marker was walked and refused,
    // and the Walked assertion is what failed and turned this up.
    //
    // What is still worth pinning is the consequence: the boundary costs no hand-back, because the
    // *contents* of the try have to compile like any other code. `(i & 7) * 3` is the only multiply
    // in the script and it sits inside the try, so MULIi being translated is that claim - if a try
    // made its body fall back, the loop would still alternate between the two engines, just at a
    // different instruction.
    const std::string source = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          try {
            total += (i & 7) * 3;
          } catch {
            total = 0;
          }
        }
        return total;
      }

      void main() { show(Work(64)); }
    )AS";

    const auto compiled = RunScript(source, true);
    ASSERT_TRUE(compiled.built) << compiled.errors;
    ASSERT_TRUE(compiled.entry_has_jit) << "the script was not compiled, so this proves nothing";
    ASSERT_EQ(compiled.execution_result, AngelScript::asEXECUTION_FINISHED)
        << "the script did not run: " << compiled.exception_string;

    EXPECT_EQ(compiled.Walked(AngelScript::asBC_TryBlock), 0u)
        << "a try marker reached the walker, so either the marker is no longer zero-sized or "
           "something now copies it into the bytecode array";
    EXPECT_EQ(compiled.Translated(AngelScript::asBC_TryBlock), 0u);

    ASSERT_GT(compiled.Walked(AngelScript::asBC_MULIi), 0u)
        << "the script did not compile to the multiply the try body is built around, so this test "
           "is not exercising the shape it names";
    EXPECT_GT(compiled.Translated(AngelScript::asBC_MULIi), 0u)
        << "the multiply inside the try was refused, so a try boundary does cost a hand-back";
  }

  TEST(JitCompiler, DeclinesFunctionsWithNoEntryPoints) {
    // A module built with the JitEntry instructions turned off has nothing for the JIT to hook into,
    // and the engine has already warned about it by the time this runs. What matters is that the
    // refusal is clean: the function stays interpreted and no code is left allocated behind it.
    auto* engine = AngelScript::asCreateScriptEngine();
    ASSERT_NE(engine, nullptr);

    engine->SetEngineProperty(AngelScript::asEP_INCLUDE_JIT_INSTRUCTIONS, false);
    engine->SetEngineProperty(AngelScript::asEP_NO_DEBUG_OUTPUT, true);

    auto* module = BuildModule(engine, "void main() { int a = 1; }");
    ASSERT_NE(module, nullptr);

    asjit::ResetStats();
    asjit::Attach(engine);

    auto* function = module->GetFunctionByDecl("void main()");
    ASSERT_NE(function, nullptr);
    EXPECT_EQ(function->GetJITFunction(), nullptr);

    // The module was built before the compiler was attached, so nothing was offered to it at all.
    EXPECT_EQ(asjit::GetStats().functions_compiled, 0u);

    module->Discard();
    engine->ShutDownAndRelease();
  }

  TEST(JitCompiler, HeapValueTypeConstructionIsReachedAndTranslated) {
    // Why this is a test of its own rather than only a differential comparison.
    //
    // The value-type branch of `asBC_ALLOC` was recorded as unreachable for three sessions, on the
    // argument that the variables it applies to - module globals, reference members, factory stubs -
    // are never inside a function the JIT is offered. The differential test beside this one asserts
    // the branch *behaves* correctly once reached; this one asserts it is reached at all, and the two
    // are not the same claim. A script can be written that exercises the instruction perfectly and
    // stops reaching it after an unrelated compiler change, and the comparison would keep passing with
    // the translation it names never running - which is the exact way the earlier, wrong record
    // survived so long.
    //
    // The globals are the interesting half of the script. They are the shape the discarded argument
    // was about, and they are here to keep its one true part visible: neither produces an `ALLOC` the
    // compiler ever walks, because module globals are initialised by a generated function the engine
    // does not offer. The count below is therefore the by-value arguments and nothing else.
    const auto outcome = RunScript(R"AS(
      Vec3 gV(1.0f, 2.0f, 3.0f);
      void Take(Vec3 v) { show(v.x); }
      void main() { Take(Vec3(4.0f, 5.0f, 6.0f)); show(gV.x); }
    )AS", true);

    ASSERT_TRUE(outcome.built) << outcome.errors;
    EXPECT_GT(outcome.Walked(AngelScript::asBC_ALLOC), 0u)
        << "no heap-allocated value type construction reached the translator";
    EXPECT_EQ(outcome.Unimplemented(AngelScript::asBC_ALLOC), 0u)
        << "the value-type branch of ALLOC was handed back to the interpreter";
    EXPECT_GT(outcome.Translated(AngelScript::asBC_ALLOC), 0u);
  }

} // namespace base::asjit::test
