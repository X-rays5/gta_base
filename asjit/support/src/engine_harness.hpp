//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>

#include <array>
#include <cstdint>
#include <string>

// The one definition of "an AngelScript engine configured the way asjit expects", shared by the test
// suite and the benchmark.
//
// It lives here rather than being duplicated because the two have to agree on it and cannot check
// each other. The tests establish that the compiled code is *correct*; the benchmark establishes that
// it is *worth it*; both claims are about the same engine configuration, and if the benchmark built
// its engine differently from the tests it would be measuring a configuration nothing had verified.
//
// No gtest here. The test-only assertion helpers stay in asjit/test, so that this can be linked by
// anything.
namespace base::asjit::support {

  struct EngineConfig {
    // Whether the JIT is attached. The single difference between the two arms of every comparison.
    bool use_jit = false;

    // asEP_INCLUDE_JIT_INSTRUCTIONS. Left on in both arms of a comparison, because turning it off
    // changes the bytecode itself - the optimizer deletes every JIT entry point without it - and the
    // two runs would then be two different programs rather than two ways of running one.
    bool include_jit_instructions = true;

    bool optimize_bytecode = true;

    // As the menu sets it, which forbids `int / int` and `int % int` at compile time. Off by default
    // here so that the test suite exercises the interpreter's division handlers, which is where some
    // of the more awkward behaviours live; the benchmark turns it on, because the benchmark is about
    // what the menu actually runs.
    bool disable_integer_division = false;

    // Installs a line callback, which is what makes the engine treat every statement's suspend
    // instruction as a hand-off point rather than something compiled code may run past. The menu
    // never installs one; this is the watchdog configuration, and the worst case for the JIT.
    //
    // Honoured by `RunScriptWithConfig`, so that both arms of a comparison run under it and the run
    // reports how often the callback fired.
    bool line_callback = false;

    // The std::string, array and math add-ons the menu registers.
    bool register_add_ons = true;

    // The POD value types the menu registers, of which `Vector3` is the one scripts actually touch.
    // Separate from the add-ons above because they are not add-ons: they are the menu's own binding
    // types, and this is the only way a script reaches asBC_COPY. See the definition for why a class
    // declared inside the script cannot substitute for them.
    bool register_value_types = true;

    // A reference type registered `asOBJ_REF | asOBJ_NOCOUNT`, which is the only thing that makes the
    // reference-copy handlers take their cheap branch. See the definition: for a counted type those
    // instructions still go back to the interpreter, so without this flag the translation of the cheap
    // branch would have nothing to run against.
    bool register_nocount_types = true;

    // Both of `asjit::Options`, which is the whole of the translator's configuration.
    //
    // Off is the shipped configuration, and every test that does not name this flag is a test of what
    // the menu runs. On is the configuration in which the two instructions the translator hands back
    // by default are translated instead - the return and the pointer-returning method call - and the
    // suite is run under it for one reason: a translation that only exists behind a flag is a
    // translation nothing has verified, and the claim being made about those two is not "they were
    // never written" but "they were written, measured and turned off".
    bool translate_returns = false;
    bool translate_object_method_calls = false;
  };

  // A fresh engine with the output bindings registered and errors routed to Errors().
  [[nodiscard]] AngelScript::asIScriptEngine* CreateEngine(const EngineConfig& config);

  // ---------------------------------------------------------------------------
  // Script output
  // ---------------------------------------------------------------------------
  //
  // Scripts write through the registered `show(...)` overloads, one per type so that a test can
  // print exactly the width it means to compare. A global buffer rather than a user-data pointer
  // because the bindings are plain C functions; there is only ever one script running.

  void ClearOutput();
  [[nodiscard]] const std::string& Output();

  // Everything the engine reported at error level, in the order it reported it.
  void ClearErrors();
  [[nodiscard]] const std::string& Errors();

  // ---------------------------------------------------------------------------
  // Building and running
  // ---------------------------------------------------------------------------

  struct Script {
    AngelScript::asIScriptModule* module = nullptr;
    AngelScript::asIScriptFunction* entry = nullptr;
    bool built = false;
  };

  // Compiles `source` into a module called `module_name`, resolving `entry_decl` in it. On failure
  // `built` is false and the reason is in Errors().
  [[nodiscard]] Script Build(AngelScript::asIScriptEngine* engine, const char* module_name, const std::string& source, const char* entry_decl = "void main()");

  struct Execution {
    // What asIScriptContext::Execute returned, or -1 if it was never reached.
    int result = -1;
    std::string exception;
    bool prepared = false;
  };

  // Runs `entry` once to completion on a context of its own.
  [[nodiscard]] Execution Execute(AngelScript::asIScriptFunction* entry, bool line_callback = false);

  // How many times the line callback above has fired since it was last reset. Only meaningful in a
  // configuration that installs one.
  void ResetLineCallbackCount();
  [[nodiscard]] std::uint64_t LineCallbackCount();

  // ---------------------------------------------------------------------------
  // The whole thing, for the common case
  // ---------------------------------------------------------------------------

  struct RunOutcome {
    // Everything the script printed, in order, one value per line.
    std::string output;

    // Compile errors, when there were any.
    std::string errors;

    std::uint64_t functions_seen = 0;
    std::uint64_t functions_compiled = 0;
    std::uint64_t functions_declined = 0;
    std::uint64_t blocks_emitted = 0;
    std::uint64_t instructions_translated = 0;
    std::uint64_t instructions_total = 0;
    std::uint64_t code_size = 0;

    int execution_result = -1;
    std::string exception_string;

    // How many times the engine called the line callback, or zero when the configuration installs
    // none. This is the one observable that separates a suspend the compiled code handed back from one
    // it ran past: the engine calls the callback when it executes a suspend instruction and the line
    // has moved on, so two arms that agree about the output and disagree about this have agreed about
    // what the instructions did and not about which of them ran at all - an instruction run twice, or
    // not run, is invisible to the output when running it twice happens to be harmless.
    std::uint64_t line_callbacks = 0;

    bool built = false;
    bool entry_found = false;

    // What the engine believes about the entry point. The authoritative answer to "did the JIT
    // engage", as opposed to the counters, which could be counting a compile that was then rejected.
    bool entry_has_jit = false;

    // How often each opcode appeared, was translated, and was declined by the translator, indexed by
    // opcode byte. Carried through so that a test can assert that the instruction it means to
    // exercise was actually compiled, rather than passing because the script happened to produce the
    // right answer interpreted.
    std::array<std::uint64_t, 256> opcodes_walked{};
    std::array<std::uint64_t, 256> opcodes_translated{};
    std::array<std::uint64_t, 256> opcodes_unimplemented{};

    [[nodiscard]] double Coverage() const {
      return instructions_total == 0 ? 0.0
                                     : static_cast<double>(instructions_translated) / static_cast<double>(instructions_total);
    }

    [[nodiscard]] std::uint64_t Walked(AngelScript::asEBCInstr opcode) const {
      return opcodes_walked[static_cast<std::size_t>(opcode)];
    }

    [[nodiscard]] std::uint64_t Translated(AngelScript::asEBCInstr opcode) const {
      return opcodes_translated[static_cast<std::size_t>(opcode)];
    }

    // How often the translator was asked for this opcode and had no case for it. Distinct from
    // `Walked(op) - Translated(op)`, which also counts the instructions sitting past the point where
    // translation of their block stopped: those are untranslated because nothing ever asked for them,
    // and only this one is a gap in the translator.
    [[nodiscard]] std::uint64_t Unimplemented(AngelScript::asEBCInstr opcode) const {
      return opcodes_unimplemented[static_cast<std::size_t>(opcode)];
    }
  };

  // Builds and runs a script on a fresh engine. The counters are per run rather than cumulative, so
  // that a caller can ask what this one script cost.
  [[nodiscard]] RunOutcome RunScript(const std::string& source, bool use_jit, const char* entry = "void main()");

  // The same, with the rest of the configuration chosen by the caller. `config.use_jit` is the one
  // field that is ignored: it is the difference between the two arms of a comparison, so it belongs to
  // whoever runs the comparison rather than to the caller here.
  //
  // This exists because one translated instruction, `asBC_ChkRefS`, is only ever compiled by an engine
  // that is not optimizing its bytecode - see the note on the tests that name it. A test that could not
  // choose the engine configuration could not reach it at all.
  [[nodiscard]] RunOutcome RunScriptWithConfig(const std::string& source, const EngineConfig& config,
                                              const char* entry = "void main()");

} // namespace base::asjit::support
