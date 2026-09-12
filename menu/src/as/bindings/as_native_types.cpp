//
// Created by X-ray on 11/09/2026.
//

#include "as_native_types.hpp"

#include "../util/as_bind.hpp"

#include "../../game/native_types.hpp"
#include "../../natives/natives_as.hpp"

#include <base-common/conversion/joaat.hpp>
#include <rage/vector.hpp>

#include <cstdint>
#include <string>

// Every registration below goes through the helpers in as_bind.hpp, which derive the AngelScript
// declaration and the thunk that has to match it from the same C++ source. That is not a convenience:
// a declaration and a thunk that disagree compile cleanly and corrupt memory at runtime, which is how
// this file used to carry two of them - opMul declared "float" against a member taking const float&,
// and four Vector3-returning operators on the thiscall path, where x64 MSVC hands the object pointer
// and the hidden return pointer in the opposite order to MSVC's own convention.
namespace base::menu::as::bindings::native_types {
  namespace {
    namespace AS = AngelScript;

    // The AngelScript namespace the generated bindings live in. Spelled out from the global scope
    // because base::menu::natives shadows it from inside this file. Taken from the generated header
    // rather than repeated as a literal so it cannot drift from the generator's config.
    inline constexpr const char* kNamespace = ::natives::kNamespace;

    // Also from base::menu::natives, and named so the two never look alike at a use site.
    using Hash = natives::Hash;

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
    void HashFromStringConstructor(const std::string& value, Hash* self) {
      *self = common::conversion::joaat(value.c_str(), value.size());
    }

    void RegisterVectors(AS::asIScriptEngine* engine) {
      util::RegisterValueType<rage::Vector2>(engine, "Vector2")
        .Desc("A two component float vector.");
      util::RegisterProperty<&rage::Vector2::x>(engine, "Vector2", "x").Desc("The x component.");
      util::RegisterProperty<&rage::Vector2::y>(engine, "Vector2", "y").Desc("The y component.");
      // Both constructors key their docs under "Vector2::f", so the block written by the one with
      // parameters is the block that documents the whole family, as with any other overload set.
      util::RegisterConstructor<rage::Vector2>(engine, "Vector2");
      util::RegisterConstructor<rage::Vector2, float, float>(engine, "Vector2", {"x", "y"})
        .Desc("A vector with the given components.")
        .Param("x", "The x component.")
        .Param("y", "The y component.");

      util::RegisterValueType<rage::Vector3>(engine, "Vector3")
        .Desc("A three component float vector. A native returning one writes it straight into a script variable.");
      util::RegisterProperty<&rage::Vector3::x>(engine, "Vector3", "x").Desc("The x component.");
      util::RegisterProperty<&rage::Vector3::y>(engine, "Vector3", "y").Desc("The y component.");
      util::RegisterProperty<&rage::Vector3::z>(engine, "Vector3", "z").Desc("The z component.");
      util::RegisterConstructor<rage::Vector3>(engine, "Vector3");
      util::RegisterConstructor<rage::Vector3, float, float, float>(engine, "Vector3", {"x", "y", "z"})
        .Desc("A vector with the given components.")
        .Param("x", "The x component.")
        .Param("y", "The y component.")
        .Param("z", "The z component.");

      // The members are named through constants rather than written inline. A member operator written
      // directly in a template argument list does not always survive lexing: the "->" that would
      // follow operator- is one token, so `RegisterMethod<&Vector3::operator->` parses as operator->.
      // The two operator* overloads need the cast anyway, and naming them all reads consistently.
      using Vector3 = rage::Vector3;
      constexpr auto kGetMagnitude = &Vector3::GetMagnitude;
      constexpr auto kGetDistance = &Vector3::GetDistance;
      constexpr auto kEquals = &Vector3::operator==;
      constexpr auto kAdd = &Vector3::operator+;
      constexpr auto kSubtract = &Vector3::operator-;
      constexpr auto kMultiplyByVector = static_cast<Vector3 (Vector3::*)(const Vector3&) const>(&Vector3::operator*);
      constexpr auto kMultiplyByScalar = static_cast<Vector3 (Vector3::*)(const float&) const>(&Vector3::operator*);

      util::RegisterMethod<kGetMagnitude>(engine, "Vector3", "GetMagnitude")
        .Desc("Length of the vector.")
        .Returns("sqrt(x*x + y*y + z*z).");
      util::RegisterMethod<kGetDistance>(engine, "Vector3", "GetDistance", {"other"})
        .Desc("Distance from this vector to another one.")
        .Param("other", "The other vector.")
        .Returns("The length of the difference between the two.");
      util::RegisterMethod<kEquals>(engine, "Vector3", "opEquals", {"other"})
        .Desc("True when all three components are equal.")
        .Param("other", "The vector to compare against.");
      util::RegisterMethod<kAdd>(engine, "Vector3", "opAdd", {"other"})
        .Desc("Component-wise sum.")
        .Param("other", "The vector to add.");
      util::RegisterMethod<kSubtract>(engine, "Vector3", "opSub", {"other"})
        .Desc("Component-wise difference.")
        .Param("other", "The vector to subtract.");
      // The two opMul overloads share one AngelScript name, and docs are keyed by name rather than by
      // signature, so the block below belongs to the pair: both parameters are named on the first
      // registration and the second contributes none, or the second Desc would overwrite the first.
      //
      // The scalar overload is also the reason the thunk cannot simply mirror the member: "float" is
      // passed in a register, so the declaration says by value where the member takes a const
      // reference. Deriving both from the member pointer is what keeps the two in step.
      util::RegisterMethod<kMultiplyByVector>(engine, "Vector3", "opMul", {"other"})
        .Desc("Scales every component. With a vector the product is component-wise, not the dot or cross product.")
        .Param("other", "The vector to multiply by.")
        .Param("scale", "The scale factor, on the overload taking a single float.");
      util::RegisterMethod<kMultiplyByScalar>(engine, "Vector3", "opMul", {"scale"});

      util::RegisterValueType<rage::Vector4>(engine, "Vector4")
        .Desc("A four component float vector.");
      util::RegisterProperty<&rage::Vector4::x>(engine, "Vector4", "x").Desc("The x component.");
      util::RegisterProperty<&rage::Vector4::y>(engine, "Vector4", "y").Desc("The y component.");
      util::RegisterProperty<&rage::Vector4::z>(engine, "Vector4", "z").Desc("The z component.");
      util::RegisterProperty<&rage::Vector4::w>(engine, "Vector4", "w").Desc("The w component.");
      util::RegisterConstructor<rage::Vector4>(engine, "Vector4");
      util::RegisterConstructor<rage::Vector4, float, float, float, float>(engine, "Vector4", {"x", "y", "z", "w"})
        .Desc("A vector with the given components.")
        .Param("x", "The x component.")
        .Param("y", "The y component.")
        .Param("z", "The z component.")
        .Param("w", "The w component.");
    }
  }

  void Register(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace(kNamespace);

    // No flags to pass: Hash is a std::uint32_t, so asGetTypeTraits already reports asOBJ_APP_PRIMITIVE.
    util::RegisterValueType<Hash>(engine, "Hash")
      .Desc("A 32 bit joaat hash. A string converts to one on its own, so a native expecting a hash can be "
            "called with \"adder\" rather than a numeric literal.");
    util::RegisterConstructor<Hash>(engine, "Hash");
    util::RegisterConstructor<Hash, std::uint32_t>(engine, "Hash", {"value"})
      .Desc("A hash with the given value.")
      .Param("value", "The numeric value of the hash.");
    util::RegisterConstructor<Hash, const std::string&>(engine, "Hash", &HashFromStringConstructor, {"value"})
      .Desc("Hashes the string with joaat, the hash the game itself uses for names.")
      .Param("value", "The string to hash.");

    // Natives taking Any* take a pointer into memory the game owns, which a script has no way to
    // fabricate. A handle type keeps the parameter honest and lets a script pass null, where an int
    // would have pretended the pointer was a number.
    util::RegisterObjectType(engine, "NativeHandle", 0, AS::asOBJ_REF | AS::asOBJ_NOCOUNT)
      .Desc("An opaque pointer to game-owned memory. A script can only pass null.");

    RegisterVectors(engine);

    engine->SetDefaultNamespace("");
  }
}
