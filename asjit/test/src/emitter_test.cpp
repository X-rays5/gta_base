//
// Created by X-ray on 13/09/2026.
//

// What the translator emits, checked by assembling it and asking asmjit about the result rather than
// by running the code and looking at what it computed.
//
// Those are two different questions and only the second one is covered by the differential suite.
// That suite is the stronger claim about *behaviour* - two arms run the same script and their output
// is compared - and it is blind to the shape of what was emitted. A guard whose branch was encoded as
// a jump to the next instruction computes exactly the right answer for as long as nothing faults, and
// that is not a hypothetical: asmjit writes a displacement of zero for a label it has not bound yet
// and patches it when the label *is* bound, so a branch to a label nothing ever binds is a valid
// encoding of "do nothing". It assembles, it is not reported as an unresolved fixup by anything that
// reads one, and the fast path never takes the branch. `asBC_COPY` had exactly that: its second guard
// overwrote the label its first guard had already branched to, so its destination-pointer check did
// nothing at all.
//
// What is asserted below is therefore the two things only the assembler can answer - that every
// branch the translator makes is to a label the caller is handed, and that an instruction it declined
// was declined without emitting anything - applied to instructions taken from scripts the engine
// compiled rather than written out by hand here.

#include "test_support.hpp"

#include <abi.hpp>
#include <emitter.hpp>

#include <asmjit/x86.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace base::asjit::test {
  namespace {
    using support::Build;
    using support::Script;
    using ByteCode = base::asjit::ByteCode;

    // The register plan the real prologue establishes, named through `reg` rather than chosen here so
    // that these tests assemble the same code the compiler does. A test that picked its own registers
    // would keep passing if the compiler's plan changed, which is the one thing it is here to notice.
    [[nodiscard]] Emitter::Abi ProductionAbi() {
      return {reg::kRegs, reg::kStackPointer, reg::kFramePointer};
    }

    // The opcode's name as the engine spells it, for failure messages.
    [[nodiscard]] const char* OpName(AngelScript::asEBCInstr opcode) {
      return AngelScript::asBCInfo[opcode].name;
    }

    // asmjit 1.23 does not keep an error on the emitter - it reports one to the code holder's handler,
    // which is what the compiler does with its own collector and what the default configuration does
    // with nothing at all. Attaching one here is what makes an instruction asmjit refused visible
    // rather than silently absent.
    class ErrorSink final : public asmjit::ErrorHandler {
    public:
      void handle_error(asmjit::Error err, const char* message, asmjit::BaseEmitter*) override {
        if (error == asmjit::kErrorOk) {
          error = err;
          text = message != nullptr ? message : "";
        }
      }

      asmjit::Error error = asmjit::kErrorOk;
      std::string text;
    };

    // One instruction, assembled on its own.
    struct Translation {
      Emitter::Outcome outcome = Emitter::Outcome::Unsupported;

      // Whether the translator created an exit label - the one it hands back through `out_fault`, and
      // the only one the caller can bind.
      bool fault_label_created = false;

      // Fixups still unresolved after the label the translator returned was bound. Anything above zero
      // is a branch to a label nobody will ever bind, which asmjit encodes as a branch to the next
      // instruction: a check that silently does nothing rather than one that fails.
      std::size_t unbound_after_binding = 0;

      // How many branches the returned label resolved - one per guard that branches to it. So an
      // instruction with two guards resolves two, and one whose second guard stole the label resolves
      // one and leaves one dangling.
      std::size_t guards_bound = 0;

      // Bytes of machine code the instruction produced, and the error the assembler was left in. The
      // second is not a formality: `Emit` returns an Outcome and not an asmjit error, so a call asmjit
      // rejected is otherwise invisible - the translator would report a translation that does not
      // exist and the block would execute whatever encoded the rejection.
      std::size_t code_size = 0;
      asmjit::Error error = asmjit::kErrorOk;
      std::string error_message;
    };

    [[nodiscard]] Translation Assemble(const Instruction& instruction) {
      asmjit::CodeHolder code;
      // Cannot fail for the host environment, which is the only one this suite builds for, and there
      // is nothing to assert with here that would be worth the two lines.
      if (code.init(asmjit::Environment::host()) != asmjit::kErrorOk)
        return {};

      ErrorSink errors;
      code.set_error_handler(&errors);

      asmjit::x86::Assembler assembler(&code);
      Emitter emitter(assembler, ProductionAbi());

      asmjit::Label fault;
      Translation result;
      result.outcome = emitter.Emit(instruction, &fault);
      result.error = errors.error;
      result.error_message = errors.text;
      result.code_size = code.code_size();
      result.fault_label_created = fault.is_valid();

      const auto pending = code.unresolved_fixup_count();
      if (fault.is_valid())
        assembler.bind(fault);
      result.unbound_after_binding = code.unresolved_fixup_count();
      result.guards_bound = pending - result.unbound_after_binding;
      return result;
    }

    // One instruction of a compiled script, with what the translator made of it.
    struct Translating {
      AngelScript::asEBCInstr opcode = AngelScript::asBC_PopPtr;

      // The instruction's own words, copied out of the module's bytecode. The engine that produced
      // them is released before the caller sees this, so a pointer into it would dangle; a copy also
      // means a test can read the arguments the translator read.
      std::vector<ByteCode> words;

      Translation translation;
    };

    // Compiles `source` and hands every instruction of every function in it to the translator on its
    // own.
    //
    // The bytecode comes from the engine rather than from an encoding written out by hand here, so
    // what is translated is what a script actually compiles to, and a change to the engine's own
    // emission is covered without anyone having to remember to update a table. The walk is the
    // interpreter's - step by each instruction's size - because that is the only thing the bytecode
    // itself offers and the only thing the compiler's own passes do.
    [[nodiscard]] std::vector<Translating> TranslateModule(const char* module_name, const std::string& source) {
      // Built with the JIT detached. Nothing here runs, so an attached compiler would only be a second
      // route to the same bytecode.
      EngineConfig config;
      const auto engine = std::unique_ptr<AngelScript::asIScriptEngine, void (*)(AngelScript::asIScriptEngine*)>(
          CreateEngine(config), [](AngelScript::asIScriptEngine* released) { released->ShutDownAndRelease(); });

      const Script script = Build(engine.get(), module_name, source);
      if (!script.built) {
        // `ASSERT_*` is not available in a function that returns something, and this is worth
        // reporting rather than leaving the caller to notice an empty vector.
        EXPECT_TRUE(script.built) << "the script did not compile, so there is nothing to translate: "
                                  << support::Errors();
        return {};
      }

      std::vector<Translating> translations;
      for (AngelScript::asUINT i = 0; i < script.module->GetFunctionCount(); ++i) {
        AngelScript::asIScriptFunction* function = script.module->GetFunctionByIndex(i);

        AngelScript::asUINT length = 0;
        const ByteCode* byte_code = function->GetByteCode(&length);
        if (byte_code == nullptr)
          continue;

        for (std::size_t at = 0; at < static_cast<std::size_t>(length);) {
          const Instruction instruction(byte_code + at);
          const std::size_t size = instruction.size();

          Translating translating;
          translating.opcode = instruction.opcode();
          translating.words.assign(byte_code + at, byte_code + at + size);
          translating.translation = Assemble(instruction);
          translations.push_back(std::move(translating));

          // Zero is not a size any opcode in the table has. Guarded against anyway, because the walk
          // would spin forever on one and a hang is a worse failure than a report - and the report is
          // an `EXPECT` rather than an `ASSERT` because a function that returns a value cannot use
          // one.
          if (size == 0) {
            EXPECT_GT(size, 0u) << OpName(instruction.opcode()) << " reported no size";
            break;
          }
          at += size;
        }
      }

      return translations;
    }

    // The invariants every translation has to hold, asserted for one instruction.
    //
    // All of them are about the *assembled* code rather than about the script, which is why they are
    // here and not in the differential suite: nothing that runs the code can see any of them.
    void ExpectSoundTranslation(const Translating& translating) {
      const char* name = OpName(translating.opcode);
      const Translation& translation = translating.translation;

      EXPECT_EQ(translation.error, asmjit::kErrorOk)
          << "asmjit rejected an instruction emitted for " << name << ": " << translation.error_message
          << ". The translator never sees that error, so this is a translation it reports as having "
             "happened and that is not there";

      EXPECT_EQ(translation.unbound_after_binding, 0u)
          << name << " branches to a label the caller is never handed, and asmjit encodes a branch to "
                     "an unbound label as a branch to the next instruction - so the check it was "
                     "emitted for does nothing, silently, and only when it was supposed to fire";

      if (translation.outcome == Emitter::Outcome::Unsupported) {
        // Documented as emitting nothing, so that the caller can end the block at exactly this
        // instruction. Bytes emitted and then declined would be instructions nothing can reach, since
        // the block stops here - and they would be counted as code that covers an opcode the
        // interpreter is about to run anyway.
        EXPECT_EQ(translation.code_size, 0u) << name << " was declined after emitting something";
        EXPECT_FALSE(translation.fault_label_created)
            << name << " was declined but still created an exit label";
        return;
      }

      if (translating.opcode == AngelScript::asBC_JitEntry) {
        // The one translation that is deliberately nothing. Once execution is already inside compiled
        // code the marker has no effect left to reproduce - the save, the call and the reload the
        // interpreter does around it are what entering this code already did.
        EXPECT_EQ(translation.code_size, 0u) << "asBC_JitEntry stopped being a no-op";
      } else {
        // Every other instruction the translator accepts has to produce code. "Translated" is what the
        // function compiler counts as coverage, so an opcode accepted into an empty translation is an
        // instruction that is neither compiled nor handed back to the interpreter - the one outcome
        // the counters have no way to describe.
        EXPECT_GT(translation.code_size, 0u) << name << " was translated into no code at all";
      }

      if (translation.outcome == Emitter::Outcome::TranslatedWithFaultRoute) {
        EXPECT_TRUE(translation.fault_label_created)
            << name << " reports a fault route but handed back no label to route it to";
        EXPECT_GT(translation.guards_bound, 0u)
            << name << " reports a fault route and the label it handed back has nothing branching to "
                       "it, so the exit it promises is not the exit its guard takes";
      } else {
        EXPECT_FALSE(translation.fault_label_created)
            << name << " created an exit label while reporting no fault route, which means the caller "
                       "has no way to bind it";
      }
    }

    [[nodiscard]] std::set<AngelScript::asEBCInstr> FaultRoutesOf(const std::vector<Translating>& translations) {
      std::set<AngelScript::asEBCInstr> routes;
      for (const auto& translating : translations) {
        if (translating.translation.outcome == Emitter::Outcome::TranslatedWithFaultRoute)
          routes.insert(translating.opcode);
      }
      return routes;
    }

    // The scripts the sweep below translates, chosen so that between them they reach every faulting
    // family the translator has: the four division and remainder pairs, the four exponentiation forms,
    // the pointer checks that guard an object access, and the block copy.
    constexpr const char* kArithmeticScript = R"AS(
      int DivI(int a, int b) { return a / b; }
      int ModI(int a, int b) { return a % b; }
      uint DivU(uint a, uint b) { return a / b; }
      uint ModU(uint a, uint b) { return a % b; }
      int64 DivL(int64 a, int64 b) { return a / b; }
      int64 ModL(int64 a, int64 b) { return a % b; }
      float DivF(float a, float b) { return a / b; }
      float ModF(float a, float b) { return a % b; }
      double DivD(double a, double b) { return a / b; }
      double ModD(double a, double b) { return a % b; }
      double PowD(double base, double exponent) { return base ** exponent; }
      float PowF(float base, float exponent) { return base ** exponent; }

      void main() {
        for (int i = 1; i <= 3; i++) {
          show(DivI(100, i));
          show(ModI(100, i));
          show(DivU(100, uint(i)));
          show(ModU(100, uint(i)));
          show(DivL(100, int64(i)));
          show(ModL(100, int64(i)));
          show(DivF(100.0f, float(i)));
          show(ModF(100.0f, float(i)));
          show(DivD(100.0, double(i)));
          show(ModD(100.0, double(i)));
          show(PowD(double(i), 2.5));
          show(PowF(float(i), 2.5f));
          show(i ** 2);
          show(uint(i) ** 2);
          show(double(i) ** 2);
        }
      }
    )AS";

    constexpr const char* kObjectScript = R"AS(
      void main() {
        string s = "hello";
        s += " world";
        show(s);
        show(s.length());

        array<int> a = {1, 2, 3};
        a.insertLast(4);
        show(a.length());
        show(a[0]);

        Vec3 v(1.0f, 2.0f, 3.0f);
        Vec3 w = v;
        show(w.x);
        show(v.GetX());

        Block block;
        block.head = 1.5f;
        block.tail = 2.5f;
        Block copy = block;
        show(copy.tail);
      }
    )AS";
  } // namespace

  TEST(Emitter, EveryTranslationBindsWhatItBranchesTo) {
    // Every instruction of two compiled scripts, assembled one at a time. The assertions are the
    // structural ones `ExpectSoundTranslation` documents; what this test adds over the differential
    // suite is coverage of the *code*, over whatever the scripts happen to compile to.
    std::vector<Translating> translations = TranslateModule("asjit_emitter_arith", kArithmeticScript);
    const auto objects = TranslateModule("asjit_emitter_objects", kObjectScript);
    translations.insert(translations.end(), objects.begin(), objects.end());

    ASSERT_FALSE(translations.empty());

    std::size_t translated = 0;
    for (const auto& translating : translations) {
      ExpectSoundTranslation(translating);
      if (translating.translation.outcome != Emitter::Outcome::Unsupported)
        ++translated;
    }

    EXPECT_GT(translated, 0u) << "every instruction of both scripts was declined, so nothing above "
                                 "was asserted about any emitted code";

    // Which faulting families the scripts actually reached. Named rather than counted, because a test
    // whose subject never appeared is a test that passes and proves nothing - and a script change that
    // stopped reaching one of these would otherwise turn it into exactly that without failing.
    const std::set<AngelScript::asEBCInstr> routes = FaultRoutesOf(translations);
    const std::set<AngelScript::asEBCInstr> expected{
        AngelScript::asBC_DIVi,   AngelScript::asBC_MODi,   AngelScript::asBC_DIVu,   AngelScript::asBC_MODu,
        AngelScript::asBC_DIVi64, AngelScript::asBC_MODi64, AngelScript::asBC_DIVf,   AngelScript::asBC_MODf,
        AngelScript::asBC_DIVd,   AngelScript::asBC_MODd,   AngelScript::asBC_POWd,   AngelScript::asBC_POWf,
        AngelScript::asBC_POWi,   AngelScript::asBC_COPY,
    };

    std::string missing;
    for (const auto opcode : expected) {
      if (!routes.contains(opcode))
        missing += std::string(missing.empty() ? "" : ", ") + OpName(opcode);
    }
    EXPECT_TRUE(missing.empty())
        << "these faulting instructions were never reached, so their guards were never checked: "
        << missing;
  }

  TEST(Emitter, CopyGuardsBothBranchToOneExit) {
    // `asBC_COPY` is the only instruction with two guards - the destination pointer and the source
    // pointer - and therefore the only one where a guard can take the exit label out from under the
    // guard before it. That is the bug this test exists for: the second guard used to replace the
    // label rather than reuse it, leaving the destination check encoded as a branch to the next
    // instruction.
    //
    // Both encodings are covered, because they are separate paths through the same case: a copy up to
    // `kInlineCopyBytes` is unrolled into straight-line moves and a larger one is handed to `rep
    // movsb`. The guards are emitted before either of them, so a fix that held for one and not the
    // other is not something this could hide.
    const auto translations = TranslateModule("asjit_emitter_copy", kObjectScript);

    std::size_t inline_copies = 0;
    std::size_t runtime_copies = 0;
    for (const auto& translating : translations) {
      if (translating.opcode != AngelScript::asBC_COPY)
        continue;

      ExpectSoundTranslation(translating);

      // Two, and exactly two: one branch per guard, both to the label the caller was handed. A single
      // fixup here is the second guard having stolen the label - the first guard would then be the
      // unbound one, which the assertion above catches, and this is the one that says *why*.
      EXPECT_EQ(translating.translation.guards_bound, 2u)
          << "COPY has two pointer guards and they have to share the exit the caller binds; binding "
             "the label the translator returned resolved "
          << translating.translation.guards_bound << " of them";

      const auto size_in_bytes = static_cast<std::size_t>(Instruction(translating.words.data()).word0()) * kDwordSize;
      if (size_in_bytes <= static_cast<std::size_t>(Emitter::kInlineCopyBytes))
        ++inline_copies;
      else
        ++runtime_copies;
    }

    EXPECT_GT(inline_copies, 0u) << "no copy small enough to be unrolled was reached";
    EXPECT_GT(runtime_copies, 0u) << "no copy large enough to be handed to `rep movsb` was reached";
  }
} // namespace base::asjit::test
