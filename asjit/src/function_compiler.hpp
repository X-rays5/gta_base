//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>
#include <asmjit/core/jit_runtime.h>

#include <array>
#include <cstdint>

#include "asjit.hpp"

namespace base::asjit {

  // Compiles one script function's bytecode into native code.
  //
  // The unit of translation is the basic block: a run of instructions beginning at an asBC_JitEntry
  // and ending at the first instruction that either cannot be translated or ends the block. Blocks
  // become machine code; anything outside them is left to the interpreter, which is what makes
  // partial coverage correct rather than merely incomplete.
  class FunctionCompiler {
  public:
    struct Result {
      // The native entry point for the function, or null if nothing could be compiled. This is what
      // SetJITFunction has to be handed, and it is the same pointer that has to come back for
      // release.
      AngelScript::asJITFunction entry = nullptr;

      // Basic blocks turned into machine code.
      std::uint64_t blocks_emitted = 0;

      // asBC_JitEntry sites given a block to enter. Zero means the function has no entry points at
      // all - which the engine also treats as "not JIT compiled", having already warned about it.
      std::uint64_t entries_published = 0;

      // Instructions the emitter accepted, and instructions the function has in total. The ratio is
      // the number that explains a speedup: "twice as fast" is a very different claim depending on
      // whether nine tenths of the function was translated or one tenth.
      //
      // Both are static counts over the bytecode, not a record of what executed. The total counts
      // every instruction in the function, including any the compiler emitted but no control flow
      // reaches; the translated count is what ended up inside a block that got native code. An
      // instruction where translation stopped is in neither, which is the point - it and everything
      // after it until the next entry point runs interpreted.
      std::uint64_t instructions_translated = 0;
      std::uint64_t instructions_total = 0;

      // Bytes of machine code emitted. The other resource a JIT spends, and the one that shows up as
      // pressure in a game process rather than as a time.
      std::uint64_t code_size = 0;

      // How often each opcode appears, how often each was translated, and how often the translator
      // was asked for one and had no case for it. Indexed by the opcode byte, so 256 is exact -
      // asEBCInstr does not go higher.
      //
      // This is what turns "the JIT is not helping this script" into a reason. A coverage figure says
      // a fraction of the code was translated; these say *which* instructions were not, and a caller
      // that prints the untranslated ones in descending order of count is looking straight at the
      // answer - it is usually one opcode, and usually a call.
      //
      // The third array is what keeps that answer honest. An opcode missing from `opcodes_translated`
      // was either declined - the emitter has no case for it, which is a gap to close - or it was
      // never asked for, because it lies past the point where translation of its block stopped and no
      // entry point follows before the end of the statement. Both read as "untranslated" and they call
      // for opposite work, so they are counted separately: `opcodes_unimplemented` counts only the
      // first, and is incremented at the one point where the emitter said no.
      //
      // Derived from the emitter by construction rather than from a second list of opcodes: an opcode
      // with a non-zero translated count is one the emitter accepted, because nothing else increments
      // that array, and likewise for the unimplemented one.
      std::array<std::uint64_t, 256> opcodes_walked{};
      std::array<std::uint64_t, 256> opcodes_translated{};
      std::array<std::uint64_t, 256> opcodes_unimplemented{};
    };

    // Emits `func` into `runtime`. On success the block addresses have been written into the
    // function's own asBC_JitEntry pointer arguments, which is how the VM learns where to enter -
    // and the entry point is returned for the caller to publish with SetJITFunction.
    //
    // Nothing is written unless everything succeeded: the bytecode is only modified after the code
    // has been allocated, so a failure part-way through leaves the function exactly as it was and
    // still running correctly in the interpreter.
    //
    // `options` selects the two translations that are a choice rather than a translation - the return
    // and the pointer-returning method call. Both are refused by default; the `Options` comment in
    // `asjit.hpp` says why, and each refusal's own record says what the numbers were.
    [[nodiscard]] static Result Compile(asmjit::JitRuntime& runtime, AngelScript::asIScriptFunction* func,
                                        const Options& options);
  };

} // namespace base::asjit
