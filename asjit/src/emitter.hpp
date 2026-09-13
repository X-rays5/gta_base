//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <asmjit/x86.h>

#include "abi.hpp"
#include "bytecode.hpp"

namespace base::asjit {

  // Translates single bytecode instructions into machine code.
  //
  // Control flow is deliberately absent. Whether a branch becomes a native jump or a return to the
  // interpreter depends on which basic blocks the function compiler managed to cover, and that is not
  // something a one-instruction translator can know. This class answers only "what does this
  // instruction look like on its own", and reports failure for anything outside the translated set so
  // that the caller can end the block *before* it and let the interpreter run it.
  //
  // Every translation below is written to match the interpreter's own handler for the same opcode
  // exactly, including the cases where the interpreter is doing something a faster implementation
  // would not. A JIT that is merely usually equivalent is worse than no JIT, because the divergence
  // shows up as a wrong script result rather than as a crash.
  class Emitter {
  public:
    // The machine registers the prologue established. Only the three the translator needs are named;
    // the rest of the register file is scratch and is used as such.
    struct Abi {
      // Points at the live asSVMRegisters.
      asmjit::x86::Gp regs;
      // Mirrors regs->stackPointer, as a byte address (the interpreter's own is an `asDWORD*`).
      asmjit::x86::Gp sp;
      // Mirrors regs->stackFramePointer, likewise a byte address.
      asmjit::x86::Gp fp;
    };

    Emitter(asmjit::x86::Assembler& assembler, const Abi& abi) noexcept : a_(assembler), abi_(abi) {}

    // The largest `asBC_COPY` that is unrolled into straight-line moves rather than handed to
    // `rep movsb`.
    //
    // The trade is startup against code size. `rep movsb` costs something close to twenty cycles
    // before it moves a byte - on the parts that implement fast short rep, which is to say on
    // everything this JIT is likely to run on - and the unrolled form pays two instructions per eight
    // bytes instead. So the unrolled form wins by a wide margin at the sizes a script actually has,
    // which are the width of a vector or a matrix, and loses once the copy is long enough that the
    // startup is amortised. Sixty-four bytes is four vectors, and it is the point where the two are
    // close enough that the choice stops mattering much either way.
    //
    // Public because that is where a reader of this class will look for it, and because the value
    // type the tests copy to reach the string instructions has to stay on the other side of it: see
    // the `Block` assertion in asjit/support/src/engine_harness.cpp. The two are a pair, so a change
    // to this number has to be checked against that one.
    static constexpr std::int64_t kInlineCopyBytes = 64;

    Emitter(const Emitter&) = delete;
    Emitter& operator=(const Emitter&) = delete;

    // What a translation did.
    enum class Outcome {
      // The opcode is outside the translated set. Nothing has been emitted, so the caller can end the
      // block at exactly this instruction.
      Unsupported,

      // Emitted, unconditionally. Execution continues at the next instruction.
      Translated,

      // Emitted, and the condition flags are left describing the value that was just written to the
      // value register - so an instruction emitted immediately after this one may branch on them
      // instead of loading the register back and comparing it against zero.
      //
      // What qualifies an instruction to report this is that its last flag-setting operation computes
      // the very dword it stores: the flags then say about the stored value exactly what reloading it
      // and testing it against zero would say, which is the whole of what a conditional branch asks.
      // Only the comparisons do this, because each of them ends by materialising the interpreter's
      // -1/0/1 with a `sub`, and the store after that `sub` is of the `sub`'s own result - the `mov`
      // in between is of a register to memory and sets no flags.
      //
      // Nothing else here may report it, and the failure if something did would be quiet: a branch
      // taken on flags describing an operation other than the one it means to test is a wrong answer
      // on one operand pair and a right one on the next.
      //
      // This is a statement about the flags, not about the value register, which is written exactly as
      // it is for `Translated` - so a caller that ignores the outcome, or an instruction compiled after
      // the branch that reads the register, sees no difference between the two.
      TranslatedWithValueFlags,

      // Emitted, but with a route out: the operation has a case that cannot be handled natively -
      // a division by zero, a null dereference - and the emitted code branches to `on_fault` when it
      // happens. The caller must place the exit there, because only the caller knows where the
      // interpreter is to be resumed and what the epilogue looks like.
      //
      // This is what lets the instruction *raise* correctly rather than merely be declined. The
      // handler for a divide-by-zero does not compute anything; it re-enters the engine to report an
      // exception, and reproducing that natively would mean reproducing the exception machinery. So
      // the fast path is compiled and the fault goes back to the one place that already knows how to
      // report it.
      //
      // The guard is emitted before the operation, so the exit lands *out of line* and the fast path
      // pays a single not-taken branch. That is the whole reason the exit is the caller's to place:
      // an emitter that closed the guard itself would have to put the exit inline and jump over it,
      // costing the common case a second branch on every division in the program.
      TranslatedWithFaultRoute,
    };

    // Emits `instr`. `out_fault` is written only for Outcome::TranslatedWithFaultRoute, and only
    // then - a label costs a slot in the code holder whether or not anything branches to it, and
    // most instructions here do not fault, so the label is created by the one branch that needs it
    // rather than by every caller that mostly does not.
    //
    // Control flow opcodes are not translated here and always report Unsupported, including the
    // branches: the caller decides those, because whether one becomes a native jump or a return to
    // the interpreter depends on which blocks were covered, and that is not something an
    // instruction-at-a-time translator can know. There is deliberately no way to ask which opcodes
    // are covered - the switch below is the single list, and a second one maintained beside it would
    // eventually disagree with it.
    [[nodiscard]] Outcome Emit(const Instruction& instr, asmjit::Label* out_fault);

  private:
    // The byte displacement of a script variable from the frame pointer. A variable is named by a
    // signed word counting `asDWORD`s below the frame pointer, so this is that word scaled by four and
    // negated - the same arithmetic the interpreter performs when it indexes its `asDWORD*` frame
    // pointer, done once here instead of at every use.
    [[nodiscard]] static std::int32_t VarDisp(std::int32_t var) noexcept;

    // Typed views of a variable, so that an operand's width is always written down rather than
    // inferred from whatever register happens to be next to it.
    [[nodiscard]] asmjit::x86::Mem Var64(std::int32_t var) const noexcept;
    [[nodiscard]] asmjit::x86::Mem Var32(std::int32_t var) const noexcept;
    [[nodiscard]] asmjit::x86::Mem Var16(std::int32_t var) const noexcept;
    [[nodiscard]] asmjit::x86::Mem Var8(std::int32_t var) const noexcept;

    // The scratch slots the prologue's caller-facing arguments live in. Both are named together so
    // that the one place that depends on the native function's signature stays visible.
    [[nodiscard]] asmjit::x86::Mem ValueRegister32() const noexcept;
    [[nodiscard]] asmjit::x86::Mem ValueRegister64() const noexcept;

    // Branches to a fault exit when `pointer` reads as zero, and hands the exit's label back through
    // `out_fault`. Seven instructions all begin with the same "this pointer must not be null" test and
    // the same `TXT_NULL_POINTER_ACCESS` exception, so the test is written once - the alternative is
    // seven chances to write a zero-test slightly differently at one of them.
    //
    // `*out_fault` is created if it is not a valid label and reused if it is, so an instruction that
    // guards two pointers guards them to one exit. That is not a convenience: a guard that replaced an
    // already-created label would leave the earlier branch pointing at an unbound label, which asmjit
    // encodes as a displacement of zero - a branch to the next instruction, silently doing nothing.
    // `asBC_COPY` has two guards and this is what makes its second one not disable its first.
    void EmitNullGuard(const asmjit::x86::Mem& pointer, asmjit::Label* out_fault);

    // The same for a pointer already in a register. Split from the above rather than taking an operand
    // because the two call for different encodings: a guard on memory folds the load into the
    // comparison, which is one instruction the register form cannot use.
    void EmitNullGuard(const asmjit::x86::Gp& pointer, asmjit::Label* out_fault);

    // Branches to a fault exit when `xmm0` holds an infinity. The exponent operations are the only
    // translations whose failure condition is a property of the *result* rather than of an operand,
    // and this is that condition, written with the pieces x86 has.
    //
    // `is_wide` says which of the two the value is, and it is named for the *double* rather than for
    // the float on purpose: a parameter answering the opposite question - `is_float` - is a call that
    // reads correctly and tests the wrong width, which is a mistake this code made once.
    void EmitInfinityGuard(bool is_wide, asmjit::Label* out_fault);

    // Sets the value register to the sign of a comparison: -1, 0 or 1, exactly as every `asBC_CMP*`
    // handler in the interpreter does.
    //
    // The left operand is a register because it has to be: x86 has no memory-to-memory comparison,
    // and the scripts that compare two variables are the common case. The caller loads it, and the
    // right operand may then be a variable or a constant without the callee having to care which.
    void EmitCompareToValueRegister(const asmjit::x86::Gp& lhs, const asmjit::Operand& rhs, bool is_signed);

    // The float and double form, which cannot share the integer one: the interpreter compares rather
    // than subtracts, so that infinities order correctly and a NaN falls through to "+1". See the
    // definition for how those three outcomes are assembled without branches.
    //
    // The right operand is always a register, which is what the SSE compare instruction takes - the
    // caller loads a variable or a constant into it. This is `xmm1` by convention, and the helper
    // leaves it alone.
    void EmitFloatCompareToValueRegister(const asmjit::x86::Mem& lhs, const asmjit::x86::Vec& rhs, bool is_double);

    asmjit::x86::Assembler& a_;
    Abi abi_;
  };

} // namespace base::asjit
