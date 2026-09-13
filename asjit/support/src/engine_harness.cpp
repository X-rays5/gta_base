//
// Created by X-ray on 13/09/2026.
//

#include "engine_harness.hpp"

#include <angelscript/scriptarray/scriptarray.h>
#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <angelscript/scriptdictionary/scriptdictionary.h>
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <asjit.hpp>

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

namespace base::asjit::support {
  namespace {
    std::string g_output;
    std::string g_errors;

    // The context the running script is executing on, published so that a binding can raise a script
    // exception from inside a native - which is the only way to reach the tail of an instruction whose
    // cleanup depends on the callee having raised.
    //
    // A global for the same reason the output buffer is: the bindings are plain C functions with no
    // user-data slot, and there is only ever one script running. It is set and cleared by `Execute`
    // rather than by `RunScript`, because `Execute` is what creates and owns the context.
    AngelScript::asIScriptContext* g_running_context = nullptr;

    void Emit(const std::string& text) {
      g_output += text;
      g_output += '\n';
    }

    // One binding per type, so that a caller can print exactly the width it means to compare.
    // Printing an int through a float would hide the very differences these exist to find.
    void OutInt(int value) { Emit(std::to_string(value)); }
    void OutUInt(unsigned int value) { Emit(std::to_string(value)); }
    void OutInt64(std::int64_t value) { Emit(std::to_string(static_cast<long long>(value))); }
    void OutUInt64(std::uint64_t value) { Emit(std::to_string(static_cast<unsigned long long>(value))); }
    void OutBool(bool value) { Emit(value ? "true" : "false"); }
    void OutString(const std::string& value) { Emit(value); }

    // %.9g round-trips a float and %.17g a double: printed any shorter, two genuinely different
    // results would compare equal and a comparison would pass for the wrong reason.
    void OutFloat(float value) {
      char buffer[64];
      std::snprintf(buffer, sizeof(buffer), "%.9g", static_cast<double>(value));
      Emit(buffer);
    }

    void OutDouble(double value) {
      char buffer[64];
      std::snprintf(buffer, sizeof(buffer), "%.17g", value);
      Emit(buffer);
    }

    void MessageCallback(const AngelScript::asSMessageInfo* msg) {
      if (msg->type != AngelScript::asMSGTYPE_ERROR)
        return;
      g_errors += std::string(msg->section != nullptr ? msg->section : "") + ":" + std::to_string(msg->row) + ": " + msg->message + "\n";
    }

    void RegisterOutput(AngelScript::asIScriptEngine* engine) {
      engine->RegisterGlobalFunction("void show(int)", AngelScript::asFUNCTION(OutInt), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(uint)", AngelScript::asFUNCTION(OutUInt), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(int64)", AngelScript::asFUNCTION(OutInt64), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(uint64)", AngelScript::asFUNCTION(OutUInt64), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(bool)", AngelScript::asFUNCTION(OutBool), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(float)", AngelScript::asFUNCTION(OutFloat), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(double)", AngelScript::asFUNCTION(OutDouble), AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void show(const string &in)", AngelScript::asFUNCTION(OutString), AngelScript::asCALL_CDECL);
    }

    // A binding reached through the *generic* calling convention, which is the one every `log::*` and
    // `thread::*` in the menu is registered with and which nothing in this component had ever exercised
    // - the benchmark's own note says its stubs stand in for natives registered `asCALL_CDECL` "or
    // `asCALL_GENERIC`", and until this existed none of them was the second.
    //
    // It is variadic as well, because that is the shape the menu's loggers take and because the engine
    // takes a separate branch for it: the generic dispatch builds an `asCGenericVariadic` rather than an
    // `asCGeneric` (as_context.cpp:5865) and computes the pop size from the argument count the call
    // actually carried. So one binding puts two things under the engine that no other binding here does,
    // and the second is not on the menu's own surface - `asCALL_CDECL` cannot be variadic at all
    // (as_scriptengine.cpp:2986 refuses it), so generic is the only way the shape exists.
    //
    // `...` is AngelScript's spelling for "the last parameter is the variadic one", and it replaces that
    // parameter's *name* rather than following it (as_parser.cpp:783). The declaration therefore reads
    // `int count` and then an unnamed trailing parameter; the call site is `VariadicTrace(3, a, b, c)`,
    // and the count has to come from the script, because the extra arguments arrive with nothing that
    // says how many there are.
    int g_variadic_total = 0;

    void VariadicTrace(AngelScript::asIScriptGeneric* generic) {
      const int count = static_cast<int>(generic->GetArgDWord(0));

      int total = 0;
      for (int i = 0; i < count; i++)
        total += static_cast<int>(generic->GetArgDWord(1 + static_cast<unsigned int>(i)));

      g_variadic_total += total;
      generic->SetReturnDWord(static_cast<AngelScript::asDWORD>(total));
    }

    void RegisterVariadic(AngelScript::asIScriptEngine* engine) {
      engine->RegisterGlobalFunction("int VariadicTrace(int count, int ...)", AngelScript::asFUNCTION(VariadicTrace),
                                     AngelScript::asCALL_GENERIC);
    }

    std::uint64_t g_line_callbacks = 0;

    // -------------------------------------------------------------------------
    // The POD value types
    // -------------------------------------------------------------------------
    //
    // The menu registers `Vector3` and its relatives as `asOBJ_VALUE | asOBJ_POD` value types with no
    // copy behaviour, and that registration is the only thing that makes `asBC_COPY` reachable at all.
    // A class declared *in* a script does not: AngelScript gives every script class an auto-generated
    // copy operator, so assigning one calls that instead, and the instruction never appears in the
    // bytecode. Only a host type marked asOBJ_POD, with no copy operator of its own, gets the bitwise
    // copy - which is the one translation that uses the string instructions and therefore the only
    // thing that makes the prologue's conditional save of rdi and rsi do any work.
    //
    // No assignment behaviour is registered below, which is deliberate rather than an omission: it is
    // the reason the copy is `asBC_COPY` instead of a call, and adding one would quietly remove the
    // only coverage this type exists to provide.
    struct Vec3 {
      float x;
      float y;
      float z;
    };

    void Vec3DefaultConstructor(Vec3* self) {
      self->x = 0.0f;
      self->y = 0.0f;
      self->z = 0.0f;
    }

    void Vec3ComponentsConstructor(float x, float y, float z, Vec3* self) {
      self->x = x;
      self->y = y;
      self->z = z;
    }

    float Vec3GetX(const Vec3& self) { return self.x; }

    // A native that returns an object **by reference** rather than by value or by handle.
    //
    // The three are separate branches of the compiler's call code, and this is the rarest of them: a
    // function whose return type is a reference to an object pushes the returned pointer with
    // `asBC_PshRPtr` (as_compiler.cpp:17543), where one returning a handle takes the object-register
    // path instead. A menu native handing back a reference to something it owns - a component of a
    // global, an entry in a table - is this shape, so leaving it out would leave the boundary's third
    // form untested.
    Vec3 g_global_vec{4.0f, 5.0f, 6.0f};

    Vec3& GetGlobalVec() { return g_global_vec; }

    // The other end of the size range: large enough that the translator hands the copy to `rep movsb`
    // instead of unrolling it. The two members that can be reached from a script are at opposite ends
    // of the object and everything between them is padding, which is what makes this a test of the
    // copy *length* rather than of the copy: a translation that moved only the first few bytes would
    // pass on `head` and fail on `tail`, and one that moved only the last few would do the reverse.
    struct Block {
      float head;
      float padding[30];
      float tail;
    };

    // The reason `Block` exists is its size: it is larger than `Emitter::kInlineCopyBytes`, so the
    // copy of it goes to `rep movsb` rather than being unrolled. A type that shrank below that limit -
    // by a padding change, say - would quietly leave half the translation unexercised with every test
    // still passing. The two are a pair, in the way the prologue's pushes and `emit_exit`'s pops are:
    // a change to either has to be checked against the other.
    static_assert(sizeof(Block) > 64, "Block must stay larger than the unrolled-copy limit in emitter.cpp");

    void BlockDefaultConstructor(Block* self) {
      std::memset(self, 0, sizeof(Block));
    }

    // A value type that can fail, and a method on it with the one signature `asBC_Thiscall1` exists
    // for. Both exist to make an *exception raised from inside a native* reachable from a script,
    // which is the only way to exercise the cleanup paths that depend on the callee having raised.
    //
    // Why this needs a type of its own rather than a script class that throws: a script exception is
    // raised by `asIScriptContext::SetException`, and that call is gated on the context believing it
    // is inside a system function - it returns `asERROR` and does nothing otherwise (as_context.cpp:
    // 4985). A script that does `throw` never goes through it, and a native that throws a C++
    // exception is converted by the engine rather than by this. So reaching the gate at all requires a
    // registered function that calls back into the context, which needs the context, which is what
    // `g_running_context` is for.
    //
    // `int &Get(int)` is the exact shape `as_compiler.cpp:17433` looks for when it decides between
    // `asBC_Thiscall1` and `asBC_CALLSYS`: an object method whose return type is a reference and whose
    // single parameter is a four-byte integer. A negative argument is the signal to raise, so a script
    // can choose per call whether the native succeeds or fails.
    //
    // `kValue` rather than `kPodValue`: a POD type carries no copy behaviour, and the reference this
    // returns has to be to something that outlives the call - hence the static below.
    struct Fault {
      int value;
    };

    int g_fault_result = 0;

    void FaultConstructor(int value, Fault* self) {
      self->value = value;
      // The constructor takes the same signal, so that a *construction* can be made to fail. That is
      // what reaches the tail of a heap-allocated value type's `asBC_ALLOC`, the one branch that hands
      // the fresh memory back to the allocator instead of storing its address.
      if (value < 0 && g_running_context != nullptr)
        g_running_context->SetException("fault: constructed with a negative value");
    }

    int& FaultGet(Fault* self, int argument) {
      if (argument < 0 && g_running_context != nullptr)
        g_running_context->SetException("fault: method called with a negative argument");

      g_fault_result = self->value + argument;
      return g_fault_result;
    }

    // Returns `Block` by value, and is a *method* rather than a free function. Both halves matter to
    // the compiler, and separately: a type this large is returned through a caller-supplied buffer
    // rather than in registers, so the callee is handed the destination address - and because the call
    // also has an object context, that address and the `this` pointer end up on the stack in the wrong
    // order and have to be exchanged (`asBC_SwapPtr`, as_compiler.cpp:17359). A free function returning
    // the same type would reach the first branch and not the second.
    Block BlockCopy(const Block& self) { return self; }

    // asOBJ_APP_CLASS rather than the traits the menu derives: these are plain C structures whose C++
    // constructors, destructors and assignments are all trivial, so there is nothing for the engine
    // to call on the native side. That is a statement about *these* types, and writing it as a trait
    // lookup would be asking the compiler the same question in a form that is harder to read.
    void RegisterValueTypes(AngelScript::asIScriptEngine* engine) {
      constexpr auto kPodValue = AngelScript::asOBJ_VALUE | AngelScript::asOBJ_POD | AngelScript::asOBJ_APP_CLASS;

      engine->RegisterObjectType("Vec3", sizeof(Vec3), kPodValue);
      engine->RegisterObjectProperty("Vec3", "float x", static_cast<int>(offsetof(Vec3, x)));
      engine->RegisterObjectProperty("Vec3", "float y", static_cast<int>(offsetof(Vec3, y)));
      engine->RegisterObjectProperty("Vec3", "float z", static_cast<int>(offsetof(Vec3, z)));
      engine->RegisterObjectBehaviour("Vec3", AngelScript::asBEHAVE_CONSTRUCT, "void f()",
                                      AngelScript::asFUNCTION(Vec3DefaultConstructor), AngelScript::asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour("Vec3", AngelScript::asBEHAVE_CONSTRUCT, "void f(float, float, float)",
                                      AngelScript::asFUNCTION(Vec3ComponentsConstructor), AngelScript::asCALL_CDECL_OBJLAST);
      // A method, so that a script can read a component through a `const Vec3&` - the shape a menu
      // native has, and one that reads the value where it lies rather than copying it first.
      engine->RegisterObjectMethod("Vec3", "float GetX() const",
                                   AngelScript::asFUNCTION(Vec3GetX), AngelScript::asCALL_CDECL_OBJFIRST);

      engine->RegisterGlobalFunction("Vec3& GetGlobalVec()", AngelScript::asFUNCTION(GetGlobalVec),
                                     AngelScript::asCALL_CDECL);

      engine->RegisterObjectType("Block", sizeof(Block), kPodValue);
      engine->RegisterObjectProperty("Block", "float head", static_cast<int>(offsetof(Block, head)));
      engine->RegisterObjectProperty("Block", "float tail", static_cast<int>(offsetof(Block, tail)));
      engine->RegisterObjectBehaviour("Block", AngelScript::asBEHAVE_CONSTRUCT, "void f()",
                                      AngelScript::asFUNCTION(BlockDefaultConstructor), AngelScript::asCALL_CDECL_OBJLAST);
      engine->RegisterObjectMethod("Block", "Block Copy() const",
                                   AngelScript::asFUNCTION(BlockCopy), AngelScript::asCALL_CDECL_OBJFIRST);

      engine->RegisterObjectType("Fault", sizeof(Fault), kPodValue);
      engine->RegisterObjectProperty("Fault", "int value", static_cast<int>(offsetof(Fault, value)));
      engine->RegisterObjectBehaviour("Fault", AngelScript::asBEHAVE_CONSTRUCT, "void f(int)",
                                      AngelScript::asFUNCTION(FaultConstructor), AngelScript::asCALL_CDECL_OBJLAST);
      engine->RegisterObjectMethod("Fault", "int &Get(int)",
                                   AngelScript::asFUNCTION(FaultGet), AngelScript::asCALL_CDECL_OBJFIRST);
    }

    // -------------------------------------------------------------------------
    // The no-count reference type
    // -------------------------------------------------------------------------
    //
    // `asOBJ_REF | asOBJ_NOCOUNT` is the one registration that makes the cheap half of the reference
    // copies reachable, and without it that half is unreachable in this suite rather than merely
    // uncommon. The interpreter's handlers for `asBC_REFCPY` and `asBC_RefCpyV` skip the addref and
    // the release entirely for a type carrying either `asOBJ_NOCOUNT` or `asOBJ_VALUE` and reduce to a
    // single pointer store. Every handle a script here created before this type existed was to a
    // script class, a funcdef or an array - `asOBJ_SCRIPT_OBJECT`, `asOBJ_APP_CLASS` - and all of
    // those are counted, so the branch was never taken. A translation of that branch with no test is
    // the shape this codebase treats as worse than no translation: a silent wrong answer waiting for
    // the first no-count type someone registers in the menu.
    //
    // A no-count type is a reference type the engine has been told not to manage - the host owns the
    // lifetime and a script only ever holds a pointer. That is exactly why the two objects below are
    // statics: nothing may free them, and a handle a script was handed has to still be readable here
    // once the script has finished with it.
    //
    // The `int` member exists so that one instance can be told from the other, which is what gives an
    // assignment something observable to get wrong.
    struct Handle {
      int value;
    };

    Handle g_handle_one{1};
    Handle g_handle_two{2};

    // A factory rather than a constructor: a reference type is brought into being by a function
    // returning a handle, not by a constructor running on a value. `asBEHAVE_FACTORY` is the
    // behaviour for that, and returning one of two statics is the whole of it.
    Handle* HandleFrom(int which) { return which == 1 ? &g_handle_one : &g_handle_two; }

    int HandleValue(const Handle& self) { return self.value; }

    // The two natives that make a handle cross a *native* boundary, which is a different code path from
    // a handle crossing a script-function boundary and the one the menu is built on.
    //
    // `TakeHandle` is what a menu writes as `DeleteVehicle(Vehicle@ v)` and `ReturnHandle` is what it
    // writes as `GetPlayerPed(Player@ p)`. Neither is decoration: an object handle passed to or from a
    // system function is marshalled by the compiler into a temporary block (`asBC_GETOBJ`,
    // as_compiler.cpp:2378) and a handle returned from one takes the object-register path (the
    // `PopRPtr` in the same routine's return handling). Both are emitter cases the suite had never
    // reached, because until these existed no script here handed a handle to a native.
    //
    // `g_last_handle` is what makes the passing observable - a native that discarded its argument could
    // be optimised into a no-op and would prove nothing.
    Handle* g_last_handle = nullptr;

    void TakeHandle(Handle* handle) { g_last_handle = handle; }

    Handle* ReturnHandle(Handle* handle) { return handle; }

    // The third way a handle crosses a native boundary, and the one the other two do not cover: taking
    // the object *by reference* where the script has a handle to it.
    //
    // `Handle@` and `const Handle &in` are different parameter kinds to the compiler. A handle argument
    // is passed as the pointer itself; a reference argument means the callee is handed the object, so a
    // handle has to be dereferenced first, which the compiler guards with `asBC_ChkRefS` rather than
    // the `asBC_ChkNullS` the source comment at as_compiler.cpp:2412 still names. The menu writes
    // natives both ways, so the boundary has two shapes and this is the second of them.
    //
    // `g_observed_handle` is what makes it observable for the same reason `g_last_handle` is: a native
    // that threw its argument away would prove nothing about whether the argument arrived.
    int g_observed_handle = 0;

    void ObserveHandle(const Handle& handle) { g_observed_handle = handle.value; }

    // A conversion from a handle to a value type, and the only shape in this suite that reaches
    // `asBC_SwapPtr` - an instruction the emitter has a case for that nothing had ever produced.
    //
    // A value type too large for the registers comes back through a buffer the caller supplies, so a
    // call that returns one is handed two addresses: the object it is being called on, and where the
    // result is to go. The conversion below is reached with the source's address already on the stack,
    // so the buffer's address lands *above* it and the two have to be exchanged before the call
    // (as_compiler.cpp:8080) - the callee reads them positionally, and the wrong order hands it the
    // result buffer as its object.
    //
    // An optimizing engine does not emit it, which is why the differential test that uses this runs
    // with `optimize_bytecode` off. The peephole that deletes a `SwapPtr` fires when the two
    // instructions below it are `PSF`/`PshVPtr`/`PshNull` (as_bytecode.cpp:305), and for this shape
    // they are the object's own `PSF` and the buffer's - so the pair is exchanged at compile time and
    // the run-time instruction is never reached. Measured rather than assumed: the same test with the
    // optimizer on reports the opcode was never compiled.
    //
    // `opImplConv` rather than `opConv`, so that a script can write `Vec3 v = h;`: an implicit
    // conversion is what a plain assignment asks for, and it is the shorter thing to write.
    Vec3 HandleToVec3(const Handle& self) {
      return Vec3{float(self.value), float(self.value) * 2.0f, 0.0f};
    }

    void RegisterNoCountTypes(AngelScript::asIScriptEngine* engine) {
      constexpr auto kNoCountRef = AngelScript::asOBJ_REF | AngelScript::asOBJ_NOCOUNT;

      // Size 0, which is what the engine expects of a reference type it does not allocate.
      engine->RegisterObjectType("Handle", 0, kNoCountRef);
      engine->RegisterObjectBehaviour("Handle", AngelScript::asBEHAVE_FACTORY, "Handle@ f(int)",
                                      AngelScript::asFUNCTION(HandleFrom), AngelScript::asCALL_CDECL);
      engine->RegisterObjectMethod("Handle", "int Value() const",
                                   AngelScript::asFUNCTION(HandleValue), AngelScript::asCALL_CDECL_OBJFIRST);

      // A property as well as the method above, and the two have different jobs. The method is what the
      // differential test reads the handle through, because a call is the honest way to ask a handle
      // what it points at. The property is what the *benchmark* row reads through: reading a member is
      // a load and a displacement, where reading through a method is `asBC_CALLSYS` - and a call ends
      // the translated block, so a benchmark loop that read through the method would break at the call
      // and could never show what translating the assignment around it bought. Making the observation
      // call-free is what lets the row measure the instruction it names.
      engine->RegisterObjectProperty("Handle", "int value", static_cast<int>(offsetof(Handle, value)));

      engine->RegisterGlobalFunction("void TakeHandle(Handle@)", AngelScript::asFUNCTION(TakeHandle),
                                     AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("Handle@ ReturnHandle(Handle@)", AngelScript::asFUNCTION(ReturnHandle),
                                     AngelScript::asCALL_CDECL);
      engine->RegisterGlobalFunction("void ObserveHandle(const Handle &in)",
                                     AngelScript::asFUNCTION(ObserveHandle), AngelScript::asCALL_CDECL);

      engine->RegisterObjectMethod("Handle", "Vec3 opImplConv() const",
                                   AngelScript::asFUNCTION(HandleToVec3), AngelScript::asCALL_CDECL_OBJFIRST);
    }

    // Deliberately does nothing but count. A benchmark arm that installs this needs the callback
    // itself to cost the same on both sides, so that the difference between them is the JIT's
    // per-block entry overhead rather than whatever the callback did.
    void CountingLineCallback(AngelScript::asIScriptContext* /*context*/, void* /*userData*/) {
      ++g_line_callbacks;
    }
  } // namespace

  AngelScript::asIScriptEngine* CreateEngine(const EngineConfig& config) {
    auto* engine = AngelScript::asCreateScriptEngine();
    if (engine == nullptr)
      return nullptr;

    engine->SetMessageCallback(AngelScript::asFUNCTION(MessageCallback), nullptr, AngelScript::asCALL_CDECL);

    // The properties that decide what the bytecode looks like. Set identically in both arms of a
    // comparison, which is what makes the comparison meaningful.
    engine->SetEngineProperty(AngelScript::asEP_INCLUDE_JIT_INSTRUCTIONS, config.include_jit_instructions);
    engine->SetEngineProperty(AngelScript::asEP_OPTIMIZE_BYTECODE, config.optimize_bytecode);
    engine->SetEngineProperty(AngelScript::asEP_INIT_GLOBAL_VARS_AFTER_BUILD, true);
    engine->SetEngineProperty(AngelScript::asEP_NO_DEBUG_OUTPUT, true);

    if (config.disable_integer_division)
      engine->SetEngineProperty(AngelScript::asEP_DISABLE_INTEGER_DIVISION, true);

    // The three container types the menu exposes to scripts, registered here in the global namespace
    // rather than under `std` - the menu sets a default namespace first, which changes how a script
    // *writes* `string` but not what the engine compiles it to, and a workload here is not made more
    // faithful by taking on a spelling the bytecode does not see.
    //
    // Dictionary is registered for the benchmark's sake: every operation on it is a method call, and
    // a key lookup is a shape menu scripts are full of. Leaving it out would have made the suite's
    // picture of "what a script does with a container" an incomplete one, which is the wrong kind of
    // omission in a benchmark whose whole output is a table of ratios.
    if (config.register_add_ons) {
      RegisterStdString(engine);
      RegisterScriptArray(engine, true);
      RegisterScriptDictionary(engine);
    }

    if (config.register_value_types)
      RegisterValueTypes(engine);

    if (config.register_nocount_types)
      RegisterNoCountTypes(engine);

    RegisterOutput(engine);
    RegisterVariadic(engine);

    if (config.use_jit) {
      // The options are read when a function is compiled rather than when one runs, and the module is
      // built after this returns - so they are in place for the first function the engine offers,
      // which is the only ordering that makes the flag mean what it says.
      asjit::Options options;
      options.translate_returns = config.translate_returns;
      options.translate_object_method_calls = config.translate_object_method_calls;
      asjit::Attach(engine, options);
    }

    return engine;
  }

  void ClearOutput() { g_output.clear(); }
  const std::string& Output() { return g_output; }

  void ClearErrors() { g_errors.clear(); }
  const std::string& Errors() { return g_errors; }

  Script Build(AngelScript::asIScriptEngine* engine, const char* module_name, const std::string& source, const char* entry_decl) {
    Script script;

    AngelScript::CScriptBuilder builder;
    builder.StartNewModule(engine, module_name);

    if (builder.AddSectionFromMemory(module_name, source.c_str(), static_cast<unsigned int>(source.size())) < 0)
      return script;
    if (builder.BuildModule() < 0)
      return script;

    script.module = builder.GetModule();
    script.built = script.module != nullptr;

    if (script.built)
      script.entry = script.module->GetFunctionByDecl(entry_decl);

    return script;
  }

  Execution Execute(AngelScript::asIScriptFunction* entry, bool line_callback) {
    Execution execution;

    if (entry == nullptr)
      return execution;

    auto* context = entry->GetEngine()->CreateContext();
    if (context == nullptr)
      return execution;

    if (line_callback)
      context->SetLineCallback(AngelScript::asFUNCTION(CountingLineCallback), nullptr, AngelScript::asCALL_CDECL);

    if (context->Prepare(entry) >= 0) {
      execution.prepared = true;
      g_running_context = context;
      execution.result = context->Execute();
      g_running_context = nullptr;
      if (execution.result == AngelScript::asEXECUTION_EXCEPTION) {
        const char* text = context->GetExceptionString();
        execution.exception = text != nullptr ? text : "";
      }
    }

    context->Release();
    return execution;
  }

  void ResetLineCallbackCount() { g_line_callbacks = 0; }
  std::uint64_t LineCallbackCount() { return g_line_callbacks; }

  RunOutcome RunScript(const std::string& source, bool use_jit, const char* entry) {
    EngineConfig config;
    config.use_jit = use_jit;
    return RunScriptWithConfig(source, config, entry);
  }

  RunOutcome RunScriptWithConfig(const std::string& source, const EngineConfig& config, const char* entry) {
    RunOutcome outcome;

    ClearOutput();
    ClearErrors();

    // Per run rather than cumulative, so that a caller can ask what this one script cost. Reset
    // before the engine exists, since the counters are process-wide and the previous run's engine may
    // still have been shutting down when it last reported.
    asjit::ResetStats();

    auto* engine = CreateEngine(config);
    if (engine == nullptr) {
      outcome.errors = "asCreateScriptEngine failed";
      return outcome;
    }

    {
      const auto script = Build(engine, "asjit_run", source, entry);
      outcome.built = script.built;

      if (!script.built) {
        outcome.errors = Errors();
        engine->ShutDownAndRelease();
        return outcome;
      }

      if (script.entry != nullptr) {
        outcome.entry_found = true;
        outcome.entry_has_jit = script.entry->GetJITFunction() != nullptr;

        // `config.line_callback` is passed on rather than assumed, so that the configuration a caller
        // wrote down is the configuration it gets. This helper read the field nowhere until a test
        // needed the watchdog configuration: a config with the flag set ran without a callback, both
        // arms agreed on a count of zero, and the comparison said nothing about the path it was for.
        ResetLineCallbackCount();
        const auto execution = Execute(script.entry, config.line_callback);
        outcome.execution_result = execution.result;
        outcome.exception_string = execution.exception;
        outcome.line_callbacks = LineCallbackCount();
      }

      // Read before the engine goes away, because shutting it down is what releases the compiled
      // functions and would otherwise move the counters between the run and the report of it.
      const auto stats = asjit::GetStats();
      outcome.functions_seen = stats.functions_seen;
      outcome.functions_compiled = stats.functions_compiled;
      outcome.functions_declined = stats.functions_declined;
      outcome.blocks_emitted = stats.blocks_emitted;
      outcome.instructions_translated = stats.instructions_translated;
      outcome.instructions_total = stats.instructions_total;
      outcome.code_size = stats.code_size;
      outcome.opcodes_walked = stats.opcodes_walked;
      outcome.opcodes_translated = stats.opcodes_translated;
      outcome.opcodes_unimplemented = stats.opcodes_unimplemented;

      outcome.output = Output();
    }

    engine->ShutDownAndRelease();
    return outcome;
  }

} // namespace base::asjit::support
