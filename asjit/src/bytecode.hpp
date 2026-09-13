//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>

#include <cstddef>
#include <cstdint>

namespace base::asjit {

  // A bytecode instruction, seen the way the interpreter sees it.
  //
  // AngelScript publishes both tables this needs in the public header: `asBCInfo[256]` maps an opcode
  // to its argument layout, and `asBCTypeSize[21]` turns that layout into a size in `asDWORD`s. Both
  // are namespace-scope `const`, which gives them internal linkage, so every translation unit gets its
  // own copy - there is nothing to link against and nothing that can drift out of sync with the engine
  // binary the way a hand-written size table would.
  using ByteCode = AngelScript::asDWORD;

  // The instruction argument macros below are the SDK's own, not reimplementations of them. Their
  // layout is not the obvious one - the opcode occupies the first `asDWORD`, so the variable words
  // live at byte offsets 2, 4 and 6 rather than 1, 2 and 3 - and a mistake there does not fail to
  // compile, it reads a plausible number from the wrong place and corrupts the frame.
  //
  // They expand to *unqualified* `asDWORD`, `asQWORD` and `asPWORD`, which with AS_USE_NAMESPACE
  // means the names have to be reachable at the point of use rather than spelled out. These three
  // declarations are what put them there, and they have to sit in the same namespace as the class
  // below for its member bodies to see them. Naming the macro `AngelScript::asBC_DWORDARG` does not
  // work, since that qualifies the macro's expansion rather than the macro itself.
  using AngelScript::asDWORD;
  using AngelScript::asPWORD;
  using AngelScript::asQWORD;
  using AngelScript::asWORD;

  class Instruction {
  public:
    explicit constexpr Instruction(const ByteCode* byteCode) noexcept : bc_(byteCode) {}

    // The opcode byte, which every argument is laid out after.
    [[nodiscard]] AngelScript::asEBCInstr opcode() const noexcept {
      return static_cast<AngelScript::asEBCInstr>(*reinterpret_cast<const AngelScript::asBYTE*>(bc_));
    }

    // Size in `asDWORD`s - what the interpreter adds to its program counter to reach the next
    // instruction. The jump instructions are the exception and carry their own displacement; nothing
    // here uses this to step over one.
    [[nodiscard]] std::size_t size() const noexcept {
      return AngelScript::asBCTypeSize[AngelScript::asBCInfo[opcode()].type];
    }

    [[nodiscard]] const ByteCode* address() const noexcept { return bc_; }

    // Variable arguments. A variable is addressed by a *signed* word that counts `asDWORD`s below the
    // frame pointer, so these are negative for the frame's own locals and positive for the caller's.
    [[nodiscard]] std::int32_t var0() const noexcept { return asBC_SWORDARG0(bc_); }
    [[nodiscard]] std::int32_t var1() const noexcept { return asBC_SWORDARG1(bc_); }
    [[nodiscard]] std::int32_t var2() const noexcept { return asBC_SWORDARG2(bc_); }

    // The same slot read as an *unsigned* word. Only the instructions that use it as a byte offset
    // into the stack take this rather than var0, and the two differ for any value above 32767 - which
    // is exactly the range a large frame's stack offsets reach.
    [[nodiscard]] std::uint32_t word0() const noexcept { return asBC_WORDARG0(bc_); }

    // ...and as a signed one, which is what the instructions that use it as an *offset from a pointer*
    // read. A negative displacement addresses before the pointer, and reading one of those as unsigned
    // turns it into a jump of nearly four gigabytes.
    [[nodiscard]] std::int32_t sword0() const noexcept { return asBC_SWORDARG0(bc_); }

    // The second word in the same role. `LoadRObjR` and `LoadVObjR` name a variable in their first
    // word and a member offset from it in their second, and the two are read differently - which is
    // exactly the distinction `var1` alone would hide.
    [[nodiscard]] std::int32_t sword1() const noexcept { return asBC_SWORDARG1(bc_); }

    // Constant arguments, at the instruction's one fixed argument position.
    [[nodiscard]] asDWORD dword() const noexcept { return asBC_DWORDARG(bc_); }
    [[nodiscard]] asQWORD qword() const noexcept { return asBC_QWORDARG(bc_); }
    [[nodiscard]] asPWORD pointer() const noexcept { return asBC_PTRARG(bc_); }
    [[nodiscard]] std::int32_t integer() const noexcept { return asBC_INTARG(bc_); }
    [[nodiscard]] float single() const noexcept { return asBC_FLOATARG(bc_); }

    // The instruction families that carry three variable words *and* a constant put the constant after
    // those words rather than at the usual position, so the interpreter reads theirs one `asDWORD`
    // further along (see its `asBC_ADDIi`). These name that offset rather than hiding it.
    [[nodiscard]] std::int32_t integerAfterWords() const noexcept { return asBC_INTARG(bc_ + 1); }
    [[nodiscard]] float singleAfterWords() const noexcept { return asBC_FLOATARG(bc_ + 1); }

    // `PTR_DW_ARG` puts the pointer at the usual position and the dword after it.
    [[nodiscard]] asDWORD dwordAfterPointer() const noexcept { return asBC_DWORDARG(bc_ + AS_PTR_SIZE); }

    // `W_DW_DW_ARG` puts two constant `asDWORD`s after the variable word rather than one - the
    // initialisation-list stores name both an offset into their buffer and the value to write there,
    // and `dword()` alone reaches only the first of the two.
    //
    // The offset is one `asDWORD`, not one argument: the first constant sits at `bc_[1]` and therefore
    // the second at `bc_[2]`, which `bc_ + 1` names and `bc_ + 2` overshoots by a whole instruction's
    // worth. Overshooting is not a compile error - it reads the next instruction's own leading word,
    // which is always a plausible-looking small number - so a wrong value here is written into the
    // script's buffer and reported as a crash somewhere else entirely. This is what the interpreter's
    // `asBC_DWORDARG(l_bc+1)` means, read the same way.
    [[nodiscard]] asDWORD dwordAfterDword() const noexcept { return asBC_DWORDARG(bc_ + 1); }

    // The branch displacement, counted in `asDWORD`s from the instruction *after* this one - which is
    // why every use below adds this instruction's own size back on.
    [[nodiscard]] std::int32_t displacement() const noexcept { return asBC_INTARG(bc_); }

    // The same instruction, `dwords` `asDWORD`s further along. Used to reach a branch target.
    [[nodiscard]] Instruction advanced(std::ptrdiff_t dwords) const noexcept { return Instruction(bc_ + dwords); }

  private:
    const ByteCode* bc_;
  };

  // A pointer argument slot, as an lvalue that can be published into. The JIT writes a basic block's
  // address here and the interpreter reads it back with `asBC_PTRARG`; going through this rather than
  // casting at the call site keeps the one place that writes bytecode visible.
  [[nodiscard]] inline AngelScript::asPWORD& PointerArgument(ByteCode* instruction) noexcept {
    return *reinterpret_cast<AngelScript::asPWORD*>(instruction + 1);
  }

} // namespace base::asjit
