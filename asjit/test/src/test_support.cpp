//
// Created by X-ray on 13/09/2026.
//

#include "test_support.hpp"

#include <gtest/gtest.h>

#include <string>

#include <initializer_list>

namespace base::asjit::test {

  namespace {
    // Shared by the four comparisons below, because the reason it exists is the same in all of them:
    // a translated opcode that the script never produced, or that the translator never accepted, is a
    // test passing for a reason other than the one it names.
    //
    // Reported as a list rather than one failure per opcode, because the useful thing to see when a
    // named opcode is missing is which opcodes the script produced instead - usually a near neighbour
    // the compiler chose rather than the one that was expected.
    void AssertOpcodeCoverage(const RunOutcome& compiled, const std::string& label,
                              std::initializer_list<AngelScript::asEBCInstr> expected_opcodes) {
      bool missing = false;
      for (const auto opcode : expected_opcodes) {
        if (compiled.Translated(opcode) == 0u)
          missing = true;
      }
      if (!missing)
        return;

      std::string translated;
      for (std::size_t opcode = 0; opcode < compiled.opcodes_translated.size(); ++opcode) {
        if (compiled.opcodes_translated[opcode] == 0)
          continue;
        const char* name = AngelScript::asBCInfo[opcode].name;
        translated += (name != nullptr ? name : "?");
        translated += " ";
      }

      for (const auto opcode : expected_opcodes) {
        const char* name = AngelScript::asBCInfo[opcode].name;
        EXPECT_GT(compiled.Translated(opcode), 0u)
            << label << ": " << (name != nullptr ? name : "?")
            << " was never compiled, so this test is not exercising the translation it names.\n"
            << "  script built: " << (compiled.built ? "yes" : "no")
            << ", functions compiled: " << compiled.functions_compiled
            << ", instructions translated: " << compiled.instructions_translated << "\n"
            << (compiled.errors.empty() ? "" : "  build errors: " + compiled.errors) //
            << "  opcodes this script did compile: " << translated;
      }
    }

    // The assertions every comparison makes before it looks at results. Shared for the same reason:
    // "the JIT engaged at all" is a precondition of both, not a claim of either.
    void AssertJitEngaged(const RunOutcome& compiled, const std::string& label) {
      EXPECT_GT(compiled.functions_compiled, 0u) << label << ": the JIT declined every function";
      EXPECT_TRUE(compiled.entry_has_jit) << label << ": the entry point has no compiled code";
    }

    // The same list, one step earlier: whether the translator was asked at all. Reported the same way
    // and for the same reason - what the script produced instead is the useful thing to see.
    void AssertOpcodeReached(const RunOutcome& compiled, const std::string& label,
                             std::initializer_list<AngelScript::asEBCInstr> expected_opcodes) {
      bool missing = false;
      for (const auto opcode : expected_opcodes) {
        if (compiled.Walked(opcode) == 0u)
          missing = true;
      }
      if (!missing)
        return;

      std::string walked;
      for (std::size_t opcode = 0; opcode < compiled.opcodes_walked.size(); ++opcode) {
        if (compiled.opcodes_walked[opcode] == 0)
          continue;
        const char* name = AngelScript::asBCInfo[opcode].name;
        walked += (name != nullptr ? name : "?");
        walked += " ";
      }

      for (const auto opcode : expected_opcodes) {
        const char* name = AngelScript::asBCInfo[opcode].name;
        EXPECT_GT(compiled.Walked(opcode), 0u)
            << label << ": " << (name != nullptr ? name : "?")
            << " was never produced, so this test is not reaching the instruction it names.\n"
            << "  script built: " << (compiled.built ? "yes" : "no")
            << ", functions compiled: " << compiled.functions_compiled << "\n"
            << (compiled.errors.empty() ? "" : "  build errors: " + compiled.errors) //
            << "  opcodes this script did produce: " << walked;
      }
    }
  } // namespace

  void ExpectJitMatchesInterpreterUsing(const std::string& source, const std::string& label,
                                        std::initializer_list<AngelScript::asEBCInstr> expected_opcodes,
                                        const EngineConfig& config) {
    EngineConfig compiled_config = config;
    compiled_config.use_jit = true;

    AssertOpcodeCoverage(RunScriptWithConfig(source, compiled_config), label, expected_opcodes);
    ExpectJitMatchesInterpreter(source, label, config);
  }

  void ExpectJitRaisesLikeInterpreterUsing(const std::string& source, const std::string& label,
                                           std::initializer_list<AngelScript::asEBCInstr> expected_opcodes,
                                           const EngineConfig& config) {
    EngineConfig compiled_config = config;
    compiled_config.use_jit = true;

    AssertOpcodeCoverage(RunScriptWithConfig(source, compiled_config), label, expected_opcodes);
    ExpectJitRaisesLikeInterpreter(source, label, config);
  }

  void ExpectJitRaisesLikeInterpreterReaching(const std::string& source, const std::string& label,
                                              std::initializer_list<AngelScript::asEBCInstr> expected_opcodes,
                                              const EngineConfig& config) {
    EngineConfig compiled_config = config;
    compiled_config.use_jit = true;

    AssertOpcodeReached(RunScriptWithConfig(source, compiled_config), label, expected_opcodes);
    ExpectJitRaisesLikeInterpreter(source, label, config);
  }

  void ExpectJitMatchesInterpreter(const std::string& source, const std::string& label, const EngineConfig& config) {
    EngineConfig interpreted_config = config;
    interpreted_config.use_jit = false;

    EngineConfig compiled_config = config;
    compiled_config.use_jit = true;

    const auto interpreted = RunScriptWithConfig(source, interpreted_config);
    const auto compiled = RunScriptWithConfig(source, compiled_config);

    ASSERT_TRUE(interpreted.built) << label << ": the script did not build without the JIT:\n"
                                   << interpreted.errors;
    ASSERT_TRUE(compiled.built) << label << ": the script did not build with the JIT:\n"
                                << compiled.errors;
    ASSERT_TRUE(interpreted.entry_found) << label << ": no entry point in either build";

    AssertJitEngaged(compiled, label);

    // The interpreted run has to get to the end. Without this the comparison below is satisfiable by
    // two runs that raise on the same early statement: the outputs match because both are truncated at
    // the same place, the results match because both are exceptions, and every instruction the test
    // names *after* that statement is compared without ever having executed. That is not a hypothetical
    // - a script that assigns a handle without the `@` operator raises on its first statement, and this
    // helper went on reporting agreement for the rest of the function until this assertion was added.
    //
    // Coverage of the named opcodes is unaffected either way, since the counters are per compilation
    // rather than per execution; what this protects is the claim that the two runs agreed about what
    // the instructions *did*.
    ASSERT_EQ(interpreted.execution_result, AngelScript::asEXECUTION_FINISHED)
        << label << ": the interpreted run raised '" << interpreted.exception_string
        << "', so nothing after that statement was compared";

    EXPECT_EQ(interpreted.output, compiled.output) << label << ": the JIT disagrees with the interpreter";
    EXPECT_EQ(interpreted.execution_result, compiled.execution_result)
        << label << ": the two runs ended differently";
    EXPECT_EQ(interpreted.exception_string, compiled.exception_string)
        << label << ": the two runs raised different exceptions";

    // And, when the configuration makes every statement a hand-off point, that the two arms handed
    // back the same number of times.
    //
    // Comparing outputs is not enough here, and the gap is wide rather than theoretical: while the
    // watchdog runs, nearly every instruction is executed by the interpreter in both arms, so a
    // compiled block that resumed at the wrong statement would print exactly the same values and be
    // wrong only about which statement it was on. What it cannot hide is the callback, which the
    // engine calls once per suspend instruction executed when the line has moved on - so a resume
    // point one instruction off is a count off by one, and a compiled block that swallowed a suspend
    // is a count short.
    //
    // The count is asserted non-zero before it is compared. Two runs that installed no callback at all
    // would agree on zero, which is how this comparison would pass while testing the one thing it is
    // not for - the same shape of vacuity as a test whose opcode never reached the translator.
    if (config.line_callback) {
      ASSERT_GT(interpreted.line_callbacks, 0u)
          << label << ": the line callback never fired, so nothing was handed back to compare";
      EXPECT_EQ(interpreted.line_callbacks, compiled.line_callbacks)
          << label << ": the compiled run handed back to the engine a different number of times";
    }
  }

  void ExpectJitRaisesLikeInterpreter(const std::string& source, const std::string& label, const EngineConfig& config) {
    EngineConfig interpreted_config = config;
    interpreted_config.use_jit = false;

    EngineConfig compiled_config = config;
    compiled_config.use_jit = true;

    const auto interpreted = RunScriptWithConfig(source, interpreted_config);
    const auto compiled = RunScriptWithConfig(source, compiled_config);

    ASSERT_TRUE(interpreted.built) << label << ": the script did not build without the JIT:\n"
                                   << interpreted.errors;
    ASSERT_TRUE(compiled.built) << label << ": the script did not build with the JIT:\n"
                                << compiled.errors;
    ASSERT_TRUE(interpreted.entry_found) << label << ": no entry point in either build";

    AssertJitEngaged(compiled, label);

    // Both arms, before comparing them. Two runs that both raised nothing would otherwise compare
    // equal and pass, which is exactly the failure this helper exists to catch.
    EXPECT_EQ(interpreted.execution_result, AngelScript::asEXECUTION_EXCEPTION)
        << label << ": the interpreted run did not raise at all";
    EXPECT_EQ(compiled.execution_result, AngelScript::asEXECUTION_EXCEPTION)
        << label << ": the compiled run did not raise at all";

    EXPECT_EQ(interpreted.output, compiled.output)
        << label << ": the JIT printed something different before the raise";
    EXPECT_EQ(interpreted.exception_string, compiled.exception_string)
        << label << ": the two runs raised different exceptions";
  }

} // namespace base::asjit::test
