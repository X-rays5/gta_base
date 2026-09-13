//
// Created by X-ray on 13/09/2026.
//

#include "runtime.hpp"

// The engine's own entry point for calling a registered function, declared rather than reached
// through a header - the same arrangement, for the same reason, as the `as_powi` block at the top of
// emitter.cpp, and that comment is the long version. In short: `as_callfunc.h` is an internal header,
// including it would make this component depend on the SDK's source tree rather than on its published
// interface, and the symbol is one this target already links. Nothing here names a private member or
// reads a private layout - the declaration is the whole dependency, and if a future AngelScript
// changes the signature the link fails and says so, which is the failure mode a declaration that
// cannot be checked against a published header should have.
//
// `asCContext` is named here rather than included. The one conversion needed - from the
// `asIScriptContext*` the register block publishes - is a pointer identity: `asCContext` derives from
// `asIScriptContext` directly, which is the only way the engine has ever published a context, so
// nothing can sit between the two addresses.
//
// The two entry points below are internal to the engine: `CallSystemFunction` is a free function in
// the engine's source tree, and `CallScriptFunction` is a member of `asCContext`, a class the SDK
// publishes only in its *internal* headers. Neither is part of `asIScriptContext`, which is what the
// engine hands out, so there is no way to reach them through the published interface - and a JIT that
// could not reach the VM's own call and dispatch routines could not do its job at all.
//
// So the class is declared here with the one member this component calls, rather than by including
// `as_context.h` and taking a build dependency on the SDK's source tree. The things that make that
// safe are worth stating, because a class declaration that disagrees with the real one reads like a
// mistake: nothing here ever names a data member, takes the size of the class, constructs one, or
// calls anything through it that is virtual - the single use is a direct call with the pointer in the
// first argument register, and a class's member functions are not laid out in it. The real definition
// is in the header the library was built from; if this one stops matching it, the mangled name stops
// matching the exported symbol and the link fails, which is the loud failure a declaration that cannot
// be checked against a published header should have.
namespace AngelScript {
  class asCScriptFunction;
  class asCObjectType;
  class asCScriptObject;

  class asCContext {
  public:
    void CallScriptFunction(asCScriptFunction* func);
    void CallInterfaceMethod(asCScriptFunction* func);

    // The two halves of `asBC_RET`, both reached the way the two above are, and both out of line.
    //
    // `PopCallState` is the engine's own frame pop - five loads, five stores, and the length - and it
    // is exactly what the interpreter's return handler calls.
    //
    // `GetCallStateRegisters` is the predicate in front of it, and it is where this class stops being
    // a formality. It is `virtual` in the class the engine publishes - `asIScriptContext` declares it
    // so - and the keyword is not decoration: **it is in the mangled name.** Declaring it without
    // `virtual` here produced a symbol MSVC mangles `...@@QEAAH...` where the library exports
    // `...@@UEAAH...`, and the link failed rather than resolving to the same function. That is worth
    // recording because the opposite is a common belief and this is the cheap way to find out: the
    // declaration is one line and the linker is the one that answers.
    //
    // So the declaration keeps the keyword, and the *call* is what is made direct, by qualifying it -
    // `context->asCContext::GetCallStateRegisters(...)`, which C++ defines as a non-virtual call even
    // where the function is virtual. What that removes is the vtable load and the indirect branch;
    // what it leaves is the body, which serialises two stack pointers on every return. Nothing here
    // would read the vtable slot a shadow class invented, which matters because a mismatch would be a
    // call to whatever the real class keeps at that index.
    //
    // The one property the direct call actually rests on is that there is nothing to override it:
    // the SDK declares `class asCContext : public asIScriptContext` and nothing in the source tree or
    // the add-ons derives from either, so a call that skips the vtable cannot miss an override. It is
    // a checkable claim - one grep - and it is the claim that makes this a declaration rather than a
    // hazard.
    virtual int GetCallStateRegisters(asUINT stackLevel, asDWORD* stackFramePointer,
                                      asIScriptFunction** currentFunction, asDWORD* programPointer,
                                      asDWORD* stackPointer, asDWORD* stackIndex);
    void PopCallState();
  };

  // The one member of a function object this component needs, declared for the same reason the context
  // members above are: it is out of line, so the declaration is the whole dependency and a change to it
  // fails the link rather than reading the wrong offset. `asBC_ALLOC`'s script-object branch uses it to
  // reach the slot the caller reserved for the new object's address - the engine's own computation of
  // where that slot is, rather than a second one derived from the signature here.
  class asCScriptFunction {
  public:
    int GetSpaceNeededForArguments();
  };

  // The engine's own function table lookup, declared for the same reason as the context member above
  // and with one property that matters as much as its signature: it is *not* virtual. `asIScriptEngine`
  // publishes `GetFunctionById`, which is virtual and whose body is this function, so naming it here
  // converts an indirect call on the hot path into a direct one. The body is a bounds check and a load
  // from the array the interpreter indexes, so what this returns is what the interpreter would have
  // used, read at the same moment - which is the point: the id is re-resolved on every call rather than
  // cached, because ids are recycled when a module is discarded.
  class asCScriptEngine {
  public:
    asCScriptFunction* GetScriptFunction(int func_id) const;

    // The two halves of object construction, which the engine keeps together and this component has no
    // business reimplementing: `CallAlloc` is the allocator the engine was configured with, and the
    // placement constructor below is the one every script object goes through. Both are out of line.
    //
    // They are reached through a declaration rather than through `asIScriptEngine` because neither is
    // on the published interface - `CallAlloc` takes an `asCObjectType*`, a type the SDK does not hand
    // out. What that costs is the same thing every declaration in this block costs: nothing here names
    // a data member or takes a size, so a signature that changes fails the link.
    void* CallAlloc(const asCObjectType* obj_type) const;

    // The other half of the same pair, and the reason it is here rather than reached through
    // `asIScriptEngine`: an object allocated by `asBC_ALLOC` is one the application's allocator owns,
    // and freeing it with `delete` or `free` would be the wrong allocator. `ReleaseScriptObject`
    // reaches this for a value type whose memory has to go back, but only after running a destructor,
    // and the branch that uses it here is the one where the constructor never completed - there is
    // nothing to destruct, only memory to hand back.
    void CallFree(void* obj) const;
  };

  // The placement constructor for a script object: writes the object type into the memory `CallAlloc`
  // returned and initialises the reference count. The interpreter calls it between the two steps above,
  // and it is a free function in the engine's source tree rather than a member of anything.
  //
  // `asCScriptObject` is named only to form the call. It is never constructed, sized or dereferenced
  // here - the pointer that goes in is the allocator's own return value, and what comes back out is a
  // pointer the script's constructor fills in.
  void ScriptObject_Construct(asCObjectType* obj_type, asCScriptObject* self);

  int CallSystemFunction(int id, asCContext* context);
} // namespace AngelScript

namespace base::asjit::runtime {

  void InvokeSystemFunction(AngelScript::asSVMRegisters* regs, int function_id) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);

    // The one line that is not a reuse. The interpreter pops the arguments off its own local stack
    // pointer and publishes that later; a compiled block keeps the stack pointer in a register, so
    // the register block is what has to be advanced instead - by the same count, which is what the
    // engine's return value is: the number of `asDWORD`s of argument and object pointer the call
    // consumed. `CallSystemFunction` never writes the field itself, in either of its two branches, so
    // this cannot compound with anything.
    regs->stackPointer += AngelScript::CallSystemFunction(function_id, context);
  }

  void ReleaseObject(AngelScript::asSVMRegisters* regs, void* object, const void* type) noexcept {
    regs->ctx->GetEngine()->ReleaseScriptObject(object, static_cast<const AngelScript::asITypeInfo*>(type));
  }

  bool ReturnFromScriptFunction(AngelScript::asSVMRegisters* regs, std::int32_t stack_words) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);

    // The interpreter's own predicate, asked rather than reproduced: it reads
    // `m_callStack[len - CALLSTACK_FRAME_SIZE]` and finishes the execution when that saved frame
    // pointer is null (as_context.cpp:2429). `GetCallStateRegisters` at stack level 1 reads the same
    // element and reports the case as `asNO_FUNCTION` - and reports a level that does not exist as
    // `asINVALID_ARG`, which is the *other* way the interpreter finishes, its call stack having one
    // frame and no saved one behind it. Both are the early return; every other exit from that
    // function is `asSUCCESS`.
    //
    // Reporting rather than deciding is what makes this safe in the direction that matters. A failure
    // to serialise one of the two pointers - the only other way that function returns non-zero - is
    // also read as "hand it back", and handing a return back to the interpreter is always correct:
    // the instruction has no side effect here, so the VM re-runs it and reaches the outcome this
    // could not.
    if (context->AngelScript::asCContext::GetCallStateRegisters(1, nullptr, nullptr, nullptr, nullptr, nullptr) !=
        AngelScript::asSUCCESS)
      return false;

    // The engine's own frame pop, which is the whole of what the interpreter does here apart from
    // reading the registers back out. It restores `programPointer`, `stackPointer` and
    // `stackFramePointer` into `m_regs`, which *is* the register block this was handed - so there is
    // nothing to copy back, and the block's exit publishes nothing for the same reason a translated
    // call's does: the registers no longer describe the function that is leaving.
    context->PopCallState();

    // The one line the interpreter writes itself, because only it knows the instruction's own word.
    // It is the caller's arguments, popped from the caller's stack.
    regs->stackPointer += stack_words;
    return true;
  }

  void EnterScriptFunction(AngelScript::asSVMRegisters* regs, void* engine, int function_id) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);

    // Three things in this function were each worth an indirect call at one point, and the two that are
    // gone are gone because they are on the path a call-heavy script walks once per call. `GetEngine`
    // was one - its result is now an immediate the caller resolved while translating, for the reason the
    // header gives. `GetFunctionById` was the other: it is virtual, and it is `GetScriptFunction` with
    // nothing added, so naming the engine's own lookup directly turns an indirect call into a direct
    // one against a table that is going to be touched anyway. What is left is the call that does the
    // work, `CallScriptFunction`, and no test around it that the interpreter does not also make.
    auto* function = static_cast<AngelScript::asCScriptEngine*>(engine)->GetScriptFunction(function_id);

    // The id is the instruction's argument and indexes the same engine-wide table the interpreter's own
    // handler indexes, so this is the identifier the compiler wrote met by the one the engine issued.
    // The interpreter asserts the result is not null rather than testing it, and it cannot be null for
    // any bytecode the compiler emits. Returning leaves the published registers describing the call
    // site, so the VM resumes the caller at the instruction after the call - which is the wrong thing to
    // do, and is deliberately the *last* wrong thing: the alternative is dereferencing the null inside
    // the engine, and the resume point is one the caller can survive.
    if (function == nullptr)
      return;

    context->CallScriptFunction(function);
  }

  void EnterInterfaceMethod(AngelScript::asSVMRegisters* regs, void* engine, int function_id) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);
    auto* function = static_cast<AngelScript::asCScriptEngine*>(engine)->GetScriptFunction(function_id);

    // The two null tests are the interpreter's, in its order, and they are not the same test: one is
    // the compiler having issued an id the table does not hold, the other - inside the engine's routine
    // - is the script having called a method on a null handle, which is a script-visible error rather
    // than a broken id. Folding them together would report a null-object access for a fault that is
    // not one.
    //
    // Unlike its sibling, a null in the second test is not a reason to leave: the engine routine has
    // already raised the exception by then, and the VM has to be given the chance to unwind - which it
    // gets, because this returns to the `asBC_JitEntry` that entered the block and that handler checks
    // the status before dispatching anything else.
    if (function == nullptr)
      return;

    context->CallInterfaceMethod(function);
  }

  void AssignHandle(AngelScript::asSVMRegisters* regs, void** destination, void* source, const void* type) noexcept {
    auto* engine = regs->ctx->GetEngine();
    const auto* type_info = static_cast<const AngelScript::asITypeInfo*>(type);

    // The interpreter writes these two calls out under a guard for the type needing reference counting
    // and null tests on both pointers. The guard is a property of the type alone, so it is settled
    // where the type is known and never reaches here - and both entry points begin with a null test
    // of their own, so the pointer tests are inside the engine's rather than repeated outside it.
    //
    // The order is the interpreter's, and the store has to be last: the old value in the destination
    // is what is being released, and writing the new one over it first would lose the only reference
    // to it. The same two calls in the other order would leak every handle a script ever reassigns.
    engine->ReleaseScriptObject(*destination, type_info);
    engine->AddRefScriptObject(source, type_info);

    *destination = source;
  }

  void CastHandle(AngelScript::asSVMRegisters* regs, void* engine, std::int32_t type_id) noexcept {
    auto* script_engine = static_cast<AngelScript::asIScriptEngine*>(engine);

    // The operand is one level of indirection away from the object: the stack slot holds the address
    // of the handle variable, and the variable holds the object. Reading the slot as the object would
    // take the address of a local for an object pointer and hand it to the type test.
    auto** slot = *reinterpret_cast<void***>(regs->stackPointer);

    // Two null tests in one condition, which is the interpreter's own `a && *a` and is not one test
    // written twice: the first is a handle with no addressable slot, the second a slot holding
    // nothing. Neither is an error - a cast that does not match is a null *result*, which the compiler
    // has already arranged by writing null where the cast's value goes - so both fall through to the
    // failure path below rather than raising anything.
    if (slot != nullptr && *slot != nullptr) {
      auto* object = static_cast<AngelScript::asIScriptObject*>(*slot);

      // The type the object is, and the type the instruction names. Both are re-resolved here rather
      // than resolved once while translating, and for the same reason the call translations re-read
      // their id: a script class belongs to a module, and discarding that module returns its type id
      // to the pool for another type to take. A pointer kept from translation time would be another
      // type's answers, not a stale pointer that crashes.
      const auto* from = object->GetObjectType();
      const auto* to = script_engine->GetTypeInfoById(type_id);

      // The one place this is not a transcription of the handler. `GetObjectTypeFromTypeId` - the
      // internal lookup the handler uses - additionally discards a type that is not an object type,
      // and this does not. It cannot matter for any type id the compiler can put in this instruction:
      // the handler asserts both are script types, and the two lookups agree for those. For anything
      // else the extra null would only have reached the same answer, because `Implements` and
      // `DerivesFrom` compare against `to` and never dereference it - and the second of the two
      // compares `this`, which is an object type and so cannot be the null.
      if (from != nullptr && to != nullptr && (from->Implements(to) || from->DerivesFrom(to))) {
        // The object is published *before* the reference is taken, in the handler's order, and the
        // object type is cleared rather than set: the register now holds a pointer the engine has
        // counted, and marking it as a typed object would tell the consumer to manage what has
        // already been managed.
        regs->objectType = nullptr;
        regs->objectRegister = object;

        // On the failure path the object register is left exactly as it is, which is what the handler
        // does and what its `asASSERT(m_regs.objectRegister == 0)` records: the register is already
        // null wherever a cast can be reached, so clearing it here would be a store the interpreter
        // does not make. Matching that is the point - a translation that tidied it up would be
        // changing an observable the interpreter leaves alone.
        script_engine->AddRefScriptObject(object, from);
      }
    }

    // Unconditional, and after the work rather than before it, which is the handler's order too: the
    // slot is still needed above, and a cast whose type test runs on an advanced stack pointer would
    // be reading the next expression's temporary.
    regs->stackPointer += AS_PTR_SIZE;
  }

  void AllocateScriptObject(AngelScript::asSVMRegisters* regs, void* engine, const void* type, int constructor_id) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);
    auto* allocator = static_cast<AngelScript::asCScriptEngine*>(engine);
    auto* object_type = static_cast<AngelScript::asCObjectType*>(const_cast<void*>(type));

    // The allocation and the pre-initialisation, in the interpreter's order. Neither is a call the
    // script can observe the internals of: the first is the allocator the engine was configured with,
    // and the second writes the type and the initial reference count into the memory it returned. The
    // constructor that follows is what gives the object its value, and it is entered exactly the way
    // `asBC_CALL` enters a callee - the engine builds its frame and the VM resumes inside it.
    auto* memory = static_cast<AngelScript::asDWORD*>(allocator->CallAlloc(object_type));
    AngelScript::ScriptObject_Construct(object_type, reinterpret_cast<AngelScript::asCScriptObject*>(memory));

    // Re-resolved at the call, not baked alongside the published arguments: ids are recycled when a
    // module is discarded, which is the same reason the two call translations above do this here rather
    // than at translation time. A null means the compiler named an id the table no longer holds, which
    // the interpreter asserts cannot happen; returning leaves the instruction half-done but the context
    // intact, which is the failure this component can survive and dereferencing is not.
    auto* constructor = allocator->GetScriptFunction(constructor_id);
    if (constructor == nullptr)
      return;

    // The caller reserved a slot for the new object's address, and where it sits is a function of the
    // constructor's signature - which is the engine's answer to give, not this component's to work out.
    // The store is what makes `this` inside the constructor point at the object.
    auto** slot = *reinterpret_cast<AngelScript::asDWORD***>(
        regs->stackPointer + constructor->GetSpaceNeededForArguments());
    if (slot != nullptr)
      *slot = memory;

    // Then the object's own address goes on the stack, which is what the constructor's frame will find
    // as its object pointer.
    regs->stackPointer -= AS_PTR_SIZE;
    *reinterpret_cast<AngelScript::asPWORD*>(regs->stackPointer) = reinterpret_cast<AngelScript::asPWORD>(memory);

    // The instruction is four dwords wide - the opcode, the type and the constructor's id - and this is
    // the interpreter's own `l_bc += 2+AS_PTR_SIZE` written against the context's copy of the pointer,
    // because the call below records it as this frame's resume point.
    regs->programPointer += 2 + AS_PTR_SIZE;

    context->CallScriptFunction(constructor);
  }

  void ConstructValueObject(AngelScript::asSVMRegisters* regs, void* engine, const void* type, int constructor_id) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);
    auto* allocator = static_cast<AngelScript::asCScriptEngine*>(engine);
    auto* object_type = static_cast<AngelScript::asCObjectType*>(const_cast<void*>(type));

    // Unconditional, and first: the interpreter allocates before it knows whether there is a
    // constructor to call, and a type with no constructor is still a type that needs memory.

    auto* memory = static_cast<AngelScript::asDWORD*>(allocator->CallAlloc(object_type));

    if (constructor_id != 0) {
      // The object's own address goes on top of the stack, below the arguments the caller already
      // pushed, because that is where the marshaller will look for it when the constructor's calling
      // convention needs a `this`. Then the same call the `asBC_CALLSYS` translation makes, whose
      // return value is the count of dwords it consumed - object pointer and arguments together - so
      // the single addition below is both the push and the pop, which is the interpreter's shape too.
      regs->stackPointer -= AS_PTR_SIZE;
      *reinterpret_cast<AngelScript::asPWORD*>(regs->stackPointer) = reinterpret_cast<AngelScript::asPWORD>(memory);

      regs->stackPointer += AngelScript::CallSystemFunction(constructor_id, context);
    }

    // The slot the caller reserved for the new object's address, which is on top of the stack now
    // that the constructor has taken everything below it. The null test is the interpreter's, and it
    // is not defensive: an instruction can be reached with no destination - that is what the guard on
    // the store is for.

    auto** slot = *reinterpret_cast<AngelScript::asDWORD***>(regs->stackPointer);
    regs->stackPointer += AS_PTR_SIZE;
    if (slot != nullptr)
      *slot = memory;

    // The instruction is four dwords wide - the opcode, the type and the constructor's id - which is
    // the interpreter's own `l_bc += 2+AS_PTR_SIZE` written against the context's copy of the pointer.
    // Unlike the script-object branch there is no frame being recorded here, so this is bookkeeping
    // rather than a resume point; it is still done, because the interpreter does it and because the
    // exit below publishes what it leaves behind.
    regs->programPointer += 2 + AS_PTR_SIZE;

    if (!regs->doProcessSuspend)
      return;

    // The published form of the test the interpreter makes against its own private status. It is the
    // *only* one of the interpreter's two questions that can be answered from outside the engine, and
    // answering it is what keeps the exception path exact; the suspension it cannot be told apart
    // from is the VM's to take, at the next point it checks for one.
    if (regs->ctx->GetState() == AngelScript::asEXECUTION_ACTIVE)
      return;

    // Order matters: the memory goes back first and the slot is nulled after, so that a hand-off
    // cannot find a variable pointing at memory the allocator has already taken back. The guard on
    // the store is not in the interpreter, which writes through the slot unconditionally on this path
    // - it is a crash rather than a difference, and reproducing a crash is not fidelity.
    allocator->CallFree(memory);
    if (slot != nullptr)
      *slot = nullptr;
  }

  bool EnterFunctionPointer(AngelScript::asSVMRegisters* regs, std::int32_t frame_offset) noexcept {
    auto* context = reinterpret_cast<AngelScript::asCContext*>(regs->ctx);

    // The variable is named as a distance below the frame pointer, in the dwords the bytecode counts in,
    // which is the same addressing every frame-relative load in the emitter uses.
    auto* function = *reinterpret_cast<AngelScript::asIScriptFunction**>(
        reinterpret_cast<std::byte*>(regs->stackFramePointer) -
        static_cast<std::ptrdiff_t>(frame_offset) * static_cast<std::ptrdiff_t>(sizeof(AngelScript::asDWORD)));

    // An unbound funcdef and an imported function are both refused here rather than handled, and the two
    // reasons are different. The first - a null - is a script-visible error the interpreter reports by
    // raising `TXT_UNBOUND_FUNCTION` after marking the arguments for the exception handler to clean up;
    // the second is a module-level indirection through a table the published interface does not publish.
    // Neither is reachable through a declaration, and both are reachable by handing the instruction back:
    // nothing above this line has changed anything, so the VM can run its own handler and reach the
    // outcome this cannot. The interpreter's `asASSERT(false)` default for the remaining kinds is left to
    // it for the same reason - a kind this component has no case for is not one it should invent one for.
    if (function == nullptr)
      return false;

    const auto kind = function->GetFuncType();
    if (kind != AngelScript::asFUNC_SCRIPT && kind != AngelScript::asFUNC_SYSTEM && kind != AngelScript::asFUNC_DELEGATE)
      return false;

    // `asBC_CallPtr` is one dword, and the interpreter advances the context's program pointer past it
    // before every branch below - so this runs after the kind is known and before anything is called,
    // which is what keeps the hand-back above from leaving a resume point behind.
    regs->programPointer += 1;

    switch (kind) {
    case AngelScript::asFUNC_SCRIPT:
      // A plain script function pointer. The call is the one `asBC_CALL` makes, reached by a different
      // route to the same routine - and the frame is built by the engine there too.
      context->CallScriptFunction(reinterpret_cast<AngelScript::asCScriptFunction*>(function));
      return true;

    case AngelScript::asFUNC_SYSTEM:
      // A pointer to a registered function. There is no frame to build; the engine's marshaller is
      // entered and hands back the number of dwords it consumed, exactly as `asBC_CALLSYS` reads it.
      regs->stackPointer += AngelScript::CallSystemFunction(function->GetId(), context);
      return true;

    case AngelScript::asFUNC_DELEGATE: {
      // A method bound to an object, which is the one kind that carries its own receiver: the object
      // pointer is pushed the way an interface call's would be, and the target is then entered by the
      // *interface* route rather than the direct one - which is the interpreter's choice, not an
      // oversight, and is reproduced rather than improved on. The two accessors are the published form
      // of the two fields the interpreter reads directly.
      regs->stackPointer -= AS_PTR_SIZE;
      *reinterpret_cast<AngelScript::asPWORD*>(regs->stackPointer) =
          reinterpret_cast<AngelScript::asPWORD>(function->GetDelegateObject());

      auto* target = function->GetDelegateFunction();
      if (target->GetFuncType() == AngelScript::asFUNC_SYSTEM) {
        regs->stackPointer += AngelScript::CallSystemFunction(target->GetId(), context);
      } else {
        context->CallInterfaceMethod(reinterpret_cast<AngelScript::asCScriptFunction*>(target));
      }
      return true;
    }

    default:
      return false;
    }
  }

} // namespace base::asjit::runtime
