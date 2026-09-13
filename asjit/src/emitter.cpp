//
// Created by X-ray on 13/09/2026.
//

#include "emitter.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>

// The interpreter's own integer exponent routines.
//
// These four are declared rather than reimplemented, and that is the whole point of the calls below.
// An integer power is not `pow` narrowed to an integer: it has its own overflow rule, and the rule is
// not the one the arithmetic suggests. `as_powi(2097153, 3)` reports overflow although the product
// fits in the 64 bits it is computed in, because the rule tests the *base* against a precomputed bound
// per exponent rather than testing the product. That bound table (`pow_info`) lives in as_context.cpp
// and is not published, so a translation that guessed it would raise where the interpreter returns a
// value - the one failure mode a JIT cannot have.
//
// Declaring them is what avoids the guess. They are free functions with external linkage in the same
// namespace as the public API, defined in as_context.cpp and compiled into the same `angelscript`
// static library this target already links, so the call resolves at link time. The dependency is on a
// symbol rather than on a header or a class layout: nothing here includes an internal header, and no
// private member is named. The declaration has to match the definition exactly - if a future
// AngelScript changes one of these signatures the link fails loudly, which is the behaviour to want
// from a declaration that cannot be checked against a published header.
//
// This has to sit at global scope, reopening the real namespace, rather than inside the anonymous
// namespace below: a nested `namespace AngelScript` there would declare a *new* namespace that shadows
// the SDK's for the rest of the translation unit, and every `asBC_*` in the switch would stop naming
// the engine's.
namespace AngelScript {
  int as_powi(int base, int exponent, bool& isOverflow);
  asDWORD as_powu(asDWORD base, asDWORD exponent, bool& isOverflow);
  asINT64 as_powi64(asINT64 base, asINT64 exponent, bool& isOverflow);
  asQWORD as_powu64(asQWORD base, asQWORD exponent, bool& isOverflow);
} // namespace AngelScript

namespace base::asjit {
  namespace {
    // The scratch registers the translations below use. None of them is one the prologue keeps state
    // of its own in, so a translation is free to clobber them - and none of them can be live across an
    // instruction boundary, because every translation loads what it needs and stores what it produced.
    //
    // The plan is not "the volatile set", because rdi and rsi are non-volatile here: they carry the
    // string instructions' operands on this platform and the prologue parks them for that reason, so
    // the two are named alongside the volatile ones rather than assumed to come free with them.
    constexpr auto kScratch64 = asmjit::x86::rax;
    constexpr auto kScratch32 = asmjit::x86::eax;
    constexpr auto kScratch2_64 = asmjit::x86::rcx;
    constexpr auto kScratch2_32 = asmjit::x86::ecx;

    // An absolute address as an immediate operand, for the instructions that reach a global or a
    // constant pool entry directly. There is no relocation to apply: the addresses come from the
    // bytecode, which lives for as long as the compiled code it describes.
    [[nodiscard]] asmjit::Imm AddressImm(std::uintptr_t address) noexcept {
      return asmjit::Imm(static_cast<std::uint64_t>(address));
    }

    // Whether the object type an instruction names is one the interpreter never reference-counts.
    //
    // This is the condition that decides which half of the reference-copy handlers runs, and it is
    // read here from the same data the interpreter reads it from: the object type pointer the compiler
    // baked into the instruction. See the reference-copy cases for why that pointer can be treated as
    // an `asITypeInfo*` without an internal header and without assuming a layout.
    //
    // A null pointer answers "no", which is a hand-back rather than a translation. It cannot occur -
    // the interpreter would fault on it too, at `&objType->beh` - but the interpreter would fault
    // while running a script, where this would fault while compiling one, and taking the engine down
    // during a module build is a worse outcome than leaving the instruction to the interpreter.
    [[nodiscard]] bool IsRefcountFreeObjectType(AngelScript::asPWORD raw) noexcept {
      if (raw == 0)
        return false;
      const auto* type = reinterpret_cast<const AngelScript::asITypeInfo*>(raw);
      return (type->GetFlags() & (AngelScript::asOBJ_NOCOUNT | AngelScript::asOBJ_VALUE)) != 0;
    }

    // Unsigned 64-bit to float or double, which x86 cannot do in one instruction - `cvtsi2ss` and
    // `cvtsi2sd` are both signed, and there is no unsigned form of either.
    //
    // The obvious branchless answer - halve the value, convert it, double the result - is wrong for
    // most of the range, and wrong in a way that only shows up at the rounding boundary. Halving
    // discards the low bit, so a value sitting exactly halfway between two representable results is
    // resolved by the halving rather than by the conversion: 2^53 + 1 comes out as 2^53 + 2, where
    // round-to-nearest-even says 2^53. Halving is only safe once the result's ulp is large enough
    // that the discarded bit cannot matter, which is true only above 2^63.
    //
    // So the two ranges are handled separately. Below 2^63 the top bit is clear, the value is a
    // non-negative signed 64-bit, and the signed conversion is directly correct. At or above it, the
    // halve-convert-double form is exact - and there the `| (v & 1)` sticky bit does the job it is
    // meant to, keeping the discarded low bit from turning a round-up into a round-down.
    void EmitUnsignedInt64ToFloatingPoint(asmjit::x86::Assembler& a, const asmjit::x86::Mem& source, bool is_double) {
      asmjit::Label halve = a.new_label();
      asmjit::Label done = a.new_label();

      a.mov(asmjit::x86::rax, source);
      a.test(asmjit::x86::rax, asmjit::x86::rax);
      a.js(halve);

      if (is_double)
        a.cvtsi2sd(asmjit::x86::xmm0, asmjit::x86::rax);
      else
        a.cvtsi2ss(asmjit::x86::xmm0, asmjit::x86::rax);
      a.jmp(done);

      a.bind(halve);
      a.mov(asmjit::x86::rcx, asmjit::x86::rax);
      a.shr(asmjit::x86::rcx, 1);
      a.and_(asmjit::x86::eax, 1);
      a.or_(asmjit::x86::rcx, asmjit::x86::rax);

      if (is_double) {
        a.cvtsi2sd(asmjit::x86::xmm0, asmjit::x86::rcx);
        a.addsd(asmjit::x86::xmm0, asmjit::x86::xmm0);
      } else {
        a.cvtsi2ss(asmjit::x86::xmm0, asmjit::x86::rcx);
        a.addss(asmjit::x86::xmm0, asmjit::x86::xmm0);
      }

      a.bind(done);
    }

    // The low 32 bits of a float, as the integer whose bits they are. Used where a float constant has
    // to go through a general-purpose register to reach an SSE one - `movd` is the only way in, and
    // taking the bits apart by hand is the only way to get them into a form it accepts.
    [[nodiscard]] std::uint32_t FloatBits(float value) noexcept {
      static_assert(sizeof(std::uint32_t) == sizeof(value), "a float is not four bytes");
      std::uint32_t bits = 0;
      std::memcpy(&bits, &value, sizeof(bits));
      return bits;
    }
  } // namespace

  std::int32_t Emitter::VarDisp(std::int32_t var) noexcept {
    return -var * static_cast<std::int32_t>(sizeof(AngelScript::asDWORD));
  }

  asmjit::x86::Mem Emitter::Var64(std::int32_t var) const noexcept {
    return asmjit::x86::qword_ptr(abi_.fp, VarDisp(var));
  }

  asmjit::x86::Mem Emitter::Var32(std::int32_t var) const noexcept {
    return asmjit::x86::dword_ptr(abi_.fp, VarDisp(var));
  }

  asmjit::x86::Mem Emitter::Var16(std::int32_t var) const noexcept {
    return asmjit::x86::word_ptr(abi_.fp, VarDisp(var));
  }

  asmjit::x86::Mem Emitter::Var8(std::int32_t var) const noexcept {
    return asmjit::x86::byte_ptr(abi_.fp, VarDisp(var));
  }

  asmjit::x86::Mem Emitter::ValueRegister32() const noexcept {
    return asmjit::x86::dword_ptr(abi_.regs, static_cast<std::int32_t>(kOffValueRegister));
  }

  asmjit::x86::Mem Emitter::ValueRegister64() const noexcept {
    return asmjit::x86::qword_ptr(abi_.regs, static_cast<std::int32_t>(kOffValueRegister));
  }

  void Emitter::EmitCompareToValueRegister(const asmjit::x86::Gp& lhs, const asmjit::Operand& rhs, bool is_signed) {
    // The interpreter's comparisons do not produce a carry or a flags word - they produce -1, 0 or 1
    // in the value register, and the conditional jumps that follow test exactly that. So the three
    // outcomes are materialised here rather than left as flags: the two setcc results are the "-1" and
    // the "+1", and neither being set is the zero.
    //
    // Through the instruction-id form rather than the typed one. asmjit's typed `cmp` overloads are
    // (Gp, Gp), (Gp, Mem), (Mem, Gp) and (Mem, Imm) - there is no single one that covers both a
    // compared variable and a compared constant, and the instruction-id form picks the encoding from
    // the operands exactly as those do.
    static_cast<void>(a_.emit(asmjit::x86::Inst::kIdCmp, lhs, rhs));

    if (is_signed) {
      a_.setg(asmjit::x86::cl);
      a_.setl(asmjit::x86::al);
    } else {
      a_.seta(asmjit::x86::cl);
      a_.setb(asmjit::x86::al);
    }

    // Both `movzx`s have to come after both `setcc`s: each setcc writes only a byte, and reading one
    // back before the other is written would fold whatever the previous use of the register left.
    a_.movzx(kScratch2_32, asmjit::x86::cl);
    a_.movzx(kScratch32, asmjit::x86::al);
    a_.sub(kScratch2_32, kScratch32);

    a_.mov(ValueRegister32(), kScratch2_32);
  }

  void Emitter::EmitFloatCompareToValueRegister(const asmjit::x86::Mem& lhs, const asmjit::x86::Vec& rhs, bool is_double) {
    // The interpreter compares floats with `<` rather than by subtracting, so that infinities order
    // correctly - and so that an unordered comparison (a NaN on either side) falls through to the
    // "+1" branch rather than reading as equal. `cmpps`/`cmpsd` reproduce both properties: the
    // ordered-equal and ordered-less predicates are false for NaN, which is exactly want we want.
    //
    // The result is assembled arithmetically rather than with branches, since a script comparison
    // inside a loop is the case worth compiling in the first place:
    //
    //     result = (not (equal or less)) - less
    //
    // which is -1 when less, 0 when equal, 1 when greater, and 1 when unordered.
    if (is_double) {
      a_.movsd(asmjit::x86::xmm0, lhs);
      a_.movaps(asmjit::x86::xmm2, asmjit::x86::xmm0);
      a_.cmpsd(asmjit::x86::xmm2, rhs, 0); // ordered equal
      a_.cmpsd(asmjit::x86::xmm0, rhs, 1); // ordered less-than
    } else {
      a_.movss(asmjit::x86::xmm0, lhs);
      a_.movaps(asmjit::x86::xmm2, asmjit::x86::xmm0);
      a_.cmpss(asmjit::x86::xmm2, rhs, 0);
      a_.cmpss(asmjit::x86::xmm0, rhs, 1);
    }

    a_.movd(kScratch32, asmjit::x86::xmm2);
    a_.movd(asmjit::x86::edx, asmjit::x86::xmm0);

    // Each predicate produced a full mask, so masking off one bit turns it into the 0/1 the
    // arithmetic above needs.
    a_.and_(kScratch32, 1);
    a_.and_(asmjit::x86::edx, 1);

    a_.or_(kScratch32, asmjit::x86::edx);
    a_.xor_(kScratch32, 1);
    a_.sub(kScratch32, asmjit::x86::edx);

    a_.mov(ValueRegister32(), kScratch32);
  }

  // The label is created on the *first* guard of an instruction and reused by any that follow, which is
  // what makes two guards share one exit.
  //
  // Unconditionally creating one here is a bug, and one that is silent in a way worth spelling out.
  // asmjit writes a displacement of zero for a label it has not bound yet and patches it when the label
  // is bound ("Emit dummy zeros, must be patched later", x86_assembler.cpp) - so a second guard that
  // overwrote the label left the first guard's branch encoded as `jcc +0`, which is a jump to the next
  // instruction: the check quietly did nothing. It does not fail to assemble, it does not leave an
  // unresolved fixup for anything to notice, and the fast path never takes the branch, so nothing
  // observes it until the fault it was supposed to catch happens. `asBC_COPY` is the only instruction
  // with two guards, and its destination-pointer check was inert for exactly this reason.
  void Emitter::EmitNullGuard(const asmjit::x86::Mem& pointer, asmjit::Label* out_fault) {
    if (!out_fault->is_valid())
      *out_fault = a_.new_label();
    a_.cmp(pointer, 0);
    a_.je(*out_fault);
  }

  void Emitter::EmitNullGuard(const asmjit::x86::Gp& pointer, asmjit::Label* out_fault) {
    if (!out_fault->is_valid())
      *out_fault = a_.new_label();
    a_.test(pointer, pointer);
    a_.je(*out_fault);
  }

  // The interpreter's overflow test for the exponent operations is `r == HUGE_VAL || isinf(r)`, and
  // the first term is already inside the second - HUGE_VAL is an infinity - so the test is `isinf(r)`
  // and nothing more. `isinf` is a macro over a comparison on this platform rather than a call, but
  // spelling it out here is what keeps that true: what it compiles to is this test, and reading it
  // as `isinf` in the emitted code would be reading it as whatever the CRT decides today.
  //
  // An infinity is a floating point number whose exponent field is all ones and whose mantissa is
  // zero. Shifting the whole word left by one drops the sign, which collapses +inf and -inf onto the
  // same value without touching any other number: a finite value's shifted exponent is below the all
  // ones pattern, and a NaN's shifted mantissa is not zero. So one shift turns a signed test into an
  // equality against a single constant.
  void Emitter::EmitInfinityGuard(bool is_wide, asmjit::Label* out_fault) {
    // Created on first use for the same reason the two above are, rather than because any instruction
    // reaches this twice today.
    if (!out_fault->is_valid())
      *out_fault = a_.new_label();

    if (!is_wide) {
      // 0x7F800000 is +inf and 0xFF800000 is -inf; both shift to 0xFF000000. The constant is written
      // as the signed 32-bit it encodes to, which is the form `cmp` takes without the assembler
      // having to decide whether an unsigned one is meant to sign-extend.
      a_.movd(kScratch2_32, asmjit::x86::xmm0);
      a_.add(kScratch2_32, kScratch2_32);
      a_.cmp(kScratch2_32, asmjit::Imm(static_cast<std::int32_t>(0xFF000000u)));
      a_.je(*out_fault);
      return;
    }

    // The wide form needs its constant in a register: `cmp` takes a sign-extended 32-bit immediate
    // and 0xFFE0000000000000 does not fit one.
    a_.movq(kScratch2_64, asmjit::x86::xmm0);
    a_.add(kScratch2_64, kScratch2_64);
    a_.mov(kScratch64, asmjit::Imm(0xFFE0000000000000ull));
    a_.cmp(kScratch2_64, kScratch64);
    a_.je(*out_fault);
  }

  Emitter::Outcome Emitter::Emit(const Instruction& ins, asmjit::Label* out_fault) {
    switch (ins.opcode()) {
      // ----------------------------------------------------------------------
      // The JIT's own entry marker
      // ----------------------------------------------------------------------
    case AngelScript::asBC_JitEntry:
      // A no-op once execution is already inside compiled code. The instruction exists so the
      // interpreter has somewhere to hand off from, and everything the handler does - save the
      // registers, call the compiled function, reload them - is what entering this code already did.
      // The one effect worth keeping is on the program pointer, and that is set by whichever exit
      // this block eventually takes.
      //
      // Reaching one here means a branch targets a JitEntry rather than its successor; the successor
      // is declared a block start of its own, so the block ends at the next instruction either way.
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Stack and pointer movement
      // ----------------------------------------------------------------------
      //
      // `sp` is a byte address, matching `regs->stackPointer`, so a dword push moves it four bytes
      // and a pointer push eight - not the one and two the interpreter's `asDWORD*` arithmetic moves
      // it by, which is the same distance spelled differently.
    case AngelScript::asBC_PopPtr:
      a_.add(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      return Outcome::Translated;

    case AngelScript::asBC_PshNull:
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), 0);
      return Outcome::Translated;

    case AngelScript::asBC_PopRPtr:
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.sp));
      a_.mov(ValueRegister64(), kScratch64);
      a_.add(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      return Outcome::Translated;

    case AngelScript::asBC_PshRPtr:
      a_.mov(kScratch64, ValueRegister64());
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_SwapPtr:
      // Translated. Its one site is as_compiler.cpp:17359, in the `DoesReturnOnStack() && !useVariable`
      // branch of a call - the comment there says "Sometimes the value types are allocated on the
      // heap", and that is the whole of the requirement: a value type the compiler decided to allocate
      // rather than place on the stack. The call then arrives with the result buffer's address pushed
      // after the object's, and the two have to be exchanged because the callee reads them
      // positionally.
      //
      // What decides whether this is reached is `asEP_OPTIMIZE_BYTECODE`, and the mechanism is narrower
      // than "the optimizer removes it": `asCByteCode::CanBeSwapped` (as_bytecode.cpp:305) folds the
      // exchange away only when the two instructions under it are `PshNull`, `PshVPtr` or `PSF` - that
      // is, when both addresses are statically known and the exchange can be done at compile time. A
      // shape whose two predecessors are anything else keeps the instruction, and gets this case.
      //
      // The case was written when this was believed to be unreachable, on the reasoning that a handle
      // to a value type is refused by `asCDataType::MakeHandle` and the heap allocation therefore
      // never happens. That reasoning is still correct about handles; it was the wrong requirement. The
      // instruction is reached with the optimizer off, by `Differential.ValueConversionExchangeIsTranslated`
      // and by the benchmark's `unoptimized` row, and both compare it against the interpreter. The
      // earlier note here said nothing had ever produced it; that is recorded rather than deleted
      // because the way it was wrong - an "unreachable" finding is a finding about a *configuration* -
      // is the thing worth remembering.
      // The two loads, the two stores, and nothing else: the exchange needs both words resident at
      // once, which is the whole reason it takes two scratch registers rather than one.
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.sp));
      a_.mov(kScratch2_64, asmjit::x86::qword_ptr(abi_.sp, static_cast<std::int32_t>(kPtrSize)));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch2_64);
      a_.mov(asmjit::x86::qword_ptr(abi_.sp, static_cast<std::int32_t>(kPtrSize)), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_PshV4:
      a_.mov(kScratch32, Var32(ins.var0()));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kDwordSize));
      a_.mov(asmjit::x86::dword_ptr(abi_.sp), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_PshV8:
      a_.mov(kScratch64, Var64(ins.var0()));
      a_.sub(abi_.sp, static_cast<std::int32_t>(2 * kDwordSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_PshC4:
    case AngelScript::asBC_TYPEID:
      a_.sub(abi_.sp, static_cast<std::int32_t>(kDwordSize));
      a_.mov(asmjit::x86::dword_ptr(abi_.sp), ins.dword());
      return Outcome::Translated;

    case AngelScript::asBC_PshC8:
      // Translated, and emitted by nothing: walking every `InstrQW` and `InstrSHORT64` site in
      // as_compiler.cpp finds no producer of this opcode. The nearest thing the compiler does with an
      // eight-byte immediate is `asBC_PshC4` twice, or a load from the constant pool string table,
      // which is why a double literal in the tests arrives as a `PshC4` pair. Answered rather than
      // left open so the next person counting the translated column does not spend the probe again.
      a_.mov(kScratch64, ins.qword());
      a_.sub(abi_.sp, static_cast<std::int32_t>(2 * kDwordSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_PshVPtr:
      a_.mov(kScratch64, Var64(ins.var0()));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_PSF:
      // Pushes the *address* of the variable, not its contents.
      a_.lea(kScratch64, Var32(ins.var0()));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_ClrVPtr:
      a_.mov(Var64(ins.var0()), 0);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Globals and constant pool entries
      // ----------------------------------------------------------------------
    case AngelScript::asBC_PGA:
    case AngelScript::asBC_FuncPtr:
    case AngelScript::asBC_OBJTYPE:
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_PshGPtr:
      // The contents of the address, not the address - the interpreter's own comment calls this the
      // fused form of PGA followed by RDSPtr.
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(kScratch64, asmjit::x86::qword_ptr(kScratch64));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_PshG4:
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(kScratch2_32, asmjit::x86::dword_ptr(kScratch64));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kDwordSize));
      a_.mov(asmjit::x86::dword_ptr(abi_.sp), kScratch2_32);
      return Outcome::Translated;

    case AngelScript::asBC_SetG4:
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(asmjit::x86::dword_ptr(kScratch64), ins.dwordAfterPointer());
      return Outcome::Translated;

    case AngelScript::asBC_CpyVtoG4:
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(kScratch2_32, Var32(ins.var0()));
      a_.mov(asmjit::x86::dword_ptr(kScratch64), kScratch2_32);
      return Outcome::Translated;

    case AngelScript::asBC_CpyGtoV4:
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(kScratch2_32, asmjit::x86::dword_ptr(kScratch64));
      a_.mov(Var32(ins.var0()), kScratch2_32);
      return Outcome::Translated;

    case AngelScript::asBC_LDG:
      // Loads the address itself into the value register, without dereferencing it.
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_LDV:
      a_.lea(kScratch64, Var32(ins.var0()));
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_LdGRdR4:
      a_.mov(kScratch64, AddressImm(ins.pointer()));
      a_.mov(ValueRegister64(), kScratch64);
      a_.mov(kScratch2_32, asmjit::x86::dword_ptr(kScratch64));
      a_.mov(Var32(ins.var0()), kScratch2_32);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Moves between variables and the value register
      // ----------------------------------------------------------------------
    case AngelScript::asBC_SetV4:
    case AngelScript::asBC_SetV1:
    case AngelScript::asBC_SetV2:
      a_.mov(Var32(ins.var0()), ins.dword());
      return Outcome::Translated;

    case AngelScript::asBC_SetV8:
      a_.mov(kScratch64, ins.qword());
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_CpyVtoV4:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_CpyVtoV8:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_CpyVtoR4:
      a_.mov(kScratch32, Var32(ins.var0()));
      a_.mov(ValueRegister32(), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_CpyVtoR8:
      a_.mov(kScratch64, Var64(ins.var0()));
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_CpyRtoV4:
      a_.mov(kScratch32, ValueRegister32());
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_CpyRtoV8:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Boolean tests against zero
      // ----------------------------------------------------------------------
      //
      // These three are the ones where the interpreter is most visibly doing something other than
      // what a hand-written translation would do, and each does something different from the other
      // two, so all three are reproduced rather than unified.
    case AngelScript::asBC_NOT: {
      // Tests only the low *byte*, then clears the rest of the dword. Clearing it matters: the value
      // register would otherwise keep a non-zero bit above the result.
      a_.cmp(Var8(ins.var0()), 0);
      a_.sete(asmjit::x86::al);
      a_.movzx(kScratch32, asmjit::x86::al);
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;
    }

    case AngelScript::asBC_TZ:
      // Tests the low *dword*, then clears all eight bytes of the register.
      a_.cmp(ValueRegister32(), 0);
      a_.sete(asmjit::x86::al);
      a_.movzx(kScratch32, asmjit::x86::al);
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_TNZ:
      a_.cmp(ValueRegister32(), 0);
      a_.setne(asmjit::x86::al);
      a_.movzx(kScratch32, asmjit::x86::al);
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_TS:
    case AngelScript::asBC_TNS:
    case AngelScript::asBC_TP:
    case AngelScript::asBC_TNP: {
      // The same test-and-set shape as TZ, with the comparison swapped, and the same full-width
      // write afterwards.
      a_.cmp(ValueRegister32(), 0);
      switch (ins.opcode()) {
      case AngelScript::asBC_TS:
        a_.setl(asmjit::x86::al);
        break;
      case AngelScript::asBC_TNS:
        a_.setge(asmjit::x86::al);
        break;
      case AngelScript::asBC_TP:
        a_.setg(asmjit::x86::al);
        break;
      default:
        a_.setle(asmjit::x86::al);
        break;
      }
      a_.movzx(kScratch32, asmjit::x86::al);
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;
    }

    case AngelScript::asBC_ClrHi:
      // Clears the three bytes above the low one, leaving the upper half of the register alone -
      // unlike TZ, which clears the whole thing.
      a_.movzx(kScratch32, asmjit::x86::byte_ptr(abi_.regs, static_cast<std::int32_t>(kOffValueRegister)));
      a_.mov(ValueRegister32(), kScratch32);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Negate, increment, decrement
      // ----------------------------------------------------------------------
    case AngelScript::asBC_NEGi:
      a_.mov(kScratch32, Var32(ins.var0()));
      a_.neg(kScratch32);
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_NEGf:
      // A float negation is a flip of the sign bit, which is exact for zero and for NaN - where an
      // arithmetic negation could quiet a signalling NaN or lose the sign of a zero.
      a_.mov(kScratch32, Var32(ins.var0()));
      a_.xor_(kScratch32, static_cast<std::uint32_t>(0x80000000u));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_NEGd:
      a_.mov(kScratch64, Var64(ins.var0()));
      a_.mov(kScratch2_64, static_cast<std::uint64_t>(0x8000000000000000ull));
      a_.xor_(kScratch64, kScratch2_64);
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_IncVi:
      a_.add(Var32(ins.var0()), 1);
      return Outcome::Translated;

    case AngelScript::asBC_DecVi:
      a_.sub(Var32(ins.var0()), 1);
      return Outcome::Translated;

    case AngelScript::asBC_BNOT:
      a_.not_(Var32(ins.var0()));
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Bitwise
      // ----------------------------------------------------------------------
    case AngelScript::asBC_BAND:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.and_(kScratch32, Var32(ins.var2()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_BOR:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.or_(kScratch32, Var32(ins.var2()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_BXOR:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.xor_(kScratch32, Var32(ins.var2()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_BSLL:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.mov(kScratch2_32, Var32(ins.var2()));
      a_.shl(kScratch32, asmjit::x86::cl);
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_BSRL:
      // The operand is an unsigned `asDWORD` in the interpreter, so this is a logical shift.
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.mov(kScratch2_32, Var32(ins.var2()));
      a_.shr(kScratch32, asmjit::x86::cl);
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_BSRA:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.mov(kScratch2_32, Var32(ins.var2()));
      a_.sar(kScratch32, asmjit::x86::cl);
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Integer arithmetic
      // ----------------------------------------------------------------------
    case AngelScript::asBC_ADDi:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.add(kScratch32, Var32(ins.var2()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_SUBi:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.sub(kScratch32, Var32(ins.var2()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_MULi:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.imul(kScratch32, Var32(ins.var2()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Division and remainder
      // ----------------------------------------------------------------------
      //
      // The only instructions here whose arithmetic can fail in a way the machine code cannot express.
      // `idiv` on a zero divisor raises a hardware divide error, which on Windows arrives as a
      // structured exception the engine has no handler for - so the condition is tested before the
      // instruction rather than discovered by it, and the exit goes back to the interpreter, which is
      // where the engine's own `TXT_DIVIDE_BY_ZERO` machinery lives.
      //
      // The signed forms carry a second case that the hardware does not report at all. The most
      // negative value divided by -1 has no representable result, and `idiv` signals it only as the
      // same divide error; the interpreter raises a *different* message (`TXT_DIVIDE_OVERFLOW`) for
      // it, so the two conditions have to be told apart here and the guard cannot simply be "divisor
      // is zero". Remainder carries the same two cases because the interpreter checks them for `%`
      // too, even though the hardware computes `INT_MIN % -1` as a perfectly ordinary zero.
      //
      // Both guards are emitted before anything is stored, which is what lets the exit re-run the
      // whole instruction interpreted rather than merely abandon it.
    case AngelScript::asBC_DIVi:
    case AngelScript::asBC_MODi:
    case AngelScript::asBC_DIVi64:
    case AngelScript::asBC_MODi64: {
      const bool wide = ins.opcode() == AngelScript::asBC_DIVi64 || ins.opcode() == AngelScript::asBC_MODi64;
      const bool remainder = ins.opcode() == AngelScript::asBC_MODi || ins.opcode() == AngelScript::asBC_MODi64;

      const auto divisor = wide ? Var64(ins.var2()) : Var32(ins.var2());
      const auto dividend = wide ? Var64(ins.var1()) : Var32(ins.var1());
      const auto quotient = wide ? Var64(ins.var0()) : Var32(ins.var0());

      EmitNullGuard(divisor, out_fault);

      // -1 is the only divisor with a value it cannot divide, and only one dividend reaches it.
      asmjit::Label divide = a_.new_label();
      a_.cmp(divisor, -1);
      a_.jne(divide);
      if (wide)
        a_.cmp(dividend, asmjit::Imm(std::numeric_limits<std::int64_t>::min()));
      else
        a_.cmp(dividend, asmjit::Imm(static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::min())));
      a_.je(*out_fault);
      a_.bind(divide);

      // `idiv` divides the double-width value in rdx:rax, so the dividend has to be sign-extended into
      // that pair first - `cqo` for the wide form, `cdq` for the narrow one. Writing it any other way
      // leaves the high half holding whatever the last translation left there, and the quotient of
      // every division in the function is wrong by a multiple of 2^32 that looks like noise.
      //
      // The narrow form loads the 32-bit register rather than the 64-bit one even though `cdq` reads
      // eax either way: `mov rax, dword_ptr` is a width mismatch asmjit resolves in the register's
      // favour, which turns a four-byte read into an eight-byte one.
      a_.mov(wide ? kScratch64 : kScratch32, dividend);
      if (wide) {
        a_.cqo();
        a_.idiv(divisor);
        a_.mov(quotient, remainder ? asmjit::x86::rdx : kScratch64);
      } else {
        a_.cdq();
        a_.idiv(divisor);
        a_.mov(quotient, remainder ? asmjit::x86::edx : kScratch32);
      }
      return Outcome::TranslatedWithFaultRoute;
    }

    case AngelScript::asBC_DIVu:
    case AngelScript::asBC_MODu:
    case AngelScript::asBC_DIVu64:
    case AngelScript::asBC_MODu64: {
      // The unsigned forms have no overflow case - every bit pattern has a quotient - so only the zero
      // divisor can fail. The interpreter's `-1` check is absent here for the same reason.
      const bool wide = ins.opcode() == AngelScript::asBC_DIVu64 || ins.opcode() == AngelScript::asBC_MODu64;
      const bool remainder = ins.opcode() == AngelScript::asBC_MODu || ins.opcode() == AngelScript::asBC_MODu64;

      const auto divisor = wide ? Var64(ins.var2()) : Var32(ins.var2());
      const auto dividend = wide ? Var64(ins.var1()) : Var32(ins.var1());
      const auto quotient = wide ? Var64(ins.var0()) : Var32(ins.var0());

      EmitNullGuard(divisor, out_fault);

      // `div` takes the high half of the dividend from rdx, so it is cleared rather than sign-extended
      // - `cqo` here would make every dividend above 2^31 a negative number of the double width. The
      // load is 32-bit for the narrow form, same as above.
      a_.mov(wide ? kScratch64 : kScratch32, dividend);
      a_.xor_(asmjit::x86::edx, asmjit::x86::edx);
      a_.div(divisor);
      a_.mov(quotient, remainder ? (wide ? asmjit::x86::rdx : asmjit::x86::edx) : (wide ? kScratch64 : kScratch32));
      return Outcome::TranslatedWithFaultRoute;
    }

    case AngelScript::asBC_ADDIi:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.add(kScratch32, ins.integerAfterWords());
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_SUBIi:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.sub(kScratch32, ins.integerAfterWords());
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_MULIi:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.imul(kScratch32, ins.integerAfterWords());
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Floating point arithmetic
      // ----------------------------------------------------------------------
    case AngelScript::asBC_ADDf:
      a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.addss(asmjit::x86::xmm0, Var32(ins.var2()));
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_SUBf:
      a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.subss(asmjit::x86::xmm0, Var32(ins.var2()));
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_MULf:
      a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.mulss(asmjit::x86::xmm0, Var32(ins.var2()));
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_ADDd:
      a_.movsd(asmjit::x86::xmm0, Var64(ins.var1()));
      a_.addsd(asmjit::x86::xmm0, Var64(ins.var2()));
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_SUBd:
      a_.movsd(asmjit::x86::xmm0, Var64(ins.var1()));
      a_.subsd(asmjit::x86::xmm0, Var64(ins.var2()));
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_MULd:
      a_.movsd(asmjit::x86::xmm0, Var64(ins.var1()));
      a_.mulsd(asmjit::x86::xmm0, Var64(ins.var2()));
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

      // The floating point divisions and remainders. Only a zero divisor can fail, but "zero" here is
      // a floating point comparison and the test has to be built the way the interpreter's `== 0` is:
      // true for -0.0, false for NaN.
      //
      // That is not what `ucomiss` followed by a branch gives on its own. The unordered result - a NaN
      // on either side - sets ZF as well as PF, so a plain `je` would read a NaN divisor as zero and
      // raise for a division the interpreter performs happily. The parity flag is what separates them,
      // and it has to be tested first because the `je` would otherwise have already taken the branch.
      //
      // The remainder forms have no instruction at all. `fmod` is not something SSE computes - the
      // exact result needs the dividend reduced by a multiple of the divisor over the whole exponent
      // range - so they call the same `fmod` the interpreter calls and get the same answer by
      // construction. That is a real call, so it needs the shadow space the Win64 convention requires
      // the caller to provide: without it the callee writes its first four argument slots over the
      // three registers the prologue parks, and the function returns to a caller whose frame pointer
      // is a floating point remainder.
    case AngelScript::asBC_DIVf:
    case AngelScript::asBC_MODf:
    case AngelScript::asBC_DIVd:
    case AngelScript::asBC_MODd: {
      const bool wide = ins.opcode() == AngelScript::asBC_DIVd || ins.opcode() == AngelScript::asBC_MODd;
      const bool remainder = ins.opcode() == AngelScript::asBC_MODf || ins.opcode() == AngelScript::asBC_MODd;

      const auto divisor = wide ? Var64(ins.var2()) : Var32(ins.var2());
      const auto dividend = wide ? Var64(ins.var1()) : Var32(ins.var1());
      const auto quotient = wide ? Var64(ins.var0()) : Var32(ins.var0());

      *out_fault = a_.new_label();
      asmjit::Label ordered = a_.new_label();
      if (wide) {
        a_.xorpd(asmjit::x86::xmm1, asmjit::x86::xmm1);
        a_.ucomisd(asmjit::x86::xmm1, divisor);
      } else {
        a_.xorps(asmjit::x86::xmm1, asmjit::x86::xmm1);
        a_.ucomiss(asmjit::x86::xmm1, divisor);
      }
      // The zero is on the *left* so that the comparison is "zero ? divisor" rather than "divisor ?
      // zero" - `ucomiss` is not symmetric, and it is the left operand's NaNs that produce the
      // unordered result. A NaN divisor has to reach `ordered`, which is exactly what the interpreter's
      // `divider == 0` says by being false.
      a_.jp(ordered);
      a_.je(*out_fault);
      a_.bind(ordered);

      if (wide) {
        a_.movsd(asmjit::x86::xmm0, dividend);
        if (remainder) {
          a_.movsd(asmjit::x86::xmm1, divisor);
          a_.sub(asmjit::x86::rsp, 32);
          a_.mov(kScratch64, asmjit::Imm(reinterpret_cast<std::uint64_t>(static_cast<double (*)(double, double)>(&std::fmod))));
          a_.call(kScratch64);
          a_.add(asmjit::x86::rsp, 32);
        } else {
          a_.divsd(asmjit::x86::xmm0, divisor);
        }
        a_.movsd(quotient, asmjit::x86::xmm0);
      } else {
        a_.movss(asmjit::x86::xmm0, dividend);
        if (remainder) {
          a_.movss(asmjit::x86::xmm1, divisor);
          a_.sub(asmjit::x86::rsp, 32);
          a_.mov(kScratch64, asmjit::Imm(reinterpret_cast<std::uint64_t>(static_cast<float (*)(float, float)>(&std::fmodf))));
          a_.call(kScratch64);
          a_.add(asmjit::x86::rsp, 32);
        } else {
          a_.divss(asmjit::x86::xmm0, divisor);
        }
        a_.movss(quotient, asmjit::x86::xmm0);
      }
      return Outcome::TranslatedWithFaultRoute;
    }

      // The exponent operations, floating point only.
      //
      // SSE has no instruction for a general power, so these are the CRT's `pow` and `powf` - the same
      // two routines the interpreter's own handlers call. That is the point rather than a shortcut: a
      // hand-written `exp(y * log(x))` would be a different function, differing in the last place for
      // most inputs and disagreeing outright about negative bases, and the whole reason to compile an
      // instruction is to get the same answer sooner.
      //
      // `POWdi` is the one mixed form the compiler emits - a double base with an `int` exponent, which
      // is what `double ** int` produces - and it widens the exponent here rather than in a second
      // call, because `pow` takes two doubles and there is no second routine to call. The compiler
      // never emits the mirror image, `int ** double`, because the instruction is chosen by the right
      // operand's type.
      //
      // The integer exponent forms carry the same public shape and a different failure condition, and
      // they are translated - see the block below this one for how, and for why the rule they have to
      // reproduce is why they call the interpreter's own routine rather than a reimplementation.
      case AngelScript::asBC_POWf:
      case AngelScript::asBC_POWd:
      case AngelScript::asBC_POWdi: {
        const bool wide = ins.opcode() != AngelScript::asBC_POWf;

        if (wide) {
          a_.movsd(asmjit::x86::xmm0, Var64(ins.var1()));
          if (ins.opcode() == AngelScript::asBC_POWdi)
            a_.cvtsi2sd(asmjit::x86::xmm1, Var32(ins.var2()));
          else
            a_.movsd(asmjit::x86::xmm1, Var64(ins.var2()));
        } else {
          a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
          a_.movss(asmjit::x86::xmm1, Var32(ins.var2()));
        }

        // The routine is chosen by width, and the choice is made here rather than at the call because
        // the two overloads are different types - `pow` and `powf` share no pointer type a conditional
        // could settle on.
        const std::uint64_t routine = wide
                                        ? reinterpret_cast<std::uint64_t>(static_cast<double (*)(double, double)>(&std::pow))
                                        : reinterpret_cast<std::uint64_t>(static_cast<float (*)(float, float)>(&std::powf));

        // The shadow space, for the reason the remainders above give: the callee writes its first four
        // argument slots below the return address whether or not it has four arguments.
        a_.sub(asmjit::x86::rsp, 32);
        a_.mov(kScratch64, asmjit::Imm(routine));
        a_.call(kScratch64);
        a_.add(asmjit::x86::rsp, 32);

        // Before the store, per the fault route's rule. The interpreter stores and then tests, so it
        // is the store that this moves rather than an effect it omits - and on the fault path the
        // interpreter re-runs the instruction and stores the same infinity itself.
        EmitInfinityGuard(wide, out_fault);

        if (wide)
          a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
        else
          a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
        return Outcome::TranslatedWithFaultRoute;
      }

    // The integer exponent operations.
    //
    // Four opcodes and one shape: two variables in, a routine call, a result out, and a flag that says
    // whether the interpreter would have raised instead. The flag is the whole reason this is a call
    // and not a loop - see the declaration of `as_powi` above for what the rule it reports is and why
    // reproducing it here would be a guess.
    //
    // The overflow case is not handled here at all. It branches to the fault exit, which publishes
    // *this* instruction's offset, so the interpreter re-runs it: it calls the same routine, gets the
    // same flag, stores the zero this translation deliberately does not store, and raises
    // `TXT_POW_OVERFLOW`. That is the same division of labour the floating forms above use for an
    // infinity, and for the same reason - the exception machinery is the interpreter's and there is
    // nothing to gain by having a second copy of it.
    //
    // The order of the two operand loads is fixed by the register the routine's address goes in: rax
    // is loaded last, after rcx, rdx and r8 have been filled from the frame it is about to leave.
    case AngelScript::asBC_POWi:
    case AngelScript::asBC_POWu:
    case AngelScript::asBC_POWi64:
    case AngelScript::asBC_POWu64: {
      const bool wide =
          ins.opcode() == AngelScript::asBC_POWi64 || ins.opcode() == AngelScript::asBC_POWu64;

      std::uint64_t routine = 0;
      switch (ins.opcode()) {
      case AngelScript::asBC_POWi:
        routine = reinterpret_cast<std::uint64_t>(&AngelScript::as_powi);
        break;
      case AngelScript::asBC_POWu:
        routine = reinterpret_cast<std::uint64_t>(&AngelScript::as_powu);
        break;
      case AngelScript::asBC_POWi64:
        routine = reinterpret_cast<std::uint64_t>(&AngelScript::as_powi64);
        break;
      default:
        routine = reinterpret_cast<std::uint64_t>(&AngelScript::as_powu64);
        break;
      }

      // Shadow space, for the reason the floating forms above give.
      a_.sub(asmjit::x86::rsp, 32);

      // The out-parameter. The interpreter hands this routine an uninitialised stack local; it is
      // zeroed here instead, which is the value every path through the routine leaves behind anyway
      // and the only way to keep the read below defined if a future version stops assigning it.
      a_.mov(asmjit::x86::byte_ptr(asmjit::x86::rsp), 0);
      a_.lea(asmjit::x86::r8, asmjit::x86::byte_ptr(asmjit::x86::rsp));

      if (wide) {
        a_.mov(kScratch2_64, Var64(ins.var1()));
        a_.mov(asmjit::x86::rdx, Var64(ins.var2()));
      } else {
        a_.mov(kScratch2_32, Var32(ins.var1()));
        a_.mov(asmjit::x86::edx, Var32(ins.var2()));
      }

      a_.mov(kScratch64, asmjit::Imm(routine));
      a_.call(kScratch64);

      // The flag is read before the stack is put back rather than after, because `add rsp` would clear
      // the flags a branch placed behind it depends on.
      a_.movzx(kScratch2_32, asmjit::x86::byte_ptr(asmjit::x86::rsp));
      a_.add(asmjit::x86::rsp, 32);

      *out_fault = a_.new_label();
      a_.test(kScratch2_32, kScratch2_32);
      a_.jne(*out_fault);

      // The result is stored only on the path that keeps it. On the fault path the interpreter's
      // re-run does the store, and doing it twice would be harmless here only by accident.
      if (wide)
        a_.mov(Var64(ins.var0()), kScratch64);
      else
        a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::TranslatedWithFaultRoute;
    }

    case AngelScript::asBC_ADDIf:
      a_.mov(kScratch32, FloatBits(ins.singleAfterWords()));
      a_.movd(asmjit::x86::xmm1, kScratch32);
      a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.addss(asmjit::x86::xmm0, asmjit::x86::xmm1);
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_SUBIf:
      a_.mov(kScratch32, FloatBits(ins.singleAfterWords()));
      a_.movd(asmjit::x86::xmm1, kScratch32);
      a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.subss(asmjit::x86::xmm0, asmjit::x86::xmm1);
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_MULIf:
      a_.mov(kScratch32, FloatBits(ins.singleAfterWords()));
      a_.movd(asmjit::x86::xmm1, kScratch32);
      a_.movss(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.mulss(asmjit::x86::xmm0, asmjit::x86::xmm1);
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Comparisons
      // ----------------------------------------------------------------------
    case AngelScript::asBC_CMPi:
      a_.mov(kScratch32, Var32(ins.var0()));
      EmitCompareToValueRegister(kScratch32, Var32(ins.var1()), true);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPu:
      a_.mov(kScratch32, Var32(ins.var0()));
      EmitCompareToValueRegister(kScratch32, Var32(ins.var1()), false);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPIi:
      a_.mov(kScratch32, Var32(ins.var0()));
      EmitCompareToValueRegister(kScratch32, asmjit::Imm(ins.integer()), true);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPIu:
      a_.mov(kScratch32, Var32(ins.var0()));
      EmitCompareToValueRegister(kScratch32, asmjit::Imm(ins.dword()), false);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPf:
      a_.movss(asmjit::x86::xmm1, Var32(ins.var1()));
      EmitFloatCompareToValueRegister(Var32(ins.var0()), asmjit::x86::xmm1, false);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPd:
      a_.movsd(asmjit::x86::xmm1, Var64(ins.var1()));
      EmitFloatCompareToValueRegister(Var64(ins.var0()), asmjit::x86::xmm1, true);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPIf:
      a_.mov(kScratch32, FloatBits(ins.single()));
      a_.movd(asmjit::x86::xmm1, kScratch32);
      EmitFloatCompareToValueRegister(Var32(ins.var0()), asmjit::x86::xmm1, false);
      return Outcome::TranslatedWithValueFlags;

      // ----------------------------------------------------------------------
      // Conversions
      // ----------------------------------------------------------------------
    case AngelScript::asBC_iTOf:
      a_.cvtsi2ss(asmjit::x86::xmm0, Var32(ins.var0()));
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_fTOi:
      // `cvttss2si` truncates toward zero, which is what the interpreter's C cast does.
      a_.cvttss2si(kScratch32, Var32(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_uTOf:
      // `cvtsi2ss` is signed, so an unsigned source has to go through a 64-bit register where every
      // 32-bit unsigned value is representable. Loading into a 32-bit register zero-extends it.
      a_.mov(kScratch32, Var32(ins.var0()));
      a_.cvtsi2ss(asmjit::x86::xmm0, kScratch64);
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_fTOu:
      // The interpreter truncates to `int` first and reinterprets the bits as unsigned, so this is
      // the same instruction as `fTOi`.
      a_.cvttss2si(kScratch32, Var32(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_sbTOi:
      a_.movsx(kScratch32, Var8(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_swTOi:
      a_.movsx(kScratch32, Var16(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_ubTOi:
      a_.movzx(kScratch32, Var8(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_uwTOi:
      a_.movzx(kScratch32, Var16(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_iTOb:
      a_.movzx(kScratch32, Var8(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_iTOw:
      a_.movzx(kScratch32, Var16(ins.var0()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_iTOd:
      a_.cvtsi2sd(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_uTOd:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.cvtsi2sd(asmjit::x86::xmm0, kScratch64);
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_fTOd:
      a_.cvtss2sd(asmjit::x86::xmm0, Var32(ins.var1()));
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_dTOi:
      a_.cvttsd2si(kScratch32, Var64(ins.var1()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_dTOu:
      a_.cvttsd2si(kScratch32, Var64(ins.var1()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_dTOf:
      a_.cvtsd2ss(asmjit::x86::xmm0, Var64(ins.var1()));
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // 64-bit integer arithmetic
      // ----------------------------------------------------------------------
      //
      // Nothing special is required beyond the operand width: every one of these has the same shape
      // as its 32-bit counterpart. They are worth having because a script that uses `int64` at all
      // tends to use it in a loop, where a single untranslated instruction costs the whole block.
    case AngelScript::asBC_ADDi64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.add(kScratch64, Var64(ins.var2()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_SUBi64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.sub(kScratch64, Var64(ins.var2()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_MULi64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.imul(kScratch64, Var64(ins.var2()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_BAND64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.and_(kScratch64, Var64(ins.var2()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_BOR64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.or_(kScratch64, Var64(ins.var2()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_BXOR64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.xor_(kScratch64, Var64(ins.var2()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_BSLL64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.mov(kScratch2_64, Var64(ins.var2()));
      a_.shl(kScratch64, asmjit::x86::cl);
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_BSRL64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.mov(kScratch2_64, Var64(ins.var2()));
      a_.shr(kScratch64, asmjit::x86::cl);
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_BSRA64:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.mov(kScratch2_64, Var64(ins.var2()));
      a_.sar(kScratch64, asmjit::x86::cl);
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_NEGi64:
      a_.neg(Var64(ins.var0()));
      return Outcome::Translated;

    case AngelScript::asBC_BNOT64:
      a_.not_(Var64(ins.var0()));
      return Outcome::Translated;

    case AngelScript::asBC_CMPi64:
      a_.mov(kScratch64, Var64(ins.var0()));
      EmitCompareToValueRegister(kScratch64, Var64(ins.var1()), true);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CMPu64:
      a_.mov(kScratch64, Var64(ins.var0()));
      EmitCompareToValueRegister(kScratch64, Var64(ins.var1()), false);
      return Outcome::TranslatedWithValueFlags;

    case AngelScript::asBC_CmpPtr:
      // The interpreter's own comment wonders why this is not an equals; either way it produces the
      // same -1/0/1, against an unsigned comparison, which is what it is written as here.
      a_.mov(kScratch64, Var64(ins.var0()));
      EmitCompareToValueRegister(kScratch64, Var64(ins.var1()), false);
      return Outcome::TranslatedWithValueFlags;

      // ----------------------------------------------------------------------
      // 64-bit conversions
      // ----------------------------------------------------------------------
      //
      // Two families with two different shapes, and the difference is not a detail: the ones to and
      // from `float` take their source from var1, and the ones to and from `double` work in place on
      // var0. The bytecode table knows this and so does the interpreter, so mixing them up would read
      // a plausible value out of the wrong slot.
    case AngelScript::asBC_iTOi64:
      a_.movsxd(kScratch64, Var32(ins.var1()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_uTOi64:
      a_.mov(kScratch32, Var32(ins.var1()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_i64TOi:
      a_.mov(kScratch64, Var64(ins.var1()));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_i64TOf:
      a_.cvtsi2ss(asmjit::x86::xmm0, Var64(ins.var1()));
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_u64TOf:
      EmitUnsignedInt64ToFloatingPoint(a_, Var64(ins.var1()), false);
      a_.movss(Var32(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_fTOi64:
      a_.cvttss2si(kScratch64, Var32(ins.var1()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_fTOu64:
      // The interpreter truncates to a signed 64-bit and reinterprets the bits as unsigned, so this
      // is the same instruction as fTOi64 rather than an unsigned conversion.
      a_.cvttss2si(kScratch64, Var32(ins.var1()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_i64TOd:
      a_.cvtsi2sd(asmjit::x86::xmm0, Var64(ins.var0()));
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_u64TOd:
      EmitUnsignedInt64ToFloatingPoint(a_, Var64(ins.var0()), true);
      a_.movsd(Var64(ins.var0()), asmjit::x86::xmm0);
      return Outcome::Translated;

    case AngelScript::asBC_dTOi64:
      a_.cvttsd2si(kScratch64, Var64(ins.var0()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_dTOu64:
      a_.cvttsd2si(kScratch64, Var64(ins.var0()));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Increment and decrement through the value register
      // ----------------------------------------------------------------------
      //
      // The interpreter's `**(T**)&m_regs.valueRegister` - the register holds an address, and the
      // operation happens at that address. Nothing here knows what the address points at, which is
      // the point: these are how a script updates a value it only holds a reference to.
    case AngelScript::asBC_INCi:
    case AngelScript::asBC_DECi:
    case AngelScript::asBC_INCi8:
    case AngelScript::asBC_DECi8:
    case AngelScript::asBC_INCi16:
    case AngelScript::asBC_DECi16:
    case AngelScript::asBC_INCi64:
    case AngelScript::asBC_DECi64: {
      a_.mov(kScratch64, ValueRegister64());

      const bool increment = ins.opcode() == AngelScript::asBC_INCi ||
                             ins.opcode() == AngelScript::asBC_INCi8 ||
                             ins.opcode() == AngelScript::asBC_INCi16 ||
                             ins.opcode() == AngelScript::asBC_INCi64;

      asmjit::x86::Mem target = asmjit::x86::dword_ptr(kScratch64);
      switch (ins.opcode()) {
      case AngelScript::asBC_INCi8:
      case AngelScript::asBC_DECi8:
        target = asmjit::x86::byte_ptr(kScratch64);
        break;
      case AngelScript::asBC_INCi16:
      case AngelScript::asBC_DECi16:
        target = asmjit::x86::word_ptr(kScratch64);
        break;
      case AngelScript::asBC_INCi64:
      case AngelScript::asBC_DECi64:
        target = asmjit::x86::qword_ptr(kScratch64);
        break;
      default:
        break;
      }

      if (increment)
        a_.inc(target);
      else
        a_.dec(target);
      return Outcome::Translated;
    }

    case AngelScript::asBC_INCf:
    case AngelScript::asBC_DECf:
    case AngelScript::asBC_INCd:
    case AngelScript::asBC_DECd: {
      // A floating point increment is an add of one, and there is no instruction for it. The one is
      // built from its bit pattern rather than loaded from a pool, since there is nowhere in the
      // emitted code to keep a constant pool.
      const bool is_double = ins.opcode() == AngelScript::asBC_INCd || ins.opcode() == AngelScript::asBC_DECd;
      const bool increment = ins.opcode() == AngelScript::asBC_INCf || ins.opcode() == AngelScript::asBC_INCd;

      a_.mov(kScratch64, ValueRegister64());
      if (is_double) {
        a_.mov(kScratch2_64, static_cast<std::uint64_t>(0x3FF0000000000000ull)); // 1.0
        a_.movq(asmjit::x86::xmm1, kScratch2_64);
        a_.movsd(asmjit::x86::xmm0, asmjit::x86::qword_ptr(kScratch64));
        if (increment)
          a_.addsd(asmjit::x86::xmm0, asmjit::x86::xmm1);
        else
          a_.subsd(asmjit::x86::xmm0, asmjit::x86::xmm1);
        a_.movsd(asmjit::x86::qword_ptr(kScratch64), asmjit::x86::xmm0);
      } else {
        a_.mov(kScratch2_32, static_cast<std::uint32_t>(0x3F800000u)); // 1.0f
        a_.movd(asmjit::x86::xmm1, kScratch2_32);
        a_.movss(asmjit::x86::xmm0, asmjit::x86::dword_ptr(kScratch64));
        if (increment)
          a_.addss(asmjit::x86::xmm0, asmjit::x86::xmm1);
        else
          a_.subss(asmjit::x86::xmm0, asmjit::x86::xmm1);
        a_.movss(asmjit::x86::dword_ptr(kScratch64), asmjit::x86::xmm0);
      }
      return Outcome::Translated;
    }

    case AngelScript::asBC_VAR:
      // Pushes the signed variable index itself, widened to a pointer - not the variable's contents.
      a_.mov(kScratch64, asmjit::Imm(static_cast<std::int64_t>(ins.var0())));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Object registers and stack-slot handles
      // ----------------------------------------------------------------------
      //
      // These move handles between a variable, the object register and a slot on the stack. None of
      // them touches a reference count - the interpreter's handlers do not either - so they are pure
      // data movement and translate exactly.
    case AngelScript::asBC_LOADOBJ:
      a_.mov(kScratch64, Var64(ins.var0()));
      a_.mov(asmjit::x86::qword_ptr(abi_.regs, static_cast<std::int32_t>(kOffObjectRegister)), kScratch64);
      a_.mov(Var64(ins.var0()), 0);
      a_.mov(asmjit::x86::qword_ptr(abi_.regs, static_cast<std::int32_t>(kOffObjectType)), 0);
      return Outcome::Translated;

    case AngelScript::asBC_STOREOBJ:
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.regs, static_cast<std::int32_t>(kOffObjectRegister)));
      a_.mov(Var64(ins.var0()), kScratch64);
      a_.mov(asmjit::x86::qword_ptr(abi_.regs, static_cast<std::int32_t>(kOffObjectRegister)), 0);
      return Outcome::Translated;

    case AngelScript::asBC_GETOBJ:
      // The argument names a stack slot holding a variable index; the handle moves out of that
      // variable and into the slot, and the variable is cleared.
      //
      // Both offsets here are counts of `asDWORD`s, not bytes, because both are added to the
      // interpreter's `asDWORD*` stack and frame pointers rather than to byte addresses. Getting that
      // wrong is not a subtle error: it reads a handle from four times further away than intended and
      // then dereferences it, which is why this is caught by a crash rather than by a wrong number.
      //
      // rcx holds the stack slot for the whole sequence, so the index scaling happens in rax and rdx
      // keeps the frame address - reusing rcx for the shift would lose the slot.
      a_.mov(asmjit::x86::rcx, abi_.sp);
      a_.add(asmjit::x86::rcx, static_cast<std::int32_t>(ins.word0() * kDwordSize));
      a_.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::rcx));

      a_.shl(asmjit::x86::rax, 2); // dword index to byte offset
      a_.mov(asmjit::x86::rdx, abi_.fp);
      a_.sub(asmjit::x86::rdx, asmjit::x86::rax);

      a_.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::rdx));
      a_.mov(asmjit::x86::qword_ptr(asmjit::x86::rcx), asmjit::x86::rax);
      a_.mov(asmjit::x86::qword_ptr(asmjit::x86::rdx), 0);
      return Outcome::Translated;

    case AngelScript::asBC_GETOBJREF:
      a_.mov(asmjit::x86::rcx, abi_.sp);
      a_.add(asmjit::x86::rcx, static_cast<std::int32_t>(ins.word0() * kDwordSize));
      a_.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::rcx));

      a_.shl(asmjit::x86::rax, 2);
      a_.mov(asmjit::x86::rdx, abi_.fp);
      a_.sub(asmjit::x86::rdx, asmjit::x86::rax);

      a_.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::rdx));
      a_.mov(asmjit::x86::qword_ptr(asmjit::x86::rcx), asmjit::x86::rax);
      return Outcome::Translated;

    case AngelScript::asBC_GETREF:
      // Like GETOBJREF, but the slot is given the *address* of the variable rather than its contents.
      // The interpreter casts the index to a signed int before subtracting, so this sign-extends
      // where GETOBJ does not; the two agree for every index the compiler emits, and matching each
      // exactly costs nothing.
      a_.mov(asmjit::x86::rcx, abi_.sp);
      a_.add(asmjit::x86::rcx, static_cast<std::int32_t>(ins.word0() * kDwordSize));
      a_.movsxd(asmjit::x86::rax, asmjit::x86::dword_ptr(asmjit::x86::rcx));

      a_.shl(asmjit::x86::rax, 2);
      a_.mov(asmjit::x86::rdx, abi_.fp);
      a_.sub(asmjit::x86::rdx, asmjit::x86::rax);

      a_.mov(asmjit::x86::qword_ptr(asmjit::x86::rcx), asmjit::x86::rdx);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Reads and writes through the pointer in the value register
      // ----------------------------------------------------------------------
      //
      // The counterpart of the increment family: the register holds an address, and a variable is
      // read from or written to it at a width chosen by the opcode.
    case AngelScript::asBC_WRTV1:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(kScratch2_32, Var32(ins.var0()));
      a_.mov(asmjit::x86::byte_ptr(kScratch64), asmjit::x86::cl);
      return Outcome::Translated;

    case AngelScript::asBC_WRTV2:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(kScratch2_32, Var32(ins.var0()));
      a_.mov(asmjit::x86::word_ptr(kScratch64), asmjit::x86::cx);
      return Outcome::Translated;

    case AngelScript::asBC_WRTV4:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(kScratch2_32, Var32(ins.var0()));
      a_.mov(asmjit::x86::dword_ptr(kScratch64), kScratch2_32);
      return Outcome::Translated;

    case AngelScript::asBC_WRTV8:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(kScratch2_64, Var64(ins.var0()));
      a_.mov(asmjit::x86::qword_ptr(kScratch64), kScratch2_64);
      return Outcome::Translated;

    case AngelScript::asBC_RDR1:
      // Reads a byte and clears the rest of the dword, exactly as the interpreter writes bytes 1-3 as
      // zero rather than leaving whatever the variable held.
      a_.mov(kScratch64, ValueRegister64());
      a_.movzx(kScratch32, asmjit::x86::byte_ptr(kScratch64));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_RDR2:
      a_.mov(kScratch64, ValueRegister64());
      a_.movzx(kScratch32, asmjit::x86::word_ptr(kScratch64));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_RDR4:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(kScratch32, asmjit::x86::dword_ptr(kScratch64));
      a_.mov(Var32(ins.var0()), kScratch32);
      return Outcome::Translated;

    case AngelScript::asBC_RDR8:
      a_.mov(kScratch64, ValueRegister64());
      a_.mov(kScratch64, asmjit::x86::qword_ptr(kScratch64));
      a_.mov(Var64(ins.var0()), kScratch64);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Pointers to a member, computed rather than read from the stack
      // ----------------------------------------------------------------------
      //
      // These three put a *member address* in the value register without touching the stack at all,
      // which is what every `object.member` and every `this.member` in a compiled program goes
      // through. Two of them dereference a variable to find their base and therefore have to check it;
      // the third takes the *address* of a variable, which is always valid, and does not.
      //
      // All three write the value register rather than the stack, so unlike `ADDSi` there is no trace
      // to leave behind on the fault path - the guard is taken before anything is stored, and the
      // interpreter re-running the instruction from scratch finds exactly the frame it left.
    case AngelScript::asBC_LoadThisR:
      // The base is the `this` pointer, which the calling convention puts in the first slot of the
      // frame - the word at the frame pointer itself, read at pointer width. The offset is a signed
      // *byte* displacement, since it is meant to be added to a pointer rather than scaled like a
      // variable index.
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.fp));
      EmitNullGuard(kScratch64, out_fault);
      a_.add(kScratch64, ins.sword0());
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_LoadRObjR:
      // A reference-typed variable below the frame pointer is followed to the object it names, and
      // the member offset is then added to that. The variable is named in `asDWORD`s, so it is scaled
      // the same way every other variable reference is; the member offset is a byte displacement and
      // is not.
      a_.mov(kScratch64, Var64(ins.var0()));
      EmitNullGuard(kScratch64, out_fault);
      a_.add(kScratch64, ins.sword1());
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_LoadVObjR:
      // The value form of the above: the base is the variable's own address rather than a pointer
      // read out of it. There is nothing to check, because a frame slot is never null - and checking
      // one would be checking the frame pointer, which the prologue has already proved usable.
      a_.lea(kScratch64, Var64(ins.var0()));
      a_.add(kScratch64, ins.sword1());
      a_.mov(ValueRegister64(), kScratch64);
      return Outcome::Translated;

      // ----------------------------------------------------------------------
      // Pointer dereferences that have to be able to fail
      // ----------------------------------------------------------------------
      //
      // The engine reports a null dereference as a script exception rather than letting the machine
      // fault, so every one of these tests its pointer and hands the instruction back when the test
      // fails. All seven raise the same message, and the guard is the same shape in all seven - which
      // is why it is one helper rather than seven transcriptions of it.
    case AngelScript::asBC_CHKREF:
      // The pointer on the top of the stack.
      EmitNullGuard(asmjit::x86::qword_ptr(abi_.sp), out_fault);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_RDSPtr:
      // The same, and then the pointer is followed. `RDSPtr` is where a null actually becomes
      // reachable - the stack slot holds the address of a reference, so a null there is a null
      // variable rather than a null value - and it is this instruction the interpreter blames.
      EmitNullGuard(asmjit::x86::qword_ptr(abi_.sp), out_fault);
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.sp));
      a_.mov(kScratch64, asmjit::x86::qword_ptr(kScratch64));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_ADDSi:
      // An offset added to the pointer on the stack, in place. The offset is signed: a *negative* one
      // walks backwards from the pointer, which is how the compiler reaches a base subobject, and
      // reading the argument as unsigned would turn that into a four-gigabyte jump.
      EmitNullGuard(asmjit::x86::qword_ptr(abi_.sp), out_fault);
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.sp));
      a_.add(kScratch64, ins.sword0());
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_ChkNullV:
      // A variable holding a pointer, rather than a pointer on the stack. Read at pointer width - the
      // interpreter casts the slot to `asDWORD**` and dereferences that, which is a pointer's worth of
      // bytes no matter what `asDWORD` names.
      a_.mov(kScratch64, Var64(ins.var0()));
      EmitNullGuard(kScratch64, out_fault);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_ChkNullS:
      // A stack *slot*, named as an offset in `asDWORD`s from the stack pointer. The scaling is the
      // part to get right: the interpreter's `l_sp` is an `asDWORD*`, so `l_sp + word0` is four bytes
      // per word, and taking the argument as a byte offset reads somewhere far above the slot the
      // engine meant.
      //
      // Reached by nothing, though unlike `PshC8` above the compiler *does* still emit it: its one
      // site is as_compiler.cpp:2412, inside `MoveArgsToStack`'s branch for a reference parameter of
      // object type, guarded by `args[n]->type.dataType.IsObjectHandle()`. The guard is the reason it
      // never fires - by the time that line runs the argument's handle has already been converted
      // away, so `IsObjectHandle()` is false and the branch is not taken. Six argument shapes that
      // ought to reach it (a handle passed to a `const T &in`, a temporary, a function's result, a
      // null handle, a member, an array element) all compile to `GETOBJREF`/`GETOBJ` followed by
      // `asBC_ChkRefS`, which is the case below and has two tests of its own. So the guard the shape
      // actually gets is the one two cases down, and this case is here for completeness.
      EmitNullGuard(asmjit::x86::qword_ptr(abi_.sp, static_cast<std::int32_t>(ins.word0() * kDwordSize)), out_fault);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_ChkRefS:
      // Two dereferences, not one: the stack holds a *handle*, and it is the object the handle names
      // that must not be null. A null handle is not checked here, because the interpreter does not
      // check it either - it dereferences the handle to reach the object and would fault on a null one
      // just as compiled code does. Reproducing the check the interpreter makes means reproducing the
      // one it does not make as well, or the two disagree about which programs crash.
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.sp));
      a_.mov(kScratch64, asmjit::x86::qword_ptr(kScratch64));
      EmitNullGuard(kScratch64, out_fault);
      return Outcome::TranslatedWithFaultRoute;

    case AngelScript::asBC_AllocMem: {
      // The initialization-list buffer: one allocation, zeroed, stored straight into a local.
      //
      // The interpreter's handler is `*var = asNEWARRAY(asBYTE, size); memset(*var, 0, size);`
      // (as_context.cpp:4602), and the allocation is why this is a call rather than a `malloc`: the
      // buffer is freed later by `asBC_FREE`, which reaches `asCScriptEngine::CallFree` and that is
      // `userFree` - the same global pair `asNEWARRAY` allocates from (`as_memory.h:77`,
      // `as_scriptengine.cpp:4985`). Allocating from a different heap would produce a buffer the
      // interpreter's free would then hand to the wrong deallocator.
      //
      // Those two globals are reached through `asAllocMem`, which the SDK publishes and which
      // as_memory.cpp defines as exactly the `asNEWARRAY` expression above (as_memory.cpp:171). So
      // this is not a reimplementation of the interpreter's allocator; it is the interpreter's
      // allocator, under its public name.
      //
      // `asBC_FREE` stays interpreted, and that is not a gap: the buffer is only freed once, at scope
      // exit, and by the time it is, the object type is the only thing that knows what is in it.
      const auto size = static_cast<std::int32_t>(ins.dword());

      // Shadow space for both calls at once - neither has more than one argument, and allocating it
      // once keeps the stack balanced on the one path out of here.
      a_.sub(asmjit::x86::rsp, 32);

      a_.mov(kScratch2_32, size);
      a_.mov(kScratch64, asmjit::Imm(reinterpret_cast<std::uint64_t>(&AngelScript::asAllocMem)));
      a_.call(kScratch64);

      // Stored before the zeroing call, which needs the pointer in rcx and is about to use rax for its
      // own address. The interpreter stores first as well, and for a reason that does not apply here -
      // but the order costs nothing and is the one the handler reads in.
      a_.mov(Var64(ins.sword0()), kScratch64);
      a_.mov(kScratch2_64, kScratch64);

      a_.mov(asmjit::x86::r8d, size);
      a_.xor_(asmjit::x86::edx, asmjit::x86::edx);
      a_.mov(kScratch64, asmjit::Imm(reinterpret_cast<std::uint64_t>(static_cast<void* (*)(void*, int, std::size_t)>(&std::memset))));
      a_.call(kScratch64);

      a_.add(asmjit::x86::rsp, 32);
      return Outcome::Translated;
    }

    // ----------------------------------------------------------------------
    // The rest of the initialisation-list family
    // ----------------------------------------------------------------------
    //
    // `AllocMem` above is the first of four instructions that together build an array or dictionary
    // literal, and the other three are here. The four touch the engine not at all: they allocate a
    // buffer, write its header and store its elements, all through pointers the script already owns.
    // Nothing about them needs an object type's behaviour table or an address to call, which is the
    // test that separates them from `ALLOC` and `FREE` below - those two are refused, and these three
    // were refused only for having been written down as one group.
    //
    // The SDK's JIT guide names the group and expects all four, under "Initialization list
    // management": "Allocates the memory for the initialization list buffer / asBC_AllocMem", "Sets
    // the number of elements that will be repeated afterwards / asBC_SetListSize", "Sets the type of
    // the next element / asBC_SetListType", "Pushes the address of the list element on the stack /
    // asBC_PshListElmnt". So this is not an invention of the emitter's - the four are one of the
    // sections the documentation groups instructions by, and three of them were in the refusal column
    // because they had never been written rather than because anything stood in the way.
    //
    // Which is the reason they are worth stating as a group rather than three separate cases: the
    // boundary this emitter draws is "does the instruction need a private class layout", not "how
    // exotic is the instruction", and a literal is not exotic. It is what `int[] a = {1, 2, 3};`
    // compiles to.
    case AngelScript::asBC_SetListSize:
    case AngelScript::asBC_SetListType: {
      // Two opcodes, one instruction. The interpreter's handlers are the same three statements with
      // different names on them - write an `asUINT` at `var + off` (as_context.cpp:4622 and :4651) -
      // and the only thing that distinguishes a count from a type id is which header field the offset
      // points at. Two cases here would be two copies of one store, free to drift apart.
      //
      // `off` is a byte displacement into the buffer (`*(asUINT*)(var+off)`) while `sword0` names the
      // *local holding the buffer pointer*, in `asDWORD`s below the frame pointer like every other
      // variable. Two different scales, in the same instruction.
      //
      // The two names are reached by different literals, which is worth knowing before deciding one of
      // them is untested: `SetListSize` is in every list pattern and a typed `array` literal produces
      // it, while `SetListType` records a *runtime* element type and is emitted only for the `?` of a
      // pattern whose type is deduced from the value (as_compiler.cpp:4143, and :4269 for a null
      // handle). A `dictionary literal`, whose registered pattern is `{repeat {string, ?}}`, is what
      // produces that; `array<int>` never does.
      //
      // No null guard, because the interpreter has none: its `asASSERT(var)` at both sites is compiled
      // out in Release and the store faults in both arms alike. A guard here would make compiled code
      // raise a script exception where interpreted code segfaults, which is a difference in what the
      // two *do* rather than in how fast they do it - and `EmitNullGuard` would not even help, since
      // the hand-back re-runs this same instruction in the interpreter and faults there instead.
      a_.mov(kScratch64, Var64(ins.sword0()));
      a_.mov(kScratch2_32, ins.dwordAfterDword());
      a_.mov(asmjit::x86::dword_ptr(kScratch64, static_cast<std::int32_t>(ins.dword())), kScratch2_32);
      return Outcome::Translated;
    }

    case AngelScript::asBC_PshListElmnt:
      // The address of one element, pushed: `var + off` rather than the value there, so this is the
      // `PSF` plus `ADDSi` pair the compiler emits for a member access, fused into one instruction
      // (as_context.cpp:4636). The pointer is what the element's own constructor or assignment reads
      // and writes through, so the addition is what the instruction is for - storing the buffer
      // pointer alone would hand every element the same address.
      //
      // `off` is again a byte displacement and `sword0` again names the local, so the same two scales
      // as the case above.
      a_.mov(kScratch64, Var64(ins.sword0()));
      a_.add(kScratch64, static_cast<std::int32_t>(ins.dword()));
      a_.sub(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::Translated;

    case AngelScript::asBC_COPY: {
      // Pops a destination pointer, then copies the block the source pointer names onto it and leaves
      // the destination in its place. Both pointers can be null and both are checked before anything
      // moves - the two checks are the only place in the emitter where one instruction guards twice,
      // and they branch to one exit rather than two. See `EmitNullGuard` for what the second one
      // overwriting the first used to do here, which is nothing at all.
      //
      // Unlike the interpreter, nothing is popped until after the check. The interpreter pops first
      // and then raises with the stack already advanced; that is correct for it, because it never runs
      // the instruction again, but this exit *does* - the interpreter takes over at the instruction
      // and runs it from the start. Handing it a stack it has already half-popped would pop a second
      // time and leave the frame pointer four bytes short of where the unwinder expects it.
      const auto size_in_bytes = static_cast<std::int64_t>(ins.word0()) * static_cast<std::int64_t>(kDwordSize);

      const auto destination = asmjit::x86::qword_ptr(abi_.sp);
      const auto source = asmjit::x86::qword_ptr(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(kScratch64, destination); // the destination, kept in rax until it is stored back below
      a_.mov(asmjit::x86::rdx, source);
      EmitNullGuard(kScratch64, out_fault);
      EmitNullGuard(asmjit::x86::rdx, out_fault);

      if (size_in_bytes <= kInlineCopyBytes) {
        // Straight-line moves. The count is a compile-time constant here, so the loop that a copy
        // routine would run is unrolled instead of executed, and the whole thing costs two
        // instructions per eight bytes rather than the ~20 cycles a `rep movsb` spends starting up.
        // See `kInlineCopyBytes` for where the line between the two is drawn.
        //
        // The tail is four bytes and never narrower, which is not an approximation: the instruction
        // carries its count in *dwords* - the interpreter's handler reads it with `asBC_WORDARG0` and
        // multiplies by four - so the byte count is a multiple of four by construction. There is no
        // object of five, six or seven bytes that could reach this, and a two-byte step written to
        // handle one would be a branch nothing can ever take.
        //
        // Nothing is rounded up to an eight-byte multiple either. A read past the end of the object
        // could take a page fault the interpreter would not take, and a write past it would corrupt
        // whatever sits next in the frame, so the widths are exact rather than convenient.
        const auto copy8 = [&](std::int32_t offset) {
          a_.mov(kScratch2_64, asmjit::x86::qword_ptr(asmjit::x86::rdx, offset));
          a_.mov(asmjit::x86::qword_ptr(kScratch64, offset), kScratch2_64);
        };
        const auto copy4 = [&](std::int32_t offset) {
          a_.mov(kScratch2_32, asmjit::x86::dword_ptr(asmjit::x86::rdx, offset));
          a_.mov(asmjit::x86::dword_ptr(kScratch64, offset), kScratch2_32);
        };

        for (std::int64_t offset = 0; offset < size_in_bytes; offset += 8) {
          if (size_in_bytes - offset >= 8)
            copy8(static_cast<std::int32_t>(offset));
          else
            copy4(static_cast<std::int32_t>(offset));
        }
      } else {
        // Past the limit, unrolling loses: the code is emitted per instruction, so a large copy would
        // cost its own length in instructions and inflate the function for a case that is rare in a
        // script. `rep movsb` it is, which on anything with fast-short-rep pays a fixed startup and
        // then moves memory at uncached bandwidth.
        //
        // The direction flag is cleared by the calling convention and left cleared, which is the state
        // `rep` requires. These are the only two instructions in the emitter that touch rdi or rsi,
        // and the prologue parks them for a function that contains this opcode for that reason.
        a_.mov(asmjit::x86::rdi, kScratch64);
        a_.mov(asmjit::x86::rsi, asmjit::x86::rdx);
        a_.mov(asmjit::x86::ecx, size_in_bytes);
        a_.rep().movsb();
      }

      a_.add(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(asmjit::x86::qword_ptr(abi_.sp), kScratch64);
      return Outcome::TranslatedWithFaultRoute;
    }

      // ----------------------------------------------------------------------
      // Reference copies that do not touch a reference count
      // ----------------------------------------------------------------------
      //
      // `REFCPY` and `RefCpyV` are the two opcodes that assign an object handle, and the interpreter's
      // handler for each is the same body (as_context.cpp:3276 and :4553), which reduces to one pointer
      // store unless the type is counted:
      //
      //     if( !(objType->flags & (asOBJ_NOCOUNT | asOBJ_VALUE)) ) {
      //         if( *d != 0 && beh->release ) m_engine->CallObjectMethod(*d, beh->release);
      //         if( s != 0 && beh->addref )   m_engine->CallObjectMethod(s, beh->addref);
      //     }
      //     *d = s;
      //
      // That condition is decidable here, at compile time, and decidable *exactly* rather than
      // approximately: the object type is not looked up through the engine, it is carried in the
      // instruction. The handler reads it as `(asCObjectType*)asBC_PTRARG(l_bc)`, which is
      // `ins.pointer()` below - a pointer baked into the bytecode when the script was compiled, and
      // stable for as long as the module that owns it.
      //
      // What makes that pointer usable without an internal header is the SDK's own inheritance chain:
      // `asCObjectType : public asCTypeInfo : public asITypeInfo` (as_objecttype.h:101,
      // as_typeinfo.h:63, angelscript.h:1069). Single inheritance at every step, so the base subobject
      // is at offset zero, and a pointer to the object type is simultaneously a valid `asITypeInfo*` -
      // which is precisely how the engine itself hands these out. `GetFlags()` is a public pure virtual
      // on `asITypeInfo` (angelscript.h:1087), so the flags are read through the vtable: no private
      // access, no internal type named, and no layout assumed beyond the one guarantee C++ makes about
      // single inheritance.
      //
      // The counted half is not here. `beh->addref` and `beh->release` are members of `asCObjectType`
      // reached through a layout that `GetFlags()` deliberately does not expose, so this case takes
      // only the types where the store *is* the assignment, and a counted one is translated in the
      // function compiler instead, by calling the engine's own `AddRefScriptObject` and
      // `ReleaseScriptObject` - the same two routines the interpreter's handler calls, in its order.
      // `runtime.cpp` has that reasoning; what matters here is that the split is drawn by the type and
      // not by the two code paths, so both arms ask the same question of the same data and there is no
      // type the two can disagree about.
      //
      // The public interface was checked for a way to do it *in this shape* rather than assumed to
      // lack one, since the pointer is already in hand and only the addresses are missing.
      // `asITypeInfo` does reach the behaviours - `GetBehaviourByIndex(n, &outBehaviour)`
      // (angelscript.h:1117) - but it returns an `asIScriptFunction*`, which has no callable address
      // in it, so a translation built on it would have to re-enter the VM per handle assignment: a
      // cost above the interpreted instruction it would be replacing. The engine's exported routines
      // are what make it cheap, and they take a type and a pointer rather than a behaviour, which is
      // why the counted half lives where it does.
    case AngelScript::asBC_REFCPY: {
      if (!IsRefcountFreeObjectType(ins.pointer()))
        return Outcome::Unsupported;

      // The destination is popped and the source read from *below* it, which is the order the handler
      // uses and not an arbitrary choice: `s` is read from the stack after the pop, so reading it from
      // the old top would take the destination pointer as the object to be assigned.
      //
      // No fault route. The instruction can still fault - a stale `d` is a write to an address the
      // script should not own - but the interpreter's own handler has already committed to the same
      // write by then, so there is no state to protect by handing back, and the store is the last thing
      // the handler does.
      a_.mov(kScratch64, asmjit::x86::qword_ptr(abi_.sp));
      a_.add(abi_.sp, static_cast<std::int32_t>(kPtrSize));
      a_.mov(kScratch2_64, asmjit::x86::qword_ptr(abi_.sp));
      a_.mov(asmjit::x86::qword_ptr(kScratch64), kScratch2_64);
      return Outcome::Translated;
    }

    case AngelScript::asBC_RefCpyV: {
      if (!IsRefcountFreeObjectType(ins.pointer()))
        return Outcome::Unsupported;

      // Same assignment, and the two differences from the above are both in the handler: the
      // destination is a named local rather than a pointer off the stack - this is "assign to v", not
      // "assign and leave a result" - and the source is read without being popped, because the
      // surrounding expression still owns that slot.
      //
      // The destination is the *address* of the variable, since `d` is a `void**` and the store goes
      // through it; `lea` rather than a load is what makes the difference, and a `mov` here would
      // write the handle over the variable's own address in memory.
      a_.lea(kScratch64, asmjit::x86::ptr(abi_.fp, VarDisp(ins.sword0())));
      a_.mov(kScratch2_64, asmjit::x86::qword_ptr(abi_.sp));
      a_.mov(asmjit::x86::qword_ptr(kScratch64), kScratch2_64);
      return Outcome::Translated;
    }

    default:
      // Everything else is left exactly as it is. The block ends here and the interpreter runs this
      // instruction and everything after it until the next entry point.
      //
      // **The call family used to be refused here, and the reason written down for it was wrong.**
      // That version said a translated `asBC_CALL` could not invoke its callee "even if every private
      // symbol were declared", because the only handle a JIT is given - the public `asIScriptFunction`
      // - publishes no callable address, and because the entry points that do take arguments of types
      // this target has no definition of. Both halves of that were true and the conclusion did not
      // follow from them, which is worth recording rather than quietly deleting, because the reasoning
      // reads as sound right up to the last step.
      //
      // The address is not needed. A callee is reached the way the interpreter reaches it - by id,
      // through the engine's own table - and the absence of a callable address in `asIScriptFunction`
      // is not an oversight to be worked around: a JIT that held one would be holding a pointer the
      // engine cannot invalidate when the module goes away. And the internal types do not have to be
      // *defined* to be *named*: a declaration is enough to form a call, and a declaration that
      // disagrees with the definition stops matching the exported symbol's mangled name and fails the
      // link. That is a checked failure. The failure this file's header comment names - a class layout
      // reproduced by hand and drifting out of step - is a silent one, and the two are not the same
      // risk. `runtime.cpp` is built on that distinction, and the three declarations at the top of it
      // are the whole of the dependency.
      //
      // So the calls are translated, in the function compiler rather than here - and they do not all
      // have the same shape, which is worth stating because this paragraph used to say they did.
      // `asBC_CALL` and `asBC_CALLINTF` end their block with a bare epilogue and nothing published,
      // because after them the registers describe the *callee*. `asBC_CALLSYS` does not: a registered
      // function is entered and left like any other routine, and the block carries on across the call
      // with the published stack pointer reloaded. The reason is the one given above - a native call
      // needs no frame of its own, so nothing about the registers changes except the stack pointer the
      // marshaller advanced - and the consequence is the whole shape of a compiled menu script: a run of
      // native code punctuated by calls rather than a block per call. That distinction is not academic.
      // The benchmark suite was designed on the assumption that "every call ends the block", which is
      // what this paragraph said, and its `generic_calls` row was written expecting to lose and came out
      // at 1.61x on 92.5% coverage instead. See the CALL and CALLSYS cases in `function_compiler.cpp`
      // for what each translation removes, and `runtime.cpp` for the entry points they reach.
      //
      // `asBC_RET` is the guide's second clause - "the instructions that setup a call to a new script
      // function, or that return from a previous instruction" - and it is off by default, but not for
      // the reason the clause gives. What the clause names is the *frame*, and the frame was never the
      // obstacle: the engine's own `asCContext::PopCallState` pops it, reached the way the other
      // internal entry points are. It was translated on exactly that basis, verified against the
      // interpreter, and then turned off on measurement - the predicate that says whether a frame may
      // be popped is a call into the engine that serialises two pointers, and paying it on every
      // return made the call-heavy rows of the benchmark twenty to forty-five percent slower, with
      // nothing gained anywhere past the control's own spread. The RET case in `function_compiler.cpp`
      // is the record: the table, the method, and the cheaper predicates that were tried and are each
      // wrong on their own.
      //
      // It is in the list below rather than in the paragraph above for that reason alone, and the
      // distinction is worth keeping straight - the calls above are translated because the guide's
      // advice turned out not to apply to them, and this one is off because it applies and the
      // translation is not worth having anyway. The first was a reasoning error; this is a judgement
      // with numbers behind it.
      //
      // And off rather than absent, which is the distinction the `Options` comment in `asjit.hpp`
      // draws: the case in `function_compiler.cpp` is written and compiled into this library, behind
      // `Options::translate_returns`, and the differential suite runs it. What is below is the default
      // configuration, and this instruction is one of exactly two that a default decision is made
      // about rather than a translation.
      //
      // What is left is the residue, and each entry below is refused on its own grounds. The
      // distinction that matters is between an instruction that *cannot* be translated without taking
      // on a risk this component has decided not to take, and one that could be but is not worth it -
      // the second is a judgement, and a judgement is only honest if it is written down as one.
      //
      // `asBC_ALLOC`'s script-object branch used to be refused here, on the judgement that a few
      // dispatches per object ever created were not worth the argument rewriting and the cleanup tail
      // the translation would have to get exactly right. Half of that was a judgement and the other
      // half was a miscount: the benchmark suite's sixteen sites are construction paths, but a
      // construction path is a loop body as soon as the script constructs in one, and the
      // argument-slot rewrite the translation looked hardest to reproduce turned out to be the
      // engine's own answer rather than this component's to derive - `GetSpaceNeededForArguments` is
      // the interpreter's computation of where the slot sits. So it is translated, in
      // `function_compiler.cpp`, with the allocation and the pre-initialisation reached through
      // `CallAlloc` and `ScriptObject_Construct` - declarations of the same kind as the ones
      // `runtime.cpp` already makes. See the ALLOC case there and `runtime::AllocateScriptObject`.
      //
      // `asBC_ALLOC`'s *value*-type branch was refused next, and that paragraph was wrong in both of
      // its two halves - which is worth recording, because the two errors are different mistakes and
      // only one of them was a judgement.
      //
      // The first was the tail. It allocates, pushes the object pointer, calls the constructor as a
      // system function (as_context.cpp:3167), pops a variable address, and then, if `doProcessSuspend`
      // is set, asks two questions: whether `asCContext::m_doSuspend` says a suspension is pending, in
      // which case it stops; and, failing that, whether the constructor raised, in which case it hands
      // the fresh memory back to the allocator and nulls the variable. The refusal treated the pair as
      // inseparable because the first question has no accessor. It is not the pair that matters - it
      // is which answer is unavailable. A suspension the translation fails to take is a *delay*: the
      // VM takes it at the next point it checks for one, which is the margin `bail_if_suspend` already
      // takes at every call. An exception the translation fails to notice is a leak and a variable
      // pointing at memory the allocator has already been given back. Only one of those is
      // irrecoverable, and it is the one that is published: `asIScriptContext::GetState` returns
      // `m_status`, which is the whole of the second question. The refusal refused the instruction
      // over the half it could answer.
      //
      // The second was reachability, and it was asserted rather than measured. The claim was that the
      // branch could not be reached from a script at all - that `IsVariableOnHeap` restricts it to
      // module globals, reference members and generated factory stubs, none of which is a function the
      // JIT is offered. That is true of the two it names and false as a description of the branch: a
      // **value type passed by value is constructed on the heap at the call site**, so an ordinary
      // `Take(Vec3(1.0f, 2.0f, 3.0f))` walks one and a value type forwarded through two calls walks
      // two. The probe that established this took one test and four minutes, and the paragraph it
      // replaced had been sitting in this list for three sessions. A claim about what no script
      // produces is a claim a script can settle.
      //
      // So it is translated, in `function_compiler.cpp`, with the allocation reached through
      // `CallAlloc`, the constructor called through the same `CallSystemFunction` the `asBC_CALLSYS`
      // translation uses, and the exception tail written out against `GetState`. See the ALLOC case
      // there and `runtime::ConstructValueObject`.
      //
      // `asBC_Thiscall1` is off by default, and it is the one entry in this list whose default rests on
      // reachability rather than on cost. The cost is real and was measured, but it is the second
      // reason and not the first, and the first is the one that cannot be answered with a faster
      // machine.
      //
      // The instruction exists to be fast, and what makes it fast is the routine behind it. The
      // compiler emits it for exactly one shape (as_compiler.cpp:17433) - a method on an object type
      // returning a reference and taking one four-byte integer - and the handler at
      // `as_context.cpp:4800` is written to that assumption rather than checking it: it does
      // everything itself, with its own null check, its own two stack pops, and its own call through
      // `asCScriptEngine::CallObjectMethodRetPtr` (as_context.cpp:4844). That routine is the one call
      // routine in the engine that takes no context, which is possible because a one-pointer method
      // call needs no marshalling, and that absence is the whole of the saving.
      //
      // So the obvious translation is to reach the same routine, and it was written and it works: the
      // call is made, the reference lands in the value register, and the benchmark's call-heavy rows do
      // not move. It is also wrong, in a way no benchmark can see, because the interpreter pairs that
      // call with one line the routine does not need and the *script* does. `m_callingSystemFunction` is
      // set to the callee before the call and cleared after it (as_context.cpp:4835 and :4853), and
      // `asCContext::SetException` - the only way a native function can raise - opens with
      // `if (m_callingSystemFunction == 0) return asERROR;` (as_context.cpp:4985), raising nothing.
      // Nothing else on that path can set the field: it has no public accessor, the writer that restores
      // it is gated on `AS_EXECUTION_DESERIALIZATION`, and the general `CallSystemFunction` sets and
      // clears it internally for its own calls (as_callfunc.cpp:790 and :821; the generic branch at
      // as_context.cpp:5869). A translation that calls `CallObjectMethodRetPtr` from outside the
      // interpreter therefore runs methods whose failures are silently dropped.
      //
      // That is a wrong script result rather than a crash, and the differential suite states it exactly.
      // `Fault::Get(int)` has the one signature the compiler picks this instruction for, and raises when
      // handed a negative argument. Interpreted, the script stops at the call with
      // `asEXECUTION_EXCEPTION` and the string "fault: method called with a negative argument"; compiled
      // through the fast path, the same script reports `asEXECUTION_FINISHED`, an empty exception string,
      // and runs on past the call to print the value the raise was supposed to prevent - `11\n` against
      // `11\n9\n999\n`. A second test does worse than diverge: `Differential.TryCatchCatchingAnException
      // FromANative` faults under the fast path, because a `try` around such a call expects the
      // exception machinery to have run. `Differential.AThiscall1ShapedMethodThatRaisesAgreesWithThe
      // Interpreter` is the test that pins this, and it is green with the instruction handed back.
      //
      // The general path has no such problem, and that was established rather than assumed: it sets and
      // clears `m_callingSystemFunction` itself, so a method reached through it raises exactly as it does
      // from the interpreter, and it reproduces the rest of the fast path - the same null check and the
      // same `TXT_NULL_POINTER_ACCESS` exception, the same published stack pointer left on the object
      // pointer for the exception handler (as_callfunc.cpp:735), and the reference written to the same
      // value register in the tail branch that names references (as_callfunc.cpp:889). With this case
      // translating against `CallSystemFunction` the suite is green - and that is no longer a past
      // tense about an experiment: the case is in `function_compiler.cpp` behind
      // `Options::translate_object_method_calls`, and the test that pins the raise above runs under
      // both configurations, so both halves of this paragraph are asserted rather than described.
      //
      // Which leaves the general path as the only faithful route, and prices it, because it is exactly
      // the trade the instruction exists not to make: the marshaller, for a call that needs no
      // marshalling. The benchmark was asked, both arms built and run with the *interpreted* column as
      // the control, since nothing done to this instruction can touch it:
      //
      //   row                    interp control   translated   refused
      //   arith_int                        0.98x       8.66x     8.88x
      //   array_index                      1.01x       0.80x     1.79x
      //   cast_heavy                       1.00x       1.04x     1.04x
      //
      // `array_index` is the only row in the suite that reaches the instruction - four sites, all
      // inside that row's loop - and it is the whole of the result: 81.4 ms compiled with the case
      // translating against 36.1 ms with it refused, a 2.26x slowdown of the compiled arm, or 1.79x
      // speedup turned into 0.80x. The same row's interpreted arm moved by 0.9% between the two runs,
      // and the two rows that do not reach the instruction moved by under 1%, which is what makes that
      // ratio the instruction's cost rather than the machine's. Coverage of the row *rose* while its
      // ratio fell - 81.6% against 87.2% - which is the useful shape of the result: translating more is
      // not the same as running faster, and this is the row where the two disagree.
      //
      // The suite now takes that measurement rather than quoting it. The `fullcov` benchmark variant is
      // this configuration, the rows that could reach either instruction opt into it, and running one
      // gives the pair of rows directly - `array_index` at 1.38x and 81.7% against 0.62x and 92.7% in
      // the run this paragraph was last checked against, which is the same result with the same shape.
      // A future engine version that moved it would move a row rather than require this argument to be
      // made again.
      //
      // One caveat, because it applies to every table this component has recorded. The interpreted
      // column is stable *within* a pair of runs and not across them: `arith_int`'s interpreted time has
      // been anywhere between 321 ms and 462 ms over these five runs while that same row's compiled time
      // stayed inside 53.1 ± 1.6 ms. So the control is what makes a pair readable, the raw compiled
      // times are the part of a table to trust on its own, and a ratio quoted across two runs that were
      // not taken together means nothing.
      //
      // The two grounds decide different things, which is why both are written down. Correctness decides
      // *which* route is available at all, and it is not a measurement - the fast path is not slower
      // here, it is unfaithful, so no amount of tuning reaches it. Cost then decides whether the one
      // available route is worth taking, and what would overturn that is a cheaper general path - a
      // `CallSystemFunction` that skipped the marshaller for one-pointer calls, which is the fast path
      // itself - and not a better translation of this instruction, since the translation is already the
      // best available route to the engine's own call.
      //
      // `asBC_CallPtr` was refused here for the layout reason and is no longer, which is worth the
      // sentence because the refusal read as final. The dispatch is on the callee's *kind* -
      // `func->funcType`, four cases with their own stack handling (as_context.cpp:4301) - and that
      // field does belong to the function object. But the question it answers is published:
      // `asIScriptFunction::GetFuncType` reports the same value, and the delegate accessors beside it
      // publish the two fields that case reads. So the dispatch is translated against the published
      // interface, and the two kinds the interface cannot reach - an unbound variable and an imported
      // function - are handed back to the interpreter rather than handled, which is what the
      // translation's false return is for. See the CallPtr case in `function_compiler.cpp` and
      // `runtime::EnterFunctionPointer`.
      //
      // `asBC_Cast` was refused here for the layout reason in its strongest form: "there is nothing in
      // it that is not layout", four reads out of two classes this target has no definitions for. That
      // was false, and it was false in the way a wrong reason is worse than no reason - it read as
      // settled. Every one of the four is published, and the instruction is translated in the function
      // compiler beside the object lifetime operations, which is where a reference-counting
      // instruction belongs. The paragraph that used to stand here is worth one sentence rather than
      // nothing, because the mistake in it was not the reachability: the handler reads
      // `asCScriptObject::objType` and calls `obj->AddRef()`, both of which really are out of reach,
      // and the conclusion drawn was that there was no route at all rather than no *direct* route.
      //
      // It has since been measured, which the rest of the refusals in this list have not been and the
      // reason it is worth the two sentences. `cast_heavy` is the row written for it - a loop that casts
      // to an interface the object's dynamic type implements and to a sibling type it does not, so both
      // outcomes and the reference each one takes or does not take are in the loop body - and the two
      // arms differ only in whether this case translates:
      //
      //   row                    interp control   translated   refused
      //   cast_heavy                       1.00x       1.04x     1.03x
      //
      // A 1% gain on a row that is 76% translated against 71%, and the interpreted arm of the row moved
      // by 0.7% between the two runs. So the translation is roughly free rather than profitable, and it
      // is kept: the instruction is on the path of every handle cast a script makes, the alternative to
      // translating it is a hand-back at the cast, and 1% is what the measurement says the hand-back is
      // worth. It is written down because "roughly free" is a number here rather than an impression,
      // and because the row that measures it is the one to re-run if this case ever changes.
      //
      // `asBC_STR` is not refused so much as absent: its handler is `asASSERT(false)` followed by a
      // skip of its own argument (as_context.cpp:2972), under a "TODO: NEWSTRING: Deprecate this
      // instruction". The interpreter aborts on it. Falling through to here reproduces that exactly,
      // and a case would mean choosing a behaviour the engine does not have.
      //
      // `asBC_CALLBND` is the one refusal here that would be a translation if the engine published one
      // more thing. Its handler resolves an import index to the function bound to it and then calls it
      // by exactly the routes this component already has - `CallScriptFunction` for a script target,
      // `CallSystemFunction` for a registered one, and both are already reached from
      // `function_compiler.cpp`'s `asBC_CALL`. What
      // is missing is the resolution: `m_engine->importedFunctions[i & ~FUNC_IMPORTED]->boundFunctionId`
      // is the interpreter's whole answer, and `importedFunctions` is private with no accessor. The
      // published pair that looks like it would do - `asIScriptFunction::GetId` into
      // `asIScriptEngine::GetFunctionById` - does not, because the id in the bytecode is an *import*
      // index tagged with `FUNC_IMPORTED` and `GetFunctionById` takes a function id. Two things would
      // have to change to close it, and the second is why it is not worth pursuing: the importing
      // module would also have to exist. `asBC_CALLBND` is emitted only for a call to a function the
      // module declared `import` (as_compiler.cpp:17425), and the menu builds one module per script and
      // never calls `BindImportedFunction`, so it has no import to reach.
      //
      // asBC_SUSPEND is not here only because the function compiler intercepts it before this point:
      // ending the block is what the suspend test does, so it cannot be an instruction translation.
      return Outcome::Unsupported;
    }
  }

} // namespace base::asjit
