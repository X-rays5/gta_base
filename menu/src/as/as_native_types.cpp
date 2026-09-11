//
// Created by X-ray on 11/09/2026.
//

#include "as_native_types.hpp"

#include "as_bind.hpp"

#include "../game/native_types.hpp"
#include "../natives/natives_as.hpp"

#include <base-common/conversion/joaat.hpp>
#include <rage/vector.hpp>

#include <cstdint>
#include <new>
#include <string>

// AS_USE_NAMESPACE is set for this target, so every engine symbol has to be qualified. The
// method macros are the exception: asMETHOD and asMETHODPR expand to an expression starting with
// asSMethodPtr, and asOFFSET to a parenthesised cast, so a AS:: prefix is a syntax error on them.
// Bring the one name the method macros need into scope instead - unqualified lookup finds it, and
// the expansion stays verbatim.
namespace base::menu::as::native_types {
  namespace {
    namespace AS = AngelScript;

    using AS::asSMethodPtr;

    // The AngelScript namespace the generated bindings live in. Spelled out from the global scope
    // because base::menu::natives shadows it from inside this file. Taken from the generated header
    // rather than repeated as a literal so it cannot drift from the generator's config.
    inline constexpr const char* kNamespace = ::natives::kNamespace;

    // Also from base::menu::natives, and named so the two never look alike at a use site.
    using Hash = base::menu::natives::Hash;

    // Constructors are registered through asBEHAVE_CONSTRUCT with asCALL_CDECL_OBJLAST, the shape the
    // scriptmath add-on uses, so the object arrives as the trailing argument. as_bind.hpp has no
    // wrapper for RegisterObjectBehaviour, so these are registered raw and carry no docs.

    void Vector2DefaultConstructor(rage::Vector2* self) { new (self) rage::Vector2(); }
    void Vector2InitConstructor(const float x, const float y, rage::Vector2* self) { new (self) rage::Vector2(x, y); }

    void Vector3DefaultConstructor(rage::Vector3* self) { new (self) rage::Vector3(); }
    void Vector3InitConstructor(const float x, const float y, const float z, rage::Vector3* self) {
      new (self) rage::Vector3(x, y, z);
    }

    void Vector4DefaultConstructor(rage::Vector4* self) { new (self) rage::Vector4(); }
    void Vector4InitConstructor(const float x, const float y, const float z, const float w, rage::Vector4* self) {
      new (self) rage::Vector4(x, y, z, w);
    }

    // Hash is a std::uint32_t underneath and is registered with asOBJ_APP_PRIMITIVE, which makes its
    // ABI a 4-byte value passed and returned in a register - identical to the natives that genuinely
    // take a uint32_t.
    //
    // The string constructor is the entire reason Hash is a type of its own. AngelScript looks for a
    // one-argument conversion constructor on the target type when it converts an argument, so
    // GetVehicleModel("adder") works with no per-native overloads. That lookup only succeeds when
    // exactly one constructor matches, which is why there is deliberately no copy constructor here:
    // one would also match a string argument and silently disable the conversion. asOBJ_POD gives
    // bitwise copy instead.
    void HashDefaultConstructor(Hash* self) { *self = 0; }
    void HashFromUintConstructor(const std::uint32_t value, Hash* self) { *self = value; }
    void HashFromStringConstructor(const std::string& in, Hash* self) {
      *self = common::conversion::joaat(in.c_str(), in.size());
    }

    // AngelScript's x64 MSVC thiscall path puts the object pointer in the first slot and the hidden
    // return pointer in the second, but MSVC's own convention is the other way round. A method that
    // returns a value type too wide for registers therefore reads `this` out of the return buffer and
    // faults. Anything returning a Vector3 goes through a free function registered CDECL_OBJFIRST
    // instead, which takes the same plain-CDECL return path the natives are registered with.
    //
    // The parameter types have to match the declaration rather than the C++ members: `float` is
    // passed in a register, so these take it by value, while a reference parameter arrives as a
    // pointer.
    rage::Vector3 Vector3Add(const rage::Vector3& self, const rage::Vector3& other) { return self + other; }
    rage::Vector3 Vector3Sub(const rage::Vector3& self, const rage::Vector3& other) { return self - other; }
    rage::Vector3 Vector3MulVector(const rage::Vector3& self, const rage::Vector3& other) { return self * other; }
    rage::Vector3 Vector3MulFloat(const rage::Vector3& self, const float other) { return self * other; }

    void RegisterVectors(AS::asIScriptEngine* engine) {
      RegisterObjectType(engine, "Vector2", sizeof(rage::Vector2),
                         AS::asOBJ_VALUE | AS::asOBJ_POD | AS::asGetTypeTraits<rage::Vector2>())
        .Desc("A two component float vector.");
      RegisterObjectProperty(engine, "Vector2", "float x", asOFFSET(rage::Vector2, x)).Desc("The x component.");
      RegisterObjectProperty(engine, "Vector2", "float y", asOFFSET(rage::Vector2, y)).Desc("The y component.");
      engine->RegisterObjectBehaviour("Vector2", AS::asBEHAVE_CONSTRUCT, "void f()", AS::asFUNCTION(Vector2DefaultConstructor),
                                      AS::asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour("Vector2", AS::asBEHAVE_CONSTRUCT, "void f(float, float)",
                                      AS::asFUNCTION(Vector2InitConstructor), AS::asCALL_CDECL_OBJLAST);

      RegisterObjectType(engine, "Vector3", sizeof(rage::Vector3),
                         AS::asOBJ_VALUE | AS::asOBJ_POD | AS::asGetTypeTraits<rage::Vector3>())
        .Desc("A three component float vector. A native returning one writes it straight into a script variable.");
      RegisterObjectProperty(engine, "Vector3", "float x", asOFFSET(rage::Vector3, x)).Desc("The x component.");
      RegisterObjectProperty(engine, "Vector3", "float y", asOFFSET(rage::Vector3, y)).Desc("The y component.");
      RegisterObjectProperty(engine, "Vector3", "float z", asOFFSET(rage::Vector3, z)).Desc("The z component.");
      engine->RegisterObjectBehaviour("Vector3", AS::asBEHAVE_CONSTRUCT, "void f()", AS::asFUNCTION(Vector3DefaultConstructor),
                                      AS::asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour("Vector3", AS::asBEHAVE_CONSTRUCT, "void f(float, float, float)",
                                      AS::asFUNCTION(Vector3InitConstructor), AS::asCALL_CDECL_OBJLAST);

      RegisterObjectMethod(engine, "Vector3", "float GetMagnitude() const", asMETHOD(rage::Vector3, GetMagnitude),
                           AS::asCALL_THISCALL)
        .Desc("Length of the vector.")
        .Returns("sqrt(x*x + y*y + z*z).");
      RegisterObjectMethod(engine, "Vector3", "float GetDistance(const Vector3 &in) const",
                           asMETHODPR(rage::Vector3, GetDistance, (const rage::Vector3&) const, float),
                           AS::asCALL_THISCALL)
        .Desc("Distance from this vector to another one.")
        .Param("in", "The other vector.")
        .Returns("The length of the difference between the two.");
      RegisterObjectMethod(engine, "Vector3", "bool opEquals(const Vector3 &in) const",
                           asMETHODPR(rage::Vector3, operator==, (const rage::Vector3&) const, bool),
                           AS::asCALL_THISCALL)
        .Desc("True when all three components are equal.")
        .Param("in", "The vector to compare against.");
      RegisterObjectMethod(engine, "Vector3", "Vector3 opAdd(const Vector3 &in) const", AS::asFUNCTION(Vector3Add),
                           AS::asCALL_CDECL_OBJFIRST)
        .Desc("Component-wise sum.")
        .Param("in", "The vector to add.");
      RegisterObjectMethod(engine, "Vector3", "Vector3 opSub(const Vector3 &in) const", AS::asFUNCTION(Vector3Sub),
                           AS::asCALL_CDECL_OBJFIRST)
        .Desc("Component-wise difference.")
        .Param("in", "The vector to subtract.");
      RegisterObjectMethod(engine, "Vector3", "Vector3 opMul(const Vector3 &in) const", AS::asFUNCTION(Vector3MulVector),
                           AS::asCALL_CDECL_OBJFIRST)
        .Desc("Component-wise product, not the dot or cross product.")
        .Param("in", "The vector to multiply by.");
      RegisterObjectMethod(engine, "Vector3", "Vector3 opMul(float) const", AS::asFUNCTION(Vector3MulFloat),
                           AS::asCALL_CDECL_OBJFIRST)
        .Desc("Scales every component.")
        .Param("in", "The scale factor.");

      RegisterObjectType(engine, "Vector4", sizeof(rage::Vector4),
                         AS::asOBJ_VALUE | AS::asOBJ_POD | AS::asGetTypeTraits<rage::Vector4>())
        .Desc("A four component float vector.");
      RegisterObjectProperty(engine, "Vector4", "float x", asOFFSET(rage::Vector4, x)).Desc("The x component.");
      RegisterObjectProperty(engine, "Vector4", "float y", asOFFSET(rage::Vector4, y)).Desc("The y component.");
      RegisterObjectProperty(engine, "Vector4", "float z", asOFFSET(rage::Vector4, z)).Desc("The z component.");
      RegisterObjectProperty(engine, "Vector4", "float w", asOFFSET(rage::Vector4, w)).Desc("The w component.");
      engine->RegisterObjectBehaviour("Vector4", AS::asBEHAVE_CONSTRUCT, "void f()", AS::asFUNCTION(Vector4DefaultConstructor),
                                      AS::asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour("Vector4", AS::asBEHAVE_CONSTRUCT, "void f(float, float, float, float)",
                                      AS::asFUNCTION(Vector4InitConstructor), AS::asCALL_CDECL_OBJLAST);
    }
  }

  void Register(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace(kNamespace);

    RegisterObjectType(engine, "Hash", sizeof(Hash), AS::asOBJ_VALUE | AS::asOBJ_POD | AS::asOBJ_APP_PRIMITIVE)
      .Desc("A 32 bit joaat hash. A string converts to one on its own, so a native expecting a hash can be "
            "called with \"adder\" rather than a numeric literal.");
    engine->RegisterObjectBehaviour("Hash", AS::asBEHAVE_CONSTRUCT, "void f()", AS::asFUNCTION(HashDefaultConstructor),
                                    AS::asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Hash", AS::asBEHAVE_CONSTRUCT, "void f(uint)", AS::asFUNCTION(HashFromUintConstructor),
                                    AS::asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Hash", AS::asBEHAVE_CONSTRUCT, "void f(const std::string &in)",
                                    AS::asFUNCTION(HashFromStringConstructor), AS::asCALL_CDECL_OBJLAST);

    // Natives taking Any* take a pointer into memory the game owns, which a script has no way to
    // fabricate. A handle type keeps the parameter honest and lets a script pass null, where an int
    // would have pretended the pointer was a number.
    RegisterObjectType(engine, "NativeHandle", 0, AS::asOBJ_REF | AS::asOBJ_NOCOUNT)
      .Desc("An opaque pointer to game-owned memory. A script can only pass null.");

    RegisterVectors(engine);

    engine->SetDefaultNamespace("");
  }
}
