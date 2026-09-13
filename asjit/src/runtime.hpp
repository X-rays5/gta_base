//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include <angelscript.h>

#include <cstdint>

// The parts of the VM the emitted code calls back into.
//
// Everything else in this component translates an instruction into machine code that touches nothing
// but the register block. A call cannot: talking to a registered function means marshalling arguments
// onto the platform's own calling convention, and that is `asCScriptFunction::sysFuncIntf` and an
// assembly-level shuffler per architecture - the one part of the engine that is *not* portable and
// the one place a translation would be a reimplementation rather than a translation.
//
// So the instructions that hand control to a native function keep calling the engine's own routine
// and translate only the dispatch around it. What that buys is on the block, not on the call: the run
// of native code no longer ends at every call, so a menu-shaped script - mostly calls, with a little
// arithmetic between them - compiles to one block per statement instead of one per native.
namespace base::asjit::runtime {

  // Runs one registered system function - the call a `asBC_CALLSYS` names by id - against the context
  // the register block belongs to.
  //
  // This is the same call the interpreter's own handler makes, in the same order, with the same state
  // published first. In particular it is the engine that converts a C++ exception into a script one,
  // that decides whether the return value goes in the value register or through a pointer the script
  // allocated, and that records the object type of a returned handle. None of that is repeated here.
  //
  // `regs` must already have `stackPointer`, `stackFramePointer` and `programPointer` describing the
  // call site: the engine reads the arguments from the first, and computes an exception's line number
  // from the last. The registers are left describing the instruction *after* the call, which is the
  // one piece of the interpreter's handler that is reproduced rather than reused - it is one
  // addition, and it is the caller's because only the caller knows where the next instruction is.
  void InvokeSystemFunction(AngelScript::asSVMRegisters* regs, int function_id) noexcept;

  // Releases the reference an object variable holds. This is the body of `asBC_FREE`, which the
  // interpreter writes out as a call to `asIScriptEngine::ReleaseScriptObject` and nothing else.
  //
  // That routine is the whole reason this needs no new understanding of object layout: it releases a
  // reference type through its release behaviour, and for a value type runs the destructor and then
  // hands the memory back to whatever allocated it - which is the same allocator `asBC_ALLOC` would
  // have used. It is the engine's own code, reached through the published interface.
  //
  // `type` is the `asCObjectType*` the instruction carries. It is passed as an `asITypeInfo*` because
  // that is the base the engine publishes, and it is a fixed-up-free conversion: `asCObjectType`
  // derives from `asITypeInfo` through a single chain, so the two pointers are the same address.
  // It is `const void*` here rather than a type the header would have to name, because the only
  // thing the caller has is an opaque bytecode word.
  void ReleaseObject(AngelScript::asSVMRegisters* regs, void* object, const void* type) noexcept;

  // Re-points a handle, releasing what the destination held and taking a reference on the source.
  // `asBC_REFCPY` and `asBC_RefCpyV` differ only in where the destination is - a pointer popped off
  // the stack in one, a frame variable named by the instruction in the other - and share this body,
  // exactly as their two interpreter handlers do.
  //
  // The caller is responsible for the one decision that is not shared: a type that neither counts
  // references nor owns a destructor is re-pointed by a plain store, and that is a property of the
  // type rather than of the instruction, so it is settled where the type is known and the store is
  // emitted with no call at all.
  void AssignHandle(AngelScript::asSVMRegisters* regs, void** destination, void* source, const void* type) noexcept;

  // The reference cast of a script object, which is `asBC_Cast`: the object on the top of the stack is
  // tested against the type the instruction names, and on a match it is published in the object
  // register and a reference is taken on it.
  //
  // **This instruction was refused for a reason that was simply false, and the correction is the whole
  // of why it is here.** The refusal said there was "nothing in it that is not layout" - that the
  // handler reads `asCScriptObject::objType`, the engine's type table, the type's interface list and
  // the object's own reference count, "four members of two classes that are not installed". Every one
  // of those four is published: `asIScriptObject::GetObjectType` returns the object type, and it is
  // the same `objType` the handler reads, since the class implements it as `return objType`;
  // `asIScriptEngine::GetTypeInfoById` resolves the instruction's type id through the same
  // `GetDataTypeFromTypeId` table `GetObjectTypeFromTypeId` uses; `Implements` and `DerivesFrom` are
  // `asITypeInfo` members; and `asIScriptEngine::AddRefScriptObject` is the engine's own entry point
  // for the reference count. What the handler calls directly, this reaches through the interface - a
  // virtual call where the interpreter has a load, and that is the cost, not the reachability.
  //
  // The one difference that is not merely indirect is the reference count. The interpreter calls
  // `obj->AddRef()` on the object, which is two stores; `AddRefScriptObject` runs the type's addref
  // *behaviour*, and for a script class that behaviour is registered as a generic thunk
  // (as_scriptengine.cpp:309), so the increment arrives through an `asCGeneric` and a call. It is the
  // same increment - the thunk's whole body is `self->AddRef()` - and no cheaper published route to it
  // exists: the reference count is on the object, `asIScriptObject` publishes it only through
  // `GetRefCount`, and the behaviours table is not published at all.
  //
  // `stackPointer` must already be published, because the operand is read through it: the stack slot
  // holds the *address* of the handle variable rather than the handle, which is how a cast of a
  // temporary and a cast of a local are the same instruction. This advances the stack pointer by the
  // slot, which the caller reloads - the interpreter's handler does the same addition at the same
  // point, after everything else.
  void CastHandle(AngelScript::asSVMRegisters* regs, void* engine, std::int32_t type_id) noexcept;

  // Enters another script function on the context's own call stack. This is the body of `asBC_CALL`,
  // which the interpreter writes out as a store of `programPointer`, a store of the two frame pointers
  // and a call to the engine's own routine - the same three publications a compiled block makes before
  // any call, followed by the same entry point.
  //
  // It is here rather than left to the interpreter because the alternative to a translated call is not
  // "the interpreter runs it", it is a round trip: the block ends, the VM dispatches the `asBC_CALL`
  // handler, the handler sets the frame up, the dispatch loop runs on to the callee's own `asBC_JitEntry`
  // and only there does compiled code resume. Reaching the same routine from here removes the two
  // dispatches and the register bookkeeping between them, and the SDK places a `JitEntry` after every
  // call precisely so that the callee can be re-entered cheaply from this direction.
  //
  // What is *not* reimplemented is the frame: `CallScriptFunction` pushes the call state, makes the new
  // function current, reserves its stack and runs its variable initialisation, and the line callback
  // for it. Reproducing that would be reproducing the VM's stack discipline, which is the part the SDK
  // says a JIT should not do.
  //
  // The caller owns the one difference from every other exit: after this returns, the registers describe
  // the *callee*, so nothing may be published over them. That is why the emitted sequence ends in a bare
  // epilogue rather than in the usual one, and why this function takes no resume point - the VM reads
  // where to continue from the frame this just installed.
  //
  // `regs` must already describe the call site with `programPointer` set to the instruction *after* the
  // call, which is what the engine stores as the caller's resume point when it pushes the call state.
  // `engine` is the engine the calling function belongs to, resolved by the caller at translation time
  // and passed in as an address the emitted code loads. It is here rather than fetched from `regs`
  // because the only way to reach an engine from a context is `asIScriptContext::GetEngine`, which is
  // virtual - and this is the hot path of exactly the workload a JIT is for: a script that calls
  // another script function in a loop. The engine is resolved once per call site while translating and
  // never again, which is free, and it is the same pointer the virtual call would have returned: a
  // function's engine is fixed for its lifetime. Baking that one is safe for the reason baking the
  // callee is not - the engine outlives every module compiled against it, and a block is released as
  // its own function is destroyed, which happens while the engine is still alive.
  //
  // The callee is deliberately *not* baked alongside it, even though resolving it here would save the
  // load below. Function ids are recycled - `asCScriptEngine::RemoveScriptFunction` clears the slot and
  // pushes the id for reuse - so a pointer kept across a module discard can dangle, or worse, be
  // silently wrong. The id is re-read through the same table the interpreter reads, at the same moment
  // and under the same bounds check, which is what makes this agree with the interpreter about *which*
  // function it is calling and not merely about how the call is made.
  void EnterScriptFunction(AngelScript::asSVMRegisters* regs, void* engine, int function_id) noexcept;

  // Constructs a script object and enters its constructor. This is the `asOBJ_SCRIPT_OBJECT` branch of
  // `asBC_ALLOC`, and it is the call shape a third time: the allocation and the constructor's frame are
  // built by the engine, the constructor is entered the way `asBC_CALL` enters a callee, and the exit is
  // a bare epilogue because after it the registers describe the constructor.
  //
  // What the translation removes is the same detour the call translation removes, plus the two
  // dispatches of `asBC_ALLOC` itself, which is a longer instruction than a call: it allocates, writes
  // the new object's address into the argument slot the caller reserved for it, pushes the address, and
  // only then calls. Every one of those steps is the interpreter's, run here against the same registers,
  // in the same order, with the instruction's own arguments - the type and the constructor's id - read
  // out of the bytecode at translation time instead of at run time.
  //
  // `programPointer` must already name this instruction. Unlike the call translation, which publishes
  // the instruction *after* the call, this one publishes the instruction itself, because the engine's
  // own handler advances the context's copy past the allocation before it builds the frame - and that
  // advanced value is what the frame records as the caller's resume point.
  void AllocateScriptObject(AngelScript::asSVMRegisters* regs, void* engine, const void* type, int constructor_id) noexcept;

  // Constructs a *value* type on the heap. This is the other half of `asBC_ALLOC`, and the two are
  // separate entry points because they share nothing after the allocator: a script object's
  // constructor is a script function, entered with a frame, and the block ends there; a value type's
  // is a registered one, called the way `asBC_CALLSYS` calls anything, and the block goes on.
  //
  // The five steps are the interpreter's, in its order, with its arithmetic: allocate, push the
  // object's address for the constructor to find as its `this`, let the engine's marshaller pop it
  // along with the arguments, pop the caller's destination slot off the stack, write the address
  // there, and advance the context past the instruction. What is deliberately not repeated is the
  // allocator and the marshaller - `CallAlloc` is the allocator the engine was configured with, and
  // `CallSystemFunction` is the routine the `asBC_CALLSYS` translation already reuses, so a
  // constructor reached through this instruction and one reached through a plain call are the same
  // call.
  //
  // The constructor is optional. An instruction with no constructor names a type whose default
  // initialisation is the allocation and nothing else, and the interpreter's `if (func)` around the
  // whole call is reproduced rather than assumed away: it is a property of the instruction, not of
  // the type.
  //
  // **The tail is the one place this component decides something rather than reproducing it, and it
  // decides less than it looks.** After the instruction the interpreter asks two questions in a row
  // when `doProcessSuspend` is set: whether `asCContext::m_doSuspend` says a suspension is pending -
  // in which case it stops, having done nothing else - and, failing that, whether the constructor
  // raised. Only the second is a test this can make: `m_doSuspend` is private with no accessor, while
  // the exception it is being told apart from *is* published, as `asIScriptContext::GetState`, which
  // returns `m_status` and nothing else. So an exception frees the memory the constructor was given
  // and nulls the slot that was about to point at it, exactly as the interpreter does; a pending
  // suspension is left for the VM to take at the next point it checks one, which is the same margin
  // `bail_if_suspend` takes at every call, and the argument for it is written out there.
  //
  // Getting the pair the wrong way round is what the previous refusal of this branch was about, and
  // it was right about that: freeing on a suspension frees an object that is alive and about to be
  // used. It is not right that the two cannot be told apart - one of them is published.
  void ConstructValueObject(AngelScript::asSVMRegisters* regs, void* engine, const void* type, int constructor_id) noexcept;

  // Enters a function named by a *pointer* rather than by an id, which is `asBC_CallPtr`: the call a
  // funcdef variable makes, and the only call in the language whose target is a value rather than a
  // constant.
  //
  // The interpreter's handler dispatches on the callee's *kind* - a script function, a system function,
  // a delegate, an imported function - and the kinds are distinguished only by a field of the function
  // object. That field is not reachable through a declaration, but the question it answers is published:
  // `asIScriptFunction::GetFuncType` reports the same value, and the delegate accessors beside it
  // publish the two fields the delegate branch reads. So the dispatch is written out here against the
  // published interface, and the branches that need something the interface does not publish - an
  // unbound variable, an imported function - are *not* taken.
  //
  // Returning false is how those are refused, and it is a refusal the caller can act on: nothing has
  // been touched, so the instruction can be handed back to the interpreter, which runs its own handler
  // and reaches the outcome this cannot. That is the whole reason the return value exists - a branch
  // this component cannot take is not an error, it is an instruction the VM still owns end to end.
  //
  // `frame_offset` is the instruction's own variable word: the local that holds the function pointer,
  // named as a distance *below* the frame pointer, in the dwords the bytecode counts in.
  [[nodiscard]] bool EnterFunctionPointer(AngelScript::asSVMRegisters* regs, std::int32_t frame_offset) noexcept;

  // The same entry for an interface method, which is the same call reached by a different route: the
  // interpreter's two handlers differ only in the routine they hand the resolved function to, and the
  // two routines differ in what they do *before* the frame goes up - one makes the callee current,
  // the other first walks the object's type for the implementation that satisfies the interface and
  // raises a null-pointer exception if the object is null. Neither difference is visible from here.
  //
  // It is a separate entry point rather than a flag on the one above because the resolution is the
  // whole of the difference and it belongs to the engine: `CallInterfaceMethod` is a context member
  // like its sibling, reached the same way, and a boolean argument would only move the branch.
  //
  // The one thing a caller has to know is that this can fail where the other cannot: a null object
  // leaves the registers describing *this* frame and the context holding an exception, which the VM
  // checks for itself when the block's exit returns through `asBC_JitEntry`. So the exit is the same
  // bare epilogue, and the published `programPointer` is the instruction after the call in both cases.
  void EnterInterfaceMethod(AngelScript::asSVMRegisters* regs, void* engine, int function_id) noexcept;

  // Leaves the current script function, which is `asBC_RET`: the caller's state is restored from the
  // context's own call stack and the caller becomes current.
  //
  // **Returns whether that happened, and the false answer is a hand-back rather than a failure.**
  // Whether a frame may be popped is a property of who called the function, and the interpreter
  // answers it by asking whether the frame it is about to pop saved a null frame pointer - the case
  // being the outermost function of an execution, and the same null appearing again as the marker a
  // pushed *state* leaves behind. When the answer is no, the interpreter finishes the execution
  // instead, and this does neither: it touches nothing and returns false, so the caller can hand the
  // instruction back to the interpreter at its own address, which re-runs it and reaches that
  // outcome. Nothing here has any effect to undo, so re-running is exact rather than merely safe.
  //
  // That division is what keeps the predicate honest rather than merely fast. The published
  // interface has no accessor for `asCContext::m_callStack`, so the question is asked through
  // `asIScriptContext::GetCallStateRegisters` - whose body serialises two stack pointers before it
  // answers, and whose answer is the saved frame pointer being null. `runtime.cpp` declares that
  // member and calls it qualified, which is a direct call rather than a vtable dispatch; the
  // reasoning that makes that safe, and the linker error that settled how it had to be declared, are
  // written out there.
  //
  // The cost is the reason this is behind `Options::translate_returns`: it is a call into the engine
  // on an instruction the interpreter runs with a load and a compare, and a return is what a
  // call-heavy script is mostly made of. The measurement is in the `asBC_RET` case in
  // `function_compiler.cpp`.
  //
  // `regs` must describe the returning function, and `stack_words` is the instruction's own word -
  // the caller's arguments, which come off the caller's stack. On a true return the register block
  // describes the *caller*, so the block's exit publishes nothing over it.
  [[nodiscard]] bool ReturnFromScriptFunction(AngelScript::asSVMRegisters* regs, std::int32_t stack_words) noexcept;

} // namespace base::asjit::runtime
