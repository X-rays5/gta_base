//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <engine_harness.hpp>

#include <string>

namespace base::asjit::test {
  // The engine harness lives in asjit/support, because the benchmark needs the same one and a second
  // definition of "an engine configured the way asjit expects" is a definition that can drift. These
  // re-exports keep the test sources reading as they did.
  using support::CreateEngine;
  using support::EngineConfig;
  using support::RunOutcome;
  using support::RunScript;
  using support::RunScriptWithConfig;

  // Why a comparison is allowed to choose its engine configuration at all.
  //
  // `asBC_ChkRefS` is emitted for a reference to a handle, and the bytecode optimizer rewrites it away
  // in every shape that also runs: `ChkRefS, RDSPtr` becomes `RDSPtr, CHKREF`, and `PGA, RDSPtr`
  // becomes `PshGPtr`. An engine that optimizes - which is the default, and what the menu uses - never
  // hands a ChkRefS to the JIT, so a test that named it while running on the default engine was
  // naming an instruction its script did not contain. Turning the optimizer off is a documented engine
  // setting (`asEP_OPTIMIZE_BYTECODE`) rather than a synthetic arrangement, and the scripts below it
  // are ordinary scripts that run to completion; they are simply compiled without the peephole passes
  // that would have removed the instruction under test.
  //
  // Runs `source` both ways and asserts the outputs match, that both builds succeeded, and that the
  // JIT actually compiled something - the last of which is what stops a test from passing because
  // the JIT silently declined everything.
  //
  // `config` is the engine configuration both arms run under, and its `use_jit` field is ignored: the
  // two arms *are* the two settings of it. The parameter is here for the tests that need an engine
  // other than the default one to reach the instruction they name - see the note below on the
  // optimizer and `asBC_ChkRefS`.
  void ExpectJitMatchesInterpreter(const std::string& source, const std::string& label,
                                   const EngineConfig& config = EngineConfig{});

  // The same, and further asserting that each named opcode was compiled at least once.
  //
  // Without this a test for a new opcode is worth very little: the differential comparison would
  // pass just as happily if the translator did not know the opcode at all and the interpreter ran it
  // in both arms. Naming the opcodes is what makes the test evidence that the translation happened.
  void ExpectJitMatchesInterpreterUsing(const std::string& source, const std::string& label,
                                        std::initializer_list<AngelScript::asEBCInstr> expected_opcodes,
                                        const EngineConfig& config = EngineConfig{});

  // Runs `source` both ways and asserts that *both* ended in a script exception, and that they raised
  // the same one.
  //
  // A guard's failure path cannot be established by comparing outputs alone. A compiled arm that
  // computed the wrong answer would be caught, but one that handed the instruction back, or that
  // raised nothing and then ran off the end of the function, would agree with the interpreter on
  // everything a plain comparison looks at. Asserting the execution result is what makes a test for a
  // guard evidence that the guard fired.
  void ExpectJitRaisesLikeInterpreter(const std::string& source, const std::string& label,
                                      const EngineConfig& config = EngineConfig{});

  // The same, and further asserting that each named opcode was compiled at least once - so that a
  // test for a guard exercises the translated guard rather than the interpreter's own.
  void ExpectJitRaisesLikeInterpreterUsing(const std::string& source, const std::string& label,
                                           std::initializer_list<AngelScript::asEBCInstr> expected_opcodes,
                                           const EngineConfig& config = EngineConfig{});

  // The same, except that the named opcodes are asserted to have been *produced* by the script rather
  // than accepted by the translator.
  //
  // The two are different claims, and only this one is available for an instruction this component
  // refuses on purpose. `Using` exists so that a test for a translation cannot pass because the
  // translation never ran; this exists so that a test for a *refusal* cannot pass because the
  // instruction never appeared - which is how a test of a deliberate gap goes quietly vacuous, and is
  // the failure the refusal's own record would never show.
  void ExpectJitRaisesLikeInterpreterReaching(const std::string& source, const std::string& label,
                                              std::initializer_list<AngelScript::asEBCInstr> expected_opcodes,
                                              const EngineConfig& config = EngineConfig{});

} // namespace base::asjit::test
