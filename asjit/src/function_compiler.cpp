//
// Created by X-ray on 13/09/2026.
//

#include "function_compiler.hpp"

#include <asmjit/core/error_handler.h>
#include <asmjit/x86.h>

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "abi.hpp"
#include "bytecode.hpp"
#include "emitter.hpp"
#include "runtime.hpp"

namespace base::asjit {
  namespace {

    // Sentinel for "this block was reached by a branch rather than by a JitEntry". Only the blocks
    // carrying a real entry offset are ever published; the rest exist so that a branch whose target
    // lies inside the compiled region can stay native instead of returning to the interpreter.
    constexpr std::uint32_t kNoJitEntry = 0xFFFFFFFFu;

    // Sentinel for "no block is emitted after this one", so a block that ends the function can never be
    // mistaken for one with a block to fall into.
    constexpr std::uint32_t kNoFallThrough = 0xFFFFFFFFu;

    struct Block {
      asmjit::Label label;

      // Every asBC_JitEntry whose successor is this block. More than one is possible, and each has to
      // have its own pointer argument filled in, since the VM reads the arg of the entry it stopped at
      // rather than looking the address up by position.
      std::vector<std::uint32_t> jit_entries;

      // Whether this block translated at least one instruction. A block that did not is emitted as a
      // bare exit, so that a branch into it still works, but it is never published - handing the VM a
      // block that translates nothing would cost a call and a return to achieve what the null
      // argument already does for free.
      bool emitted = false;
    };

    // asmjit reports problems by calling out to one of these rather than by returning every error
    // through the call chain, so the handler is where the failure is noticed. Only the first counts:
    // a failed emission is usually followed by more of them.
    class ErrorCollector final : public asmjit::ErrorHandler {
    public:
      void handle_error(asmjit::Error err, const char* message, asmjit::BaseEmitter*) override {
        if (error_ == asmjit::kErrorOk) {
          error_ = err;
          message_ = message != nullptr ? message : "";
        }
      }

      [[nodiscard]] bool ok() const noexcept { return error_ == asmjit::kErrorOk; }

      asmjit::Error error_ = asmjit::kErrorOk;
      std::string message_;
    };

    [[nodiscard]] bool IsConditionalBranch(AngelScript::asEBCInstr opcode) noexcept {
      switch (opcode) {
      case AngelScript::asBC_JZ:
      case AngelScript::asBC_JNZ:
      case AngelScript::asBC_JS:
      case AngelScript::asBC_JNS:
      case AngelScript::asBC_JP:
      case AngelScript::asBC_JNP:
      case AngelScript::asBC_JLowZ:
      case AngelScript::asBC_JLowNZ:
        return true;
      default:
        return false;
      }
    }

    // The relation a conditional branch asks about. What it is asked *of* is the caller's business:
    // the branch wants to know whether that value is below zero, equal to it, or above it, and whether
    // it reads the answer out of memory or out of the flags is a difference in how the answer is
    // reached rather than in what the answer is.
    enum class BranchRelation { Equal, NotEqual, Negative, NotNegative, Positive, NotPositive };

    // The relation `opcode` asks about, for the eight opcodes IsConditionalBranch accepts. The two
    // low-width tests ask the same question as the full-width pair they are named after, and differ
    // only in how much of the value register they read.
    //
    // Which the name does not always give away: asBC_JP and asBC_JNP are the two halves of a signed
    // comparison against zero, not the parity flag their names would otherwise imply.
    [[nodiscard]] BranchRelation RelationTestedBy(AngelScript::asEBCInstr opcode) noexcept {
      switch (opcode) {
      case AngelScript::asBC_JZ:
      case AngelScript::asBC_JLowZ:
        return BranchRelation::Equal;
      case AngelScript::asBC_JNZ:
      case AngelScript::asBC_JLowNZ:
        return BranchRelation::NotEqual;
      case AngelScript::asBC_JS:
        return BranchRelation::Negative;
      case AngelScript::asBC_JNS:
        return BranchRelation::NotNegative;
      case AngelScript::asBC_JP:
        return BranchRelation::Positive;
      case AngelScript::asBC_JNP:
        return BranchRelation::NotPositive;
      default:
        // Never reached: the caller has asked IsConditionalBranch first. The alternative to answering
        // is an emit function that walks away without emitting, which is a branch that does not branch
        // - the one outcome here that is worse than the wrong one.
        return BranchRelation::Equal;
      }
    }

    // The relation each conditional branch actually tests, then the branch itself.
    //
    // Written as a signed comparison rather than as the flag test each relation reads most directly
    // (`js` for Negative, say), because both sources are correct under that reading and only one of
    // them is correct under the other. The register form compares the value register against zero, so
    // the subtraction it performs cannot overflow and its sign flag is its result's sign. The flag
    // form's flags are the ones left by the materialisation's own `sub`, whose operands are the two
    // `setcc` results - 0 and 1 - so that subtraction cannot overflow either, for the same reason and
    // by the same argument.
    //
    // That is the whole of why `jl` is right here, and it is invisible from both call sites: a
    // materialisation that subtracted anything other than a pair of 0/1 values would make these four
    // wrong by the overflow flag while the register form stayed right, and every test that does not
    // produce an overflowed comparison would still pass.
    void EmitRelation(asmjit::x86::Assembler& a, BranchRelation relation, const asmjit::Label& taken) {
      switch (relation) {
      case BranchRelation::Equal:
        a.je(taken);
        return;
      case BranchRelation::NotEqual:
        a.jne(taken);
        return;
      case BranchRelation::Negative:
        a.jl(taken);
        return;
      case BranchRelation::NotNegative:
        a.jge(taken);
        return;
      case BranchRelation::Positive:
        a.jg(taken);
        return;
      case BranchRelation::NotPositive:
        a.jle(taken);
        return;
      }
    }

    // Where a conditional branch reads the value it tests.
    enum class BranchSource {
      // Out of the value register: load it and compare it against zero, which is what the
      // interpreter's handler for every one of these does. This is the form that needs no assumption
      // at all - the register is in memory and its contents are whatever the program last put there,
      // which is the same thing the interpreter would be reading.
      ValueRegister,

      // Out of the flags, where the instruction emitted immediately before this branch is one that
      // reported `Emitter::Outcome::TranslatedWithValueFlags`. That outcome is a promise that the
      // flags describe the value register's contents, which is exactly the comparison this branch would
      // otherwise have done for itself - so the branch is the same test with a load and a compare
      // removed from it.
      //
      // The adjacency is the whole of the correctness argument and it is not a property of the
      // bytecode: it is a property of the two emissions above being consecutive in the code this
      // function is writing. Hence the caller's rule that this is used only for a branch in the same
      // block as the comparison it follows - a block boundary is a place another translation unit, or
      // the interpreter, can arrive at with flags of its own.
      //
      // The byte-wide pair is where the two forms are not literally the same test: the register form
      // asks whether the low *byte* is zero and the flag form asks it of the whole dword. They answer
      // the same because the low byte of -1, of 0 and of 1 is zero exactly when the value is - which is
      // a fact about what the comparisons produce, not about bytes, and is the second reason this source
      // is only ever chosen after one of them.
      Flags,
    };

    // The condition each conditional branch actually tests, then the branch itself.
    void EmitBranch(asmjit::x86::Assembler& a, const Emitter::Abi& abi, AngelScript::asEBCInstr opcode, BranchSource source, const asmjit::Label& taken) {
      if (source == BranchSource::ValueRegister) {
        // The two low-width tests read a byte, the six others the low dword - the same width asBC_NOT
        // reads, and a different one from the pair, which is why those two cannot share a comparison.
        if (opcode == AngelScript::asBC_JLowZ || opcode == AngelScript::asBC_JLowNZ)
          a.cmp(asmjit::x86::byte_ptr(abi.regs, static_cast<std::int32_t>(kOffValueRegister)), 0);
        else
          a.cmp(asmjit::x86::dword_ptr(abi.regs, static_cast<std::int32_t>(kOffValueRegister)), 0);
      }

      EmitRelation(a, RelationTestedBy(opcode), taken);
    }

  } // namespace

  FunctionCompiler::Result FunctionCompiler::Compile(asmjit::JitRuntime& runtime, AngelScript::asIScriptFunction* func,
                                                      const Options& options) {
    Result result;

    if (func == nullptr)
      return result;

    AngelScript::asUINT length_dwords = 0;
    const auto* byte_code = func->GetByteCode(&length_dwords);
    if (byte_code == nullptr || length_dwords == 0)
      return result;

    const auto* blob = reinterpret_cast<const ByteCode*>(byte_code);
    const auto length = static_cast<std::uint32_t>(length_dwords);
    const auto blob_address = reinterpret_cast<std::uintptr_t>(blob);

    // The engine this function belongs to, resolved once and baked as an immediate into every translated
    // call. Not a micro-optimisation: the only route from a context to its engine is the virtual
    // `asIScriptContext::GetEngine`, and a translated `asBC_CALL` is the hot path of a script that calls
    // another script function in a loop, which is the shape a JIT exists for. One virtual call here
    // costs nothing, and it buys the removal of one from every such call for the life of the code.
    //
    // It cannot dangle while the code it is baked into is reachable: an engine owns its modules, and
    // this block is released through `SetJITFunction` as its own function is destroyed, which is either
    // a module discard - the engine is alive - or engine shutdown, where functions are destroyed before
    // the engine is.
    const auto engine_address = reinterpret_cast<std::uintptr_t>(func->GetEngine());

    // ------------------------------------------------------------------------
    // Pass one - find every position a block can begin at
    // ------------------------------------------------------------------------
    //
    // The walk is linear over the whole function rather than a traversal of the control flow graph,
    // because the two answer different questions. Traversal finds the blocks worth compiling;
    // walking finds the blocks that have to *exist*. A branch whose target was never given a block
    // would have to fall back to the interpreter, and since the compiler puts a JitEntry at the top
    // of every loop rather than at every branch target, that would be most backward jumps - the
    // hottest edges in the function. Declaring every branch target a block start instead keeps them
    // native, at the cost of compiling some code the VM can never reach, which is only size.
    using BlockMap = std::map<std::uint32_t, Block>;
    BlockMap blocks;

    bool needs_string_registers = false;

    const auto declare_block = [&blocks, length](std::int64_t offset, std::uint32_t jit_entry) {
      // A target outside the function is not a block, and a JitEntry as the last instruction would
      // put one past the end. Neither is reachable from a well-formed function, but the arithmetic
      // that produces a target is a signed addition on a value read out of the bytecode, and being
      // wrong about it here would mean emitting a label and then jumping to it.
      if (offset < 0 || offset >= static_cast<std::int64_t>(length))
        return;
      auto& block = blocks[static_cast<std::uint32_t>(offset)];
      if (jit_entry != kNoJitEntry)
        block.jit_entries.push_back(jit_entry);
    };

    for (std::uint32_t pc = 0; pc < length;) {
      const Instruction ins(blob + pc);
      const auto size = static_cast<std::uint32_t>(ins.size());
      if (size == 0 || pc + size > length)
        break;

      const auto next = static_cast<std::int64_t>(pc) + static_cast<std::int64_t>(size);

      ++result.instructions_total;
      ++result.opcodes_walked[static_cast<std::uint8_t>(ins.opcode())];

      // Whether the prologue has to park rdi and rsi, which it needs to do for exactly one opcode.
      // `asBC_COPY` is the only translation that uses the string instructions, and those take their
      // operands in rdi and rsi on this platform - so a function with no COPY in it never touches
      // either register and has no reason to spend four instructions of every hand-off saving them.
      //
      // The test is "does the function contain a COPY anywhere", not "does a COPY end up inside a
      // compiled block", because the prologue is emitted before either is known. That over-saves in
      // the case of a COPY that translation stops short of, which costs the same four instructions
      // this exists to remove - and it cannot under-save, which would be a caller's live value
      // silently overwritten by half a copied object. See `emit_exit` for the other half of this.
      if (ins.opcode() == AngelScript::asBC_COPY)
        needs_string_registers = true;

      if (ins.opcode() == AngelScript::asBC_JitEntry) {
        // The block begins *after* the entry, not at it. The interpreter resumes by dispatching at
        // whatever the JIT wrote back, so the entry instruction itself is never executed natively.
        declare_block(next, pc);
      } else if (ins.opcode() == AngelScript::asBC_JMP || IsConditionalBranch(ins.opcode())) {
        // The displacement is relative to the end of the instruction, which is the same rule the
        // interpreter applies when it adds the instruction's own size back on.
        declare_block(next + ins.displacement(), kNoJitEntry);
        if (ins.opcode() != AngelScript::asBC_JMP)
          declare_block(next, kNoJitEntry);
      }

      pc = static_cast<std::uint32_t>(next);
    }

    if (blocks.empty())
      return result;

    // ------------------------------------------------------------------------
    // Pass two - emit
    // ------------------------------------------------------------------------
    asmjit::CodeHolder code;
    code.init(runtime.environment(), runtime.cpu_features());

    ErrorCollector errors;
    code.set_error_handler(&errors);

    asmjit::x86::Assembler a(&code);

    // Every block needs a real label before anything can jump to one. A default-constructed
    // asmjit::Label is invalid, and asmjit only notices when the code is finalized - it reports
    // InvalidLabel for the whole function rather than for the jump that named one, which says nothing
    // about where the mistake is. So they are all created here, in one place, rather than at
    // whichever point a block first happens to be bound or jumped to.
    for (auto& [offset, block] : blocks) {
      static_cast<void>(offset);
      block.label = a.new_label();
    }

    const Emitter::Abi abi{reg::kRegs, reg::kStackPointer, reg::kFramePointer};
    Emitter emitter(a, abi);

    // The prologue. The VM enters here with the register block in rcx and the block to run in rdx,
    // and `programPointer` left pointing at the asBC_JitEntry rather than at anything executable -
    // so the first thing that has to happen is the dispatch the interpreter would otherwise do.
    //
    // Three registers are parked here unconditionally, and every one of them has to be: all three are
    // callee-saved under the x64 convention, so a fallback that calls back into the interpreter and
    // from there into a script binding cannot have them clobbered underneath us, and anything this
    // code clobbers the caller of the compiled function was entitled to keep.
    //
    // rdi and rsi are in that set on Windows and not on SysV, which is the trap: they are argument
    // registers there and scratch here. Twenty years of x86-64 code assumes the opposite, and the
    // failure mode is a caller's live value silently replaced by half a copied object. They are
    // pushed only for a function that contains a COPY, because that is the only translation that
    // clobbers them - so a function without one saves four instructions on every single hand-off.
    //
    // The push order is reversed at every exit, and the *condition* here is the one in `emit_exit`:
    // pushing without popping corrupts the return address, popping without pushing corrupts two of
    // the caller's registers, and both are silent. They are two statements of one decision, so a
    // change to either has to be a change to both.
    //
    // The count is odd either way, which is what keeps the stack aligned. The VM calls in with rsp
    // eight bytes off a sixteen-byte boundary - the return address - and every push flips that by
    // eight, so three pushes and five both land it on a boundary and the `fmod` call the float
    // remainder translation makes is entered the way its own ABI requires. A fourth unconditional
    // push would break that, and the failure would be in the callee's vector code rather than here.
    a.push(asmjit::x86::rbx);
    a.push(asmjit::x86::r12);
    a.push(asmjit::x86::r13);
    if (needs_string_registers) {
      a.push(asmjit::x86::rdi);
      a.push(asmjit::x86::rsi);
    }
    a.mov(reg::kRegs, kArgRegs);
    a.mov(reg::kStackPointer, asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackPointer)));
    a.mov(reg::kFramePointer, asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackFramePointer)));
    a.jmp(kArgJitEntry);

    // Every instruction that ends up inside a block of native code goes through here, so that the
    // total and the per-opcode histogram cannot disagree about what was translated.
    const auto count_translated = [&result](AngelScript::asEBCInstr opcode) {
      ++result.instructions_translated;
      ++result.opcodes_translated[static_cast<std::uint8_t>(opcode)];
    };

    // Returns to the interpreter at `resume_dwords`, publishing the registers the blocks have been
    // keeping natively. This is both the exit taken at the end of a translated run and the exit taken
    // wherever translation stops, which is why it takes the resume point as an argument rather than
    // being a single fixed epilogue.
    // Gives back the three registers the prologue parked and returns. Every exit ends here, whether it
    // published the registers first or not, so that the two lists cannot drift apart - pushing without
    // popping corrupts the return address and popping without pushing corrupts two of the caller's
    // registers, and both are silent.
    const auto emit_epilogue = [&]() {
      if (needs_string_registers) {
        a.pop(asmjit::x86::rsi);
        a.pop(asmjit::x86::rdi);
      }
      a.pop(asmjit::x86::r13);
      a.pop(asmjit::x86::r12);
      a.pop(asmjit::x86::rbx);
      a.ret();
    };

    const auto emit_exit = [&](std::int64_t resume_dwords) {
      const auto resume = blob_address + static_cast<std::uintptr_t>(resume_dwords) * sizeof(ByteCode);
      a.mov(asmjit::x86::rax, asmjit::Imm(static_cast<std::uint64_t>(resume)));
      a.mov(asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffProgramPointer)), asmjit::x86::rax);
      a.mov(asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackPointer)), reg::kStackPointer);
      // Two of the three fields are published and the third is not, and the asymmetry is the whole
      // point rather than an oversight.
      //
      // Nothing assigns the frame pointer. The prologue loads it from the register block at every
      // entry and no translation writes it - neither in this file, where it appears only as a base for
      // a frame-relative operand, nor in the emitter, which reads it the same way. So within an entry
      // the native copy and the field cannot disagree, and storing it here would write back the value
      // that is already there: a store and four bytes of encoding at every exit, on a path a hand-off
      // spends a large share of its instructions in. Measured over the benchmark, dropping it takes
      // 12228 bytes off the 82 compiled functions and leaves every one of them smaller.
      //
      // The stack pointer is the case this reasoning does not cover: the emitter moves it in fourteen
      // places, and those moves are in registers with the exit being what carries them back out. It
      // stays, and so does the copy in `publish_registers` below, for the same reason.
      emit_epilogue();
    };

    // The block that will be emitted immediately after the one being emitted, or kNoFallThrough when
    // this is the last one. Set at the top of each block below.
    //
    // Blocks are emitted in bytecode order, so "the next block" is the next one in that order and its
    // body is what this block's code is followed by in the finished function. A transfer to it is
    // therefore already written: the jump would land on the instruction that follows it, so it is
    // dropped and the block falls through instead. Every block in a loop ends this way, and the branch
    // is neither free nor neutral - it is a taken jump into the next instruction, which the branch
    // predictor handles well and the front end still pays for on every pass through the loop.
    std::uint32_t fall_through = kNoFallThrough;

    // Leaves the block for `target_dwords`, natively if a block was declared there and through the
    // interpreter if not. The lookup being by bytecode offset rather than by label is what keeps the
    // two representations from having to be kept in step.
    //
    // `ends_block` says this is the last thing emitted in the block. That is the caller's to know and
    // not something this can see, and it is what makes the fall-through case sound: a transfer with
    // more code after it cannot be dropped, because the code after it is what an omitted jump would
    // run into - which is how the not-taken half of a conditional branch whose taken half is an exit
    // stays a jump even when its target is the next block.
    const auto emit_transfer = [&](std::int64_t target_dwords, bool ends_block) {
      const auto it = target_dwords >= 0 ? blocks.find(static_cast<std::uint32_t>(target_dwords)) : blocks.end();
      if (it == blocks.end()) {
        emit_exit(target_dwords);
        return;
      }

      if (ends_block && static_cast<std::uint32_t>(target_dwords) == fall_through)
        return;

      a.jmp(it->second.label);
    };

    // Publishes the fields of the register block that anything reached from inside a block reads:
    // where execution is, and where the stack is. Compiled code keeps the stack pointer in a register,
    // so engine code entered mid-block would otherwise find the value it was left with however many
    // instructions ago - which is what the interpreter avoids by writing it before every instruction
    // that can re-enter it.
    //
    // `programPointer` is set to the instruction being executed rather than to the next one, because
    // that is what the engine computes a line number from when something inside the call reports an
    // error, and what it would resume at if it had to.
    const auto publish_registers = [&](std::int64_t at_dwords) {
      const auto instruction = blob_address + static_cast<std::uintptr_t>(at_dwords) * sizeof(ByteCode);
      a.mov(asmjit::x86::rax, asmjit::Imm(static_cast<std::uint64_t>(instruction)));
      a.mov(asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffProgramPointer)), asmjit::x86::rax);
      a.mov(asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackPointer)), reg::kStackPointer);
      // The frame pointer is not among them, for the reason `emit_exit` gives: nothing assigns it, so
      // the field already holds what a store here would put in it.
    };

    // Calls one of the helpers in runtime.cpp by address. asmjit names a function outside the code it
    // is building by loading its address, and every address taken here belongs to this same library,
    // so there is nothing to relocate and nothing to keep in step with a symbol table.
    //
    // The thirty-two bytes reserved around the call are the four register-argument home slots this
    // platform's calling convention hands the callee - it owns `[rsp]` through `[rsp+32]` from the
    // moment it is entered and may spill its arguments there whenever it likes, without using any
    // space of its own. A call site that does not reserve them is not merely unconventional: the
    // callee who chooses to spill writes over whatever the caller kept *below* its stack pointer, and
    // what this caller keeps there is the three registers its prologue pushed and the return address
    // it was given. That is the caller's frame, so the failure is not in the helper - it is in this
    // function's own epilogue restoring registers out of slots the helper used as scratch, and in
    // returning to an address the helper overwrote.
    //
    // It was invisible for as long as every helper happened to be simple enough that the compiler had
    // no reason to write the home slots at all - which made it a property of the *helpers* rather than
    // of this code, and therefore a bug waiting for the first helper to be compiled differently. The
    // two ways it stops being invisible are worth naming, because both are ordinary: giving a helper
    // enough parameters and enough work that spilling becomes the cheapest thing to do, and calling
    // one that takes variadic arguments, whose prologue writes all four slots and the vector registers
    // behind them unconditionally. `emitter.cpp` reserves the same thirty-two bytes at every call it
    // emits, so this is that convention applied where the other half of the translator did not apply
    // it. The reservation keeps the sixteen-byte alignment the prologue's comment is about, since
    // thirty-two is a multiple of sixteen.
    const auto call_native = [&](std::uintptr_t target) {
      a.mov(asmjit::x86::rax, asmjit::Imm(static_cast<std::uint64_t>(target)));
      a.sub(asmjit::x86::rsp, 32);
      a.call(asmjit::x86::rax);
      a.add(asmjit::x86::rsp, 32);
    };

    // Ends the block at `resume_dwords` if the engine code the instruction just ran left the context
    // with something to service, and carries straight on if it did not.
    //
    // The test is `doProcessSuspend`, which is a *superset* of what the interpreter checks after such
    // an instruction: every path that suspends a script or raises an exception from it sets this same
    // field on the way out, so testing it alone cannot miss either. Bailing more often than the
    // interpreter would is safe, and is why the extra margin is worth taking - the block ends where
    // the interpreter might have stopped anyway, rather than running on past a suspension a watchdog
    // is waiting for.
    //
    // The resume point is whichever offset the caller names, and the callers name it for two opposite
    // reasons. Every instruction that bails because the engine code it just ran left something to
    // service has already had its full effect - the call returned, the handle was assigned, the object
    // was released - so it names the instruction *after* it, since resuming at the instruction itself
    // would run it a second time. A suspend is the other way round: what has to run is the interpreter's
    // own handler, and the interpreter reaches it by executing the instruction, so a suspend names
    // itself.
    //
    // The exit goes out of line rather than after the branch, which is what `doProcessSuspend` being
    // false nearly always means the check is paying for: the working case is the fall-through, and the
    // exit is the rare one. Written in place the check has to be a taken branch over the exit - and
    // the exit is nine instructions, most of them a sixty-four-bit address - so every statement in a
    // loop carries its own exit in the middle of the loop's straight-line code, jumped over but still
    // there, and the loop's hot bytes are spread across cache lines they do not need. Written this way
    // the common case is a not-taken branch with nothing after it, and the exit is one branch away
    // with every other exit in the function.
    //
    // The two halves of that are one decision written in two places: the check below branches to a
    // label and stops there, and the labels it collected are bound to their exits after the last
    // block. What those exits publish is the offset to *resume* at rather than one to run again, which
    // is the one difference between these and the fault exits - the instruction that bailed has
    // already had its effect, so the interpreter must not see it twice.
    std::vector<std::pair<asmjit::Label, std::uint32_t>> suspend_stubs;

    const auto bail_if_suspend = [&](std::int64_t resume_dwords) {
      asmjit::Label quit = a.new_label();
      a.cmp(asmjit::x86::byte_ptr(reg::kRegs, static_cast<std::int32_t>(kOffDoProcessSuspend)), 0);
      a.jne(quit);
      suspend_stubs.emplace_back(quit, static_cast<std::uint32_t>(resume_dwords));
    };

    // Every instruction that compiled a guard collects its exit here instead of emitting it in place,
    // so that the exit sits outside the straight-line code and costs the working case nothing. Each
    // entry is a label the guard branches to and the bytecode offset whose address that exit is to
    // publish - the faulting instruction's own, since the interpreter will run it again.
    std::vector<std::pair<asmjit::Label, std::uint32_t>> fault_stubs;

    for (auto& [offset, block] : blocks) {
      a.bind(block.label);

      // The block whose code will follow this one's, which is the next in bytecode order - see
      // `emit_transfer`. Asked of the map rather than tracked, because the map is what fixes the order
      // the loop below emits in.
      const auto following = blocks.upper_bound(offset);
      fall_through = following != blocks.end() ? following->first : kNoFallThrough;

      std::uint32_t pc = offset;
      bool terminated = false;

      // Whether the instruction emitted immediately before the one being looked at is a comparison
      // that reported `Emitter::Outcome::TranslatedWithValueFlags`, which is what lets a conditional
      // branch test the flags instead of loading the value register back and comparing it against zero.
      //
      // It has to live outside the loop below, and the form of this that does not is worth spelling
      // out because it is silent. A variable declared inside the body is re-initialised before every
      // instruction, so the branch - which is the instruction *after* the comparison - reads the value
      // its own iteration just gave it and never the comparison's. That version compiles, translates
      // every comparison correctly, passes the whole suite, and folds nothing: the flags form is simply
      // never chosen. The two lines at the top of the body are what make this one mean what it says.
      bool value_flags_live = false;

      while (pc < length) {
        // What the previous iteration left, and a clean slate for this one: an instruction that does
        // not reach the emission at the bottom of this body - one of the special cases, a block
        // boundary, one that emits no code at all - therefore leaves `value_flags_live` false, which is
        // the answer that is always safe.
        const bool previous_left_value_flags = value_flags_live;
        value_flags_live = false;

        // A declared block start is where this block ends. Reaching one means control continues
        // somewhere the VM may also enter directly, so it gets its own code rather than being folded
        // in - folding would duplicate it once per predecessor.
        if (pc != offset && blocks.find(pc) != blocks.end()) {
          // Not `a.jmp` directly: this is the commonest block ending there is - it is reached by every
          // block that runs into the next declared start, which in a loop is all of them - and the
          // block it transfers to is very often the one emitted next.
          emit_transfer(pc, true);
          terminated = true;
          break;
        }

        const Instruction ins(blob + pc);
        const auto size = static_cast<std::uint32_t>(ins.size());
        if (size == 0 || pc + size > length)
          break;

        const auto next = static_cast<std::int64_t>(pc) + static_cast<std::int64_t>(size);


        if (ins.opcode() == AngelScript::asBC_RET) {
          // The return, and the instruction the SDK's own JIT guide names directly: "Some byte code
          // instructions are not meant to be converted into native code. These are usually the ones
          // that have a more global effect on the VM, e.g. the instructions that setup a call to a new
          // script function, or that return from a previous instruction." This is the second clause.
          //
          // **This instruction was translated, and then taken back out, and the numbers are the whole
          // reason.** The guide's advice is about the *frame*, and the frame is not what stopped it:
          // the engine's own `asCContext::PopCallState` pops it, reached the way the other internal
          // entry points in `runtime.cpp` are, and the version that did exactly that agreed with the
          // interpreter on every test in this suite - including a recursion four frames deep and the
          // outermost frame, which are the two exits the handler has. What stopped it is the
          // *predicate* in front of the pop.
          //
          // Whether a frame may be popped is a property of who called the function, and the only
          // published way to ask is `asIScriptContext::GetCallStateRegisters` at stack level 1: a
          // call whose body serialises the frame pointer and the stack pointer unconditionally before
          // it answers, and whose answer is the saved frame pointer being null. The interpreter reads
          // that same element out of `m_callStack` and pays a load and a compare for it. So the
          // translation replaces a cheap dispatch with an expensive call on *every* return - and a
          // return is what a call-heavy script is mostly made of.
          //
          // The call is direct rather than through the vtable - it is declared on the shadow
          // `asCContext` in `runtime.cpp` and invoked with explicit qualification, and the linker
          // error that settled how it had to be declared is recorded there - and that was tried
          // against the hypothesis that the indirection was the cost. It is not: the body is, and the
          // table below is what a direct call produces.
          //
          // Measured by building the library with this case refusing instead, and running the
          // benchmark's two arms in one process, with the interpreted column as the control: no change
          // to this instruction can touch it, and across the table it moved by a median of 0.3% and by
          // no more than 4% in either direction. `speedup` is interpreted time over compiled time, so
          // a larger number is better and the middle column is the one to want.
          //
          //   row                    interp control   translated   refused
          //   delegate_call                    0.98x        0.65x     1.23x
          //   many_short_ticks                 1.00x        0.64x     0.95x
          //   script_calls                     1.01x        0.74x     1.06x
          //   recursion                        0.99x        0.83x     1.21x
          //   member_access                    0.99x        1.03x     1.41x
          //   switch_heavy                     1.03x        1.22x     1.59x
          //   handle_guards                    0.99x        1.25x     1.67x
          //
          // Not one row improved past the control's own spread, and the rows that lose are the ones
          // with a return per few instructions of work - the shape this component exists for. Two of
          // them were *slower than the interpreter* with the translation in. The structural counters
          // say the loss is not an artifact of code layout: the translated build emits one to three
          // more blocks and five to ten percent more code, which is what translating one more
          // instruction costs and nothing beyond it.
          //
          // What would make it worth doing is a cheaper predicate, and there is not one. Each cheap
          // half is wrong on its own: `GetCallstackSize() <= 1` identifies the outermost frame, but it
          // says nothing about a *pushed state*, whose frame marker is a null frame pointer with a
          // whole call stack behind it - and that case is reached by `PushState` plus a nested
          // `Execute`, so a script handing control to itself through a system function would have that
          // return pop a marker instead of ending the execution. `IsNested` walks the call stack, so it
          // costs more than the call it would replace. The fields themselves are private, and reading
          // them out of a layout reproduced by hand is the silent failure this component's declaration
          // rule exists to avoid.
          //
          // So the block ends here and the interpreter runs the return. It costs one dispatch, on a
          // path that is otherwise as cheap as it can be: the call that led here is translated and the
          // caller resumes compiled, so what is left for the interpreter is this instruction and
          // nothing around it.
          //
          // **And it is translated anyway, behind `Options::translate_returns`, which is off.** The
          // two paragraphs above are the measurement that sets that default and this is the code that
          // makes it a default rather than a limitation: the instruction is not refused because it
          // cannot be written, and a future engine version that moves the numbers should be a flag to
          // flip rather than a translation to write again. The differential suite runs this
          // configuration too, so what is behind the flag is verified rather than merely present.
          //
          // The suite now takes the measurement rather than quoting it, and the re-taken numbers say
          // the same thing with one row added. The `fullcov` benchmark variant is this configuration;
          // six rows that reach the instruction opt into it, and each is the same script and the same
          // interpreted control as its `plain` twin. Interpreted time over compiled time, so larger is
          // better:
          //
          //   row                    plain    fullcov     coverage plain -> fullcov
          //   native_calls           1.38x      1.41x     85.7% -> 91.4%
          //   script_calls           0.99x      0.74x     82.3% -> 88.7%
          //   array_index            1.35x      0.61x     81.7% -> 92.7%
          //   recursion              1.28x      0.92x     87.8% -> 91.9%
          //   many_short_ticks       0.97x      0.74x     84.1% -> 89.9%
          //   delegate_call          1.18x      0.93x     83.7% -> 89.5%
          //
          // Five of the six lose, and the one that does not is inside the spread. Coverage rises on
          // every row, by five to eleven points, and the ratio falls - which is the same shape the
          // first measurement had and the reason the coverage column exists at all. The `array_index`
          // pair is the cleanest of them only because that row also carries `Thiscall1`, and the two
          // instructions are turned on together by that variant; the rows that carry only a return are
          // the ones to read for this instruction, and they lose by between a quarter and a third.
          //
          // The shape is the one the two exits call for. `ReturnFromScriptFunction` asks the engine's
          // own predicate and then pops the engine's own frame; on a true answer the registers
          // describe the *caller*, so the exit is bare, for the reason a translated call's is. On a
          // false answer nothing was touched, so the exit publishes this instruction and the
          // interpreter re-runs it - which is the outermost return, the one that ends the execution.
          // The interpreter's own handler does exactly this: it sets `asEXECUTION_FINISHED` and
          // returns, and the VM stops before dispatching anything else.
          if (options.translate_returns) {
            a.mov(asmjit::x86::rcx, reg::kRegs);
            a.mov(asmjit::x86::edx, asmjit::Imm(ins.sword0()));
            call_native(reinterpret_cast<std::uintptr_t>(&runtime::ReturnFromScriptFunction));

            const auto hand_back = a.new_label();
            a.test(asmjit::x86::al, asmjit::x86::al);
            a.je(hand_back);

            emit_epilogue();

            a.bind(hand_back);
            emit_exit(pc);

            block.emitted = true;
            count_translated(ins.opcode());
            terminated = true;
            break;
          }

          ++result.opcodes_unimplemented[static_cast<std::uint8_t>(ins.opcode())];
          emit_exit(pc);
          terminated = true;
          break;
        }

        if (ins.opcode() == AngelScript::asBC_CALL || ins.opcode() == AngelScript::asBC_CALLINTF) {
          // A call into another script function, which is the one instruction the SDK's own JIT guide
          // names as not meant to be converted: "the instructions that setup a call to a new script
          // function ... When these functions are encountered, the JIT function should return the
          // control to the VM, and then the VM will execute the instruction."
          //
          // The reasoning behind that is sound and is honoured here - the frame is not built by this
          // code. What the guide is describing is the *instruction*, and the advice to hand back exists
          // because a hand-back is cheap: the compiler places a `JitEntry` after every call, so the VM
          // is going to dispatch into compiled code again a few instructions later either way.
          //
          // What this translation removes is not the frame, it is the detour. Handing back leaves the
          // block, and the VM then walks: dispatch the `asBC_CALL` handler, watch it store three
          // registers and call the engine routine that builds the frame, load the three back, dispatch
          // again - on to the callee's `asBC_JitEntry`, which publishes the three a *third* time before
          // the callee's compiled code runs. Reaching the same engine routine from inside the block
          // skips the two dispatches and two of the three publication rounds, and the callee still runs
          // natively because its own entry is where the VM resumes.
          //
          // The routine reached is `asCContext::CallScriptFunction`, which pushes the call state, makes
          // the callee current, reserves and initialises its stack and calls its line callback - the
          // VM's own stack discipline, untouched. There is no suspend test after it, unlike the native
          // call above: the exit here is unconditional, and the VM checks the execution status itself
          // before it resumes.
          //
          // `asBC_CALLINTF` is the same instruction reached through an interface, and it shares this
          // case because it shares every step of it: the interpreter's two handlers differ only in the
          // routine the resolved function is handed to, and both handlers publish, call and re-read the
          // registers in the same order. What differs is inside the engine routine - one makes the
          // callee current, the other resolves the implementation off the object's type first and can
          // find the object null - and neither is visible from here.
          //
          // That it *can* fail is worth naming, because the failure does not install a frame and this
          // exit does not publish one. It does not need to: the engine raises the exception and leaves
          // the registers describing this frame at the instruction after the call, and the exit returns
          // to the `asBC_JitEntry` that entered the block, whose handler checks the status before it
          // dispatches anything else. So a method called on a null handle unwinds exactly as it does
          // interpreted, by the VM's own check rather than by one added here.
          //
          // The published `programPointer` is the instruction *after* the call, because that is what
          // the engine stores as this frame's resume point when it pushes the call state. And the exit
          // is a bare epilogue rather than `emit_exit` for the reason that makes this instruction
          // different from every other: after the call the registers describe the *callee*, so
          // publishing this frame's pointers over them would throw the new frame away.
          publish_registers(next);

          a.mov(asmjit::x86::rcx, reg::kRegs);
          a.mov(asmjit::x86::rdx, asmjit::Imm(static_cast<std::uint64_t>(engine_address)));
          a.mov(asmjit::x86::r8d, asmjit::Imm(static_cast<std::int32_t>(ins.integer())));
          call_native(reinterpret_cast<std::uintptr_t>(
              ins.opcode() == AngelScript::asBC_CALLINTF ? &runtime::EnterInterfaceMethod
                                                         : &runtime::EnterScriptFunction));

          emit_epilogue();

          block.emitted = true;
          count_translated(ins.opcode());
          terminated = true;
          break;
        }

        if (ins.opcode() == AngelScript::asBC_CallPtr) {
          // The call a funcdef variable makes, and the only call in the language whose target is a
          // value rather than a constant baked into the instruction. That is the whole of its shape:
          // the pointer is a local, the interpreter reads it out of the frame and dispatches on what
          // kind of function it turned out to be.
          //
          // Two of those kinds are handled by the helper and the rest are refused, and the refusal is
          // what the branch below is for. `runtime::EnterFunctionPointer` reports one of the kinds it
          // does not dispatch - an unbound variable, an imported function - by returning through this
          // path untouched, and the instruction is then handed back to the interpreter at its own
          // address, which re-runs it and reaches the outcome this does not. Nothing is half done on
          // that path, so re-running it is exact rather than merely safe, and the two exits are one
          // compare apart because neither can be chosen at translation time: the kind is a value in
          // the script.
          //
          // The published `programPointer` is this instruction, not the one after it, which is where
          // this differs from the two call translations above. The interpreter publishes the same
          // value - its handler stores the registers before it looks at the callee and advances the
          // context's copy only once a branch has been taken - and the helper advances it there, on
          // the far side of the decision, so that the hand-back cannot leave a resume point behind.
          publish_registers(pc);

          a.mov(asmjit::x86::rcx, reg::kRegs);
          a.mov(asmjit::x86::edx, asmjit::Imm(ins.sword0()));
          call_native(reinterpret_cast<std::uintptr_t>(&runtime::EnterFunctionPointer));

          // The callee is current on the success path, so this exit publishes nothing, for the reason
          // the call translations give: the registers now describe a frame that is not this one.
          const auto hand_back = a.new_label();
          a.test(asmjit::x86::al, asmjit::x86::al);
          a.je(hand_back);

          emit_epilogue();

          a.bind(hand_back);
          emit_exit(pc);

          block.emitted = true;
          count_translated(ins.opcode());
          terminated = true;
          break;
        }

        // `asBC_Thiscall1` joins this case behind `Options::translate_object_method_calls`, off by
        // default, because the translation *is* this one: the instruction is a faster spelling of
        // `asBC_CALLSYS` for one signature - a method on an object type returning a reference and
        // taking one four-byte integer - and the interpreter's handler differs from the CALLSYS
        // handler only in the call it makes, with an identical suspend-and-exception tail after it.
        // The fast spelling reaches `asCScriptEngine::CallObjectMethodRetPtr`, which takes no context
        // and therefore cannot set `m_callingSystemFunction`; that field is what `SetException` gates
        // a raise on, so a translation reaching that routine directly runs methods whose failures are
        // silently dropped. `CallSystemFunction` is the one faithful route - it reproduces the fast
        // path's null check, its exception and its value-register write, and sets the field itself -
        // and it is exactly the marshaller the instruction exists to avoid paying for. The emitter's
        // `asBC_Thiscall1` paragraph has the measurements; they are what the default is set from.
        if (ins.opcode() == AngelScript::asBC_CALLSYS ||
            (options.translate_object_method_calls && ins.opcode() == AngelScript::asBC_Thiscall1)) {
          // A call to a function registered by the application - every native, every method on a
          // registered type, every `log::` and every `array` operation a menu script makes. The SDK's
          // own JIT guide lists the instructions that set up a call among those "not meant to be
          // converted into native code", and that is right about the call itself: getting from the
          // argument block to the platform's calling convention is `CallSystemFunction` and, under it,
          // an assembly-level marshaller per architecture. Reproducing that would be writing the part
          // of the engine that is deliberately not portable, and getting it subtly wrong for one of
          // six thousand registered functions would be a wrong script result rather than a crash.
          //
          // So the call is not translated, and this is not that. What is translated is the *dispatch*
          // around it, which is what the guide's advice costs and what the shape of a compiled
          // function is decided by: without this, every call ends the block, and a realistic menu
          // script - a tick of native calls with a line of arithmetic between them - compiles into one
          // tiny block per native, each paying a prologue, an epilogue and two trips through the
          // interpreter's dispatch to do almost nothing. With it, the run stays native across the
          // call and only breaks where the interpreter might have stopped anyway.
          //
          // The three fields published first are what the engine reads rather than what it writes:
          // the arguments come off `stackPointer`, and `programPointer` is what an exception raised
          // inside the call uses to find the line it was raised on - which is why it points at this
          // instruction and not at the next one, exactly as the interpreter sets it. The frame
          // pointer is published for the same reason the interpreter publishes it on every
          // instruction that can re-enter the engine: the debug interface may read it.
          //
          // The break afterwards is `doProcessSuspend`, and it is worth being clear that it is a
          // *superset* of what the interpreter tests here. The interpreter only acts on this point
          // when a line callback is installed or a suspension is pending, and then only to check for a
          // suspension or an exception - but every path that raises either of those sets this same
          // field on its way out, including `SetInternalException` for a script exception raised from
          // inside the native, so testing it alone cannot miss one. The cost of the extra margin is
          // that a block ends at a call in the line-callback configuration, where it would have ended
          // at the next statement anyway.
          publish_registers(pc);

          // The two arguments are the register block and the id the instruction carries.
          a.mov(asmjit::x86::rcx, reg::kRegs);
          a.mov(asmjit::x86::edx, asmjit::Imm(static_cast<std::int32_t>(ins.integer())));
          call_native(reinterpret_cast<std::uintptr_t>(&runtime::InvokeSystemFunction));

          // The helper advanced the published stack pointer by the size the arguments occupied; the
          // native copy is reloaded from it, since the block goes on using that one.
          a.mov(reg::kStackPointer, asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackPointer)));

          bail_if_suspend(next);

          block.emitted = true;
          count_translated(ins.opcode());
          pc = static_cast<std::uint32_t>(next);
          continue;
        }

        if (ins.opcode() == AngelScript::asBC_ALLOC) {
          // Object construction, and half of it is translated here. The instruction has two branches
          // that share nothing but the allocator: a script class is allocated, pre-initialised and then
          // has its *constructor* entered as a script function, which is a call like any other; a value
          // type is allocated, optionally has a registered constructor called on it as a system
          // function, and then has its address written into the variable the caller named.
          //
          // The first is the call shape for the third time, and it ends the block: the constructor's
          // frame is what the registers describe afterwards. The second does not - a registered
          // constructor is entered and left the way `asBC_CALLSYS` enters anything, so the run of
          // native code survives it.
          //
          // That second branch was refused for a while on the grounds that its tail could not be
          // written. The tail reads: if `doProcessSuspend` is set, suspend - unless the constructor
          // raised, in which case free the memory and null the variable instead. The first of those two
          // tests is against `asCContext::m_doSuspend`, private with no accessor, and the refusal
          // treated the pair as inseparable. It is not: the exception is published, as
          // `asIScriptContext::GetState`, and it is the half that does something irreversible. Freeing
          // an object the constructor never finished building is a wrong result; leaving a pending
          // suspension for the next instruction that checks one is a delay, and is the margin this
          // component already takes at every call. So the exception is handled exactly and the
          // suspension is left to the VM, which is where it was always going to be taken.
          const auto* object_type = reinterpret_cast<const AngelScript::asITypeInfo*>(ins.pointer());
          if (object_type != nullptr && (object_type->GetFlags() & AngelScript::asOBJ_SCRIPT_OBJECT) != 0) {
            // The instruction's own address, not the next one: the helper advances the context past the
            // allocation before it builds the constructor's frame, and that advanced value is what the
            // frame records as this one's resume point. Publishing it here and advancing it there is how
            // the interpreter's own handler is divided between the two.
            publish_registers(pc);

            a.mov(asmjit::x86::rcx, reg::kRegs);
            a.mov(asmjit::x86::rdx, asmjit::Imm(static_cast<std::uint64_t>(engine_address)));
            a.mov(asmjit::x86::r8, asmjit::Imm(static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(object_type))));
            a.mov(asmjit::x86::r9d, asmjit::Imm(static_cast<std::int32_t>(ins.dwordAfterPointer())));
            call_native(reinterpret_cast<std::uintptr_t>(&runtime::AllocateScriptObject));

            emit_epilogue();

            block.emitted = true;
            count_translated(ins.opcode());
            terminated = true;
            break;
          }

          if (object_type != nullptr) {
            // The same four arguments as the branch above, and the same first publication for the same
            // reason: the helper advances the context's program pointer past the instruction, and that
            // is the pointer every exit from here publishes.
            publish_registers(pc);

            a.mov(asmjit::x86::rcx, reg::kRegs);
            a.mov(asmjit::x86::rdx, asmjit::Imm(static_cast<std::uint64_t>(engine_address)));
            a.mov(asmjit::x86::r8, asmjit::Imm(static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(object_type))));
            a.mov(asmjit::x86::r9d, asmjit::Imm(static_cast<std::int32_t>(ins.dwordAfterPointer())));
            call_native(reinterpret_cast<std::uintptr_t>(&runtime::ConstructValueObject));

            // The helper pushed the object's address for the constructor to find as its `this` and
            // popped it again with the arguments, then popped the caller's destination slot: three
            // movements the native copy of the stack pointer is out of step with until it is reloaded.
            // The block goes on using the native copy, so that is the one that has to be corrected.
            a.mov(reg::kStackPointer, asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackPointer)));

            // One test covers both of the interpreter's, exactly as it does after a call: the
            // suspension this cannot see is the VM's to take at the next point it checks for one, and
            // the exception it can see has already had its memory freed by the helper - all that is
            // left of it is to stop here.
            bail_if_suspend(next);

            block.emitted = true;
            count_translated(ins.opcode());
            pc = static_cast<std::uint32_t>(next);
            continue;
          }
        }

        // The three instructions that manage an object's lifetime: the two that re-point a handle and
        // update the reference counts on both sides, and the one that releases it.
        //
        // They are translated because the engine publishes the routines they are made of rather than
        // only the bytecode that names them. `asIScriptEngine::ReleaseScriptObject` and
        // `AddRefScriptObject` are the exact two calls the interpreter's own handlers make - release
        // through the type's release behaviour, or the destructor and then the allocator's free - so
        // what is written out below is the addressing and the guards around them, and what is reused
        // is everything that knows how an object is laid out.
        //
        // That distinction is the whole reason these are in reach and the call instructions are not.
        // The behaviour table saying which function releases a given type is internal, and walking it
        // from here would mean a private header and a layout to track across SDK versions; asking the
        // engine to release an object needs neither, and is less code than the interpreter's handler
        // because the engine's entry points already contain its null and behaviour-presence tests. The
        // one thing that does become visible is the type's flags, which are published - so the
        // question the interpreter asks at run time, whether this type counts references at all, is
        // answered once, here, at compile time. A type that does not is a plain store.
        if (ins.opcode() == AngelScript::asBC_FREE) {
          const auto* object_type = reinterpret_cast<const AngelScript::asITypeInfo*>(ins.pointer());
          if (object_type == nullptr) {
            ++result.opcodes_unimplemented[static_cast<std::uint8_t>(ins.opcode())];
            emit_exit(pc);
            terminated = true;
            break;
          }

          // Everything the instruction does is inside a test for the variable being null. The whole
          // guard comes out as one compare against a slot the instruction names by frame offset, and
          // the slot is loaded before the test because the engine call needs the value anyway.
          const auto slot = asmjit::x86::qword_ptr(
              reg::kFramePointer, -ins.sword0() * static_cast<std::int32_t>(kDwordSize));
          const auto done = a.new_label();

          a.mov(asmjit::x86::r10, slot);
          a.test(asmjit::x86::r10, asmjit::x86::r10);
          a.je(done);

          publish_registers(pc);

          a.mov(asmjit::x86::rcx, reg::kRegs);
          a.mov(asmjit::x86::rdx, asmjit::x86::r10);
          a.mov(asmjit::x86::r8, asmjit::Imm(static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(object_type))));
          call_native(reinterpret_cast<std::uintptr_t>(&runtime::ReleaseObject));

          // Cleared *after* the release and never before: the release is what needs the pointer, and
          // a hand-off must not find the variable still holding an object that has just been freed.
          a.mov(slot, asmjit::Imm(0));

          bail_if_suspend(next);

          a.bind(done);

          block.emitted = true;
          count_translated(ins.opcode());
          pc = static_cast<std::uint32_t>(next);
          continue;
        }

        if (ins.opcode() == AngelScript::asBC_REFCPY || ins.opcode() == AngelScript::asBC_RefCpyV) {
          const auto* object_type = reinterpret_cast<const AngelScript::asITypeInfo*>(ins.pointer());
          if (object_type == nullptr) {
            ++result.opcodes_unimplemented[static_cast<std::uint8_t>(ins.opcode())];
            emit_exit(pc);
            terminated = true;
            break;
          }

          // Where the handle being written lives, which is the whole of the difference between the
          // two instructions: `asBC_REFCPY` pops the address of the destination off the stack, while
          // `asBC_RefCpyV` names it as a frame variable and leaves the stack alone. The source is on
          // top of the stack in both, and neither pops it - the compiler leaves it there for whatever
          // consumes it next, and advancing past it here would corrupt the frame.
          if (ins.opcode() == AngelScript::asBC_REFCPY) {
            a.mov(asmjit::x86::r10, asmjit::x86::qword_ptr(reg::kStackPointer));
            a.mov(asmjit::x86::r11, asmjit::x86::qword_ptr(reg::kStackPointer, static_cast<std::int32_t>(kPtrSize)));
            a.add(reg::kStackPointer, static_cast<std::int32_t>(kPtrSize));
          } else {
            a.lea(asmjit::x86::r10,
                  asmjit::x86::ptr(reg::kFramePointer, -ins.sword0() * static_cast<std::int32_t>(kDwordSize)));
            a.mov(asmjit::x86::r11, asmjit::x86::qword_ptr(reg::kStackPointer));
          }

          if ((object_type->GetFlags() & (AngelScript::asOBJ_NOCOUNT | AngelScript::asOBJ_VALUE)) == 0) {
            publish_registers(pc);

            // The destination address, the source pointer and the type. The store back into the
            // destination is the helper's last act and is not repeated here - it has to follow both
            // reference count updates, which is the one ordering detail that is not obvious from the
            // bytecode and is why the two are not separate helpers.
            a.mov(asmjit::x86::rcx, reg::kRegs);
            a.mov(asmjit::x86::rdx, asmjit::x86::r10);
            a.mov(asmjit::x86::r8, asmjit::x86::r11);
            a.mov(asmjit::x86::r9, asmjit::Imm(static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(object_type))));
            call_native(reinterpret_cast<std::uintptr_t>(&runtime::AssignHandle));

            bail_if_suspend(next);
          } else {
            // A type that neither counts references nor has a destructor owns nothing that could
            // raise, so there is nothing for the helper to do and the whole instruction is the store.
            // There is no call on this path, which is why nothing is published either: every exit
            // from the block writes the same three fields anyway, and no engine code runs in between
            // to read them. A script that only moves handles of such a type pays one instruction
            // where the interpreter pays a flag test and a branch.
            a.mov(asmjit::x86::qword_ptr(asmjit::x86::r10), asmjit::x86::r11);
          }

          block.emitted = true;
          count_translated(ins.opcode());
          pc = static_cast<std::uint32_t>(next);
          continue;
        }

        if (ins.opcode() == AngelScript::asBC_Cast) {
          // The reference cast, and the last instruction this component refused on the grounds that
          // its operand was layout. That ground was wrong - the object's type, the type id's
          // resolution, the interface and inheritance tests and the reference count are all published -
          // and the paragraph it used to be refused under is gone from the emitter's list. What is
          // left of it is the cost, which is real and is why this is a call rather than an inline
          // sequence: the reference count arrives through the type's addref *behaviour*, which for a
          // script class is a generic thunk, where the interpreter increments directly.
          //
          // What the translation removes is the dispatch and the register traffic around it. The
          // handler publishes nothing - this is one of the few instructions that re-enters nothing and
          // can raise nothing - so the two publications here exist for the helper's sake rather than
          // the engine's: the operand is read through the *published* stack pointer, because compiled
          // code keeps that in a register and the helper has only the register block.
          //
          // The exit is a plain fall-through rather than an epilogue, because nothing here can suspend
          // or throw: the interpreter's handler has no `doProcessSuspend` test either, and the addref
          // of a script object is the engine's own thunk rather than anything a script registered. That
          // is the one place the shape differs from the calls above, which end their blocks because the
          // registers afterwards describe a frame that is not this one.
          publish_registers(pc);

          a.mov(asmjit::x86::rcx, reg::kRegs);
          a.mov(asmjit::x86::rdx, asmjit::Imm(static_cast<std::uint64_t>(engine_address)));
          a.mov(asmjit::x86::r8d, asmjit::Imm(static_cast<std::int32_t>(ins.dword())));
          call_native(reinterpret_cast<std::uintptr_t>(&runtime::CastHandle));

          // The helper advanced the published stack pointer past the slot it read the operand from, so
          // the native copy has to be reloaded - the block goes on using that one, and it is now one
          // slot further along than it was.
          a.mov(reg::kStackPointer, asmjit::x86::qword_ptr(reg::kRegs, static_cast<std::int32_t>(kOffStackPointer)));

          block.emitted = true;
          count_translated(ins.opcode());
          pc = static_cast<std::uint32_t>(next);
          continue;
        }

        if (ins.opcode() == AngelScript::asBC_SUSPEND) {
          // A suspend is only sometimes a suspend. The handler does nothing at all unless
          // regs->doProcessSuspend is set, and that field is the interface's own answer to "may a
          // compiled block run past this point" - it is set while a line callback or a pending
          // suspension needs servicing and clear otherwise. So the whole instruction compiles to one
          // test: break out when the interpreter has something to do, carry on when it does not.
          //
          // Getting this wrong in the permissive direction would be the worse failure. The compiler
          // turns every statement's line cue into a SUSPEND, so a JIT that always ran past them would
          // never give the context a chance to fire the line callback - which is what a script
          // timeout is built on - and an unbounded script would hang the game thread instead of being
          // aborted.
          bail_if_suspend(pc);

          block.emitted = true;
          count_translated(ins.opcode());
          pc = static_cast<std::uint32_t>(next);
          continue;
        }

        if (ins.opcode() == AngelScript::asBC_JMP) {
          emit_transfer(next + ins.displacement(), true);
          block.emitted = true;
          count_translated(ins.opcode());
          terminated = true;
          break;
        }

        if (ins.opcode() == AngelScript::asBC_JMPP) {
          // A `switch`. The selector is a small integer read out of a variable, and the "table" is
          // not a table at all: it is a run of `asBC_JMP` instructions immediately following, two
          // `asDWORD`s each, which the interpreter indexes into by adding the selector to its own
          // program counter. The compiler emits a range check before every one of these, so the
          // selector is in range whenever the interpreter reaches one.
          //
          // It compiles to a compare chain rather than to an indirect jump through a relocated
          // table, which is the other obvious translation and is not worth what it costs. asmjit
          // embeds an address only as an offset from the position that embeds it, so the indirect
          // form needs the table's own address, a scaled load, an add back and the jump - four
          // instructions before anything is dispatched - against one compare per case here, for the
          // single-digit case counts these scripts contain. The chain is also the form that cannot
          // go wrong at the one point the interpreter itself leaves undefined: an out-of-range
          // selector, which a table jump would turn into a jump to an arbitrary address.
          a.mov(asmjit::x86::eax, asmjit::x86::dword_ptr(abi.fp, -ins.var0() * static_cast<std::int32_t>(kDwordSize)));

          std::uint32_t cursor = static_cast<std::uint32_t>(next);
          std::int32_t selector = 0;

          while (cursor + 1 < length) {
            const Instruction entry(blob + cursor);
            if (entry.opcode() != AngelScript::asBC_JMP)
              break;

            const auto entry_size = static_cast<std::uint32_t>(entry.size());
            if (entry_size == 0 || cursor + entry_size > length)
              break;

            // The next case is tested by falling past this one, which is why the test is inverted:
            // the transfer that follows always ends in a jump or a return, so execution reaches the
            // label below it only when the selector did not match.
            asmjit::Label next_case = a.new_label();
            a.cmp(asmjit::x86::eax, selector);
            a.jne(next_case);
            // Not the end of the block: the next case's test, and eventually the fall-through past the
            // whole run, are emitted after this.
            emit_transfer(static_cast<std::int64_t>(cursor) + entry_size + entry.displacement(), false);
            a.bind(next_case);

            // Counted as translated even though no jump of its own is emitted: the transfer above is
            // this entry, and the dispatcher's compare chain is what the interpreter's indexing
            // arithmetic would have done. Leaving it uncounted would report every switch as having
            // one untranslated jump per case, which is the shape of a diagnostic that is wrong
            // rather than merely unflattering.
            count_translated(entry.opcode());

            ++selector;
            cursor += entry_size;
          }

          // Past the end of the run is a selector the compiler should never have produced, because
          // it range-checks first. Handing the instruction back is still the right answer rather
          // than falling out of the chain: the interpreter's own behaviour here is to add the
          // selector to its program counter and jump wherever that lands, and re-running the
          // instruction is the only way to reproduce a rule that has no rule.
          emit_exit(pc);

          block.emitted = true;
          count_translated(ins.opcode());
          terminated = true;
          break;
        }

        if (IsConditionalBranch(ins.opcode())) {
          // Two exits and one branch: the test goes to whichever of the two paths is not the one the
          // code after it falls into, and each path pays one transfer. This is the shape a condition is
          // compiled for, since the alternative - a test per path - costs a second branch on the path
          // that is taken, which is the path a loop takes on every iteration.
          const auto source = previous_left_value_flags ? BranchSource::Flags : BranchSource::ValueRegister;
          const auto target_dwords = next + ins.displacement();
          const auto target_block =
              target_dwords >= 0 ? blocks.find(static_cast<std::uint32_t>(target_dwords)) : blocks.end();

          if (target_block != blocks.end()) {
            // The branch names the target block outright, and the not-taken half is the last thing in
            // this block - so a not-taken successor that is the block emitted next falls through to it
            // instead of jumping, and a taken one reaches its block in a single branch.
            //
            // Both of those are what a loop back edge is: the block it transfers to is the loop head,
            // which is behind this one and so can never be what follows it, and the block that does
            // follow it is the loop's exit path or its next section. Getting this wrong is not a
            // matter of a few bytes. The shape this replaces branched to a local label bound
            // immediately after the not-taken transfer, and the taken half of that label was a `jmp`
            // to the block - so continuing a loop cost two taken branches per iteration, and the
            // second one was pure indirection.
            EmitBranch(a, abi, ins.opcode(), source, target_block->second.label);
            emit_transfer(next, true);
          } else {
            // The taken half is not a block, so it is an exit and has to be emitted somewhere. It
            // cannot be emitted in place - the test would have to branch over it on the path that does
            // not take it - so the test branches to a local label with the not-taken transfer before
            // it, and the exit sits after that, reachable only by the branch.
            //
            // The not-taken transfer is therefore never the end of the block: the exit is emitted
            // between it and whatever comes next, so dropping this jump would run the exit's code on
            // the fall-through.
            asmjit::Label taken = a.new_label();
            EmitBranch(a, abi, ins.opcode(), source, taken);
            emit_transfer(next, false);
            a.bind(taken);
            emit_transfer(target_dwords, true);
          }

          block.emitted = true;
          count_translated(ins.opcode());
          terminated = true;
          break;
        }

        asmjit::Label fault;
        const auto outcome = emitter.Emit(ins, &fault);

        // Read off the outcome rather than assumed, and read here rather than at the branch: the only
        // thing that can make the flags live is the instruction emitted last, and this is the emission
        // that just happened.
        value_flags_live = outcome == Emitter::Outcome::TranslatedWithValueFlags;

        if (outcome == Emitter::Outcome::Unsupported) {
          // The instruction is outside the translated set - a call, an object lifetime operation, a
          // string operation. Everything from here on runs interpreted, and the interpreter will hand
          // control back at the next JitEntry it reaches.
          //
          // This is the only site that records a translation gap, and it is the right one to be the
          // only one: an instruction the emitter has no case for reaches here and is counted, while an
          // instruction after it that the emitter *does* have a case for is never handed to the
          // emitter at all - translation of this block has already stopped. The two are both missing
          // from the translated count and mean different things, and this is what tells them apart.
          ++result.opcodes_unimplemented[static_cast<std::uint8_t>(ins.opcode())];
          emit_exit(pc);
          terminated = true;
          break;
        }

        if (outcome == Emitter::Outcome::TranslatedWithFaultRoute) {
          // The guard already branches here out of line, so the fall-through path - the one a working
          // division takes - has not been charged for it. The exit is deferred to the end of the
          // function rather than emitted here: putting it inline would mean jumping over it on the
          // fast path, which is the second branch the out-of-line guard exists to avoid.
          //
          // What the exit publishes is this instruction's own address, which is what the interpreter's
          // own fault paths publish too. That is what makes handing back correct rather than merely
          // safe: the interpreter resumes *at the instruction*, re-runs it, and raises from the one
          // place that knows how. It also means a fault site must not leave any partial effect behind
          // - the instruction is about to run a second time - which is why every guard below tests its
          // condition before storing anything.
          fault_stubs.emplace_back(fault, pc);
        }

        block.emitted = true;
        count_translated(ins.opcode());
        pc = static_cast<std::uint32_t>(next);
      }

      if (!terminated) {
        // Only reachable if the function's bytecode ends without a terminator, which the compiler
        // does not produce - every path ends in a RET. Treated as a block that does not translate so
        // that nothing is published for it: a branch into the label below hands the instruction back
        // to the interpreter and the VM continues from there, rather than re-entering the block it
        // just left.
        block.emitted = false;
        emit_exit(offset);
      } else if (!block.emitted) {
        // Declared as a branch target but never reached by anything translatable. It still needs a
        // body, because a native jump into it must land on something that returns to the VM.
        emit_exit(offset);
      }
    }

    // The exits the guards and the suspend checks branch to, past everything that can fall into them.
    // Nothing reaches this point by falling: a block ends by transferring, and a transfer is a jump or
    // a return - the one exception being a fall-through into the block emitted next, and the last block
    // has no next block to fall into. So an exit here is only ever entered through the branch that
    // names it, and a branch to a label bound after every block is a branch over all of them.
    //
    // Emitted in the order they were collected, which is the order their instructions appear in. The
    // only thing that depends on the order is the size of the branches that reach each one.
    for (const auto& [label, stub_pc] : fault_stubs) {
      a.bind(label);
      emit_exit(stub_pc);
    }

    for (const auto& [label, stub_pc] : suspend_stubs) {
      a.bind(label);
      emit_exit(stub_pc);
    }

    // Every label has been bound by this point - the exits by the loops above, every other label by
    // the instruction that made it - so nothing a branch was waiting on is still waiting. A fixup
    // still outstanding is a branch to a label nothing binds, and asmjit encodes an unbound label as
    // a displacement of zero, which for a branch is the instruction *after* it: the check the branch
    // was emitted for silently does nothing, the code assembles, and no error is reported anywhere.
    // That is not hypothetical - it is what `asBC_COPY`'s destination-pointer guard did until the
    // emitter stopped letting its second guard overwrite the first guard's label.
    //
    // The function is declined rather than published in that state. A guard that does not guard is a
    // wrong answer, and a function that runs interpreted is only a slow one.
    if (!errors.ok() || code.has_unresolved_fixups())
      return result;

    // Read before the code is relocated: `add` flattens and moves the sections, and while the final
    // size is the same, asking before the move keeps this a statement about what was emitted rather
    // than about what the allocator did with it.
    result.code_size = code.code_size();

    // ------------------------------------------------------------------------
    // Publish
    // ------------------------------------------------------------------------
    AngelScript::asJITFunction entry = nullptr;
    if (runtime.add(&entry, &code) != asmjit::kErrorOk || entry == nullptr)
      return result;

    // Nothing above this line has touched the function, so a failure anywhere leaves it exactly as it
    // was and running correctly interpreted. Everything below is the point of no return: the entry
    // points have to be written, and a failure to write them would leave compiled code unreachable
    // rather than leave the function broken.
    result.entry = entry;

    for (const auto& [offset, block] : blocks) {
      if (!block.emitted)
        continue;

      for (const std::uint32_t jit_entry : block.jit_entries) {
        // The VM re-reads the pointer argument every time it reaches this instruction, so the address
        // has to be the relocated one, not the pre-relocation offset. `add` has already moved the
        // code to its final address by this point, which is why this runs after it and not before.
        const auto target = reinterpret_cast<std::uintptr_t>(entry) + code.label_offset_from_base(block.label);
        PointerArgument(const_cast<ByteCode*>(blob) + jit_entry) = static_cast<AngelScript::asPWORD>(target);
        ++result.entries_published;
      }

      ++result.blocks_emitted;
    }

    if (result.entries_published == 0) {
      // Every block was reached by a branch and none by a JitEntry, so the VM has no way in. The code
      // is correct but unreachable; handing it to the engine would mean the function is reported as
      // compiled while continuing to be interpreted.
      //
      // The walked counts are deliberately kept rather than cleared with the rest. A function the JIT
      // declined still has instructions, and a coverage figure that left them out would report the
      // translated fraction of the functions that were easy to translate rather than of the program -
      // which is the number that would flatter the result.
      //
      // The other two go to zero, and they have to go together. Nothing in this function was
      // translated - the code is being thrown away - so every instruction in it is untranslated for
      // one reason: it is code the emitter never delivered. Leaving the gaps counted would report a
      // function with no entry points as though the emitter had choked on it, which is the opposite
      // of what happened and would point the next piece of work at the wrong opcode.
      runtime.release(entry);
      result.entry = nullptr;
      result.code_size = 0;
      result.blocks_emitted = 0;
      result.entries_published = 0;
      result.instructions_translated = 0;
      result.opcodes_translated = {};
      result.opcodes_unimplemented = {};
      return result;
    }

    return result;
  }

} // namespace base::asjit
