//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>
#include <asmjit/x86.h>

#include <cstddef>

namespace base::asjit {

  // The register state the VM keeps between instructions, and the one structure the JIT shares with
  // it. Every offset below is taken from the vendored SDK's own definition rather than written out
  // as a number, because a stale constant here does not fail to compile - it writes to the wrong
  // field and corrupts the interpreter's state at some later, unrelated instruction.
  using Registers = AngelScript::asSVMRegisters;

  inline constexpr std::size_t kOffProgramPointer = offsetof(Registers, programPointer);
  inline constexpr std::size_t kOffStackFramePointer = offsetof(Registers, stackFramePointer);
  inline constexpr std::size_t kOffStackPointer = offsetof(Registers, stackPointer);
  inline constexpr std::size_t kOffValueRegister = offsetof(Registers, valueRegister);
  inline constexpr std::size_t kOffObjectRegister = offsetof(Registers, objectRegister);
  inline constexpr std::size_t kOffObjectType = offsetof(Registers, objectType);
  inline constexpr std::size_t kOffDoProcessSuspend = offsetof(Registers, doProcessSuspend);
  inline constexpr std::size_t kOffContext = offsetof(Registers, ctx);

  // `programPointer` heads the struct and `ctx` tails it, so the two together pin both ends of the
  // layout: if a field is ever inserted or reordered, one of these breaks here instead of at run
  // time. The middle fields are checked for ordering, not for exact value, since padding between
  // them is the platform's business.
  static_assert(kOffProgramPointer == 0, "asSVMRegisters no longer starts with programPointer");
  static_assert(kOffStackFramePointer < kOffStackPointer, "frame pointer must precede stack pointer");
  static_assert(kOffStackPointer < kOffValueRegister, "stack pointer must precede value register");
  static_assert(kOffValueRegister < kOffObjectRegister, "value register must precede object register");
  static_assert(kOffObjectRegister < kOffObjectType, "object register must precede object type");
  static_assert(kOffObjectType < kOffDoProcessSuspend, "object type must precede doProcessSuspend");
  static_assert(kOffDoProcessSuspend < kOffContext, "doProcessSuspend must precede ctx");

  // --------------------------------------------------------------------------
  // The native register plan
  // --------------------------------------------------------------------------
  //
  // A compiled function is entered once per asBC_JitEntry site, through the one asJITFunction the
  // script function carries, with `jitArg` selecting the block to resume at:
  //
  //     void native(Registers* regs /* rcx */, asPWORD jitArg /* rdx */);
  //
  // These four registers are what the prologue establishes and the exit epilogue restores. They are
  // all callee-saved, so nothing the VM or a binding does can clobber them across a call.
  //
  // Deliberately *not* in registers: valueRegister, objectRegister and objectType. The interpreter
  // keeps them in `regs` memory and any instruction that falls back has to observe them there, so
  // caching them natively would mean writing them back before every exit anyway. `programPointer`
  // is not held either, and that is the whole win: within a block the next instruction to run is a
  // compile-time constant, so the interpreter's per-instruction dispatch disappears.
  namespace reg {
    // Points at the live asSVMRegisters for this execution.
    inline constexpr auto kRegs = asmjit::x86::rbx;
    // Mirrors regs->stackPointer while inside a block.
    inline constexpr auto kStackPointer = asmjit::x86::r12;
    // Mirrors regs->stackFramePointer while inside a block.
    inline constexpr auto kFramePointer = asmjit::x86::r13;
  }

  // The SysV/MSVC x86-64 argument registers for asJITFunction, named for what they hold here.
  inline constexpr auto kArgRegs = asmjit::x86::rcx;
  inline constexpr auto kArgJitEntry = asmjit::x86::rdx;

  inline constexpr std::size_t kPtrSize = sizeof(void*);
  inline constexpr std::size_t kDwordSize = sizeof(AngelScript::asDWORD);

} // namespace base::asjit
