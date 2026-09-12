//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <cmath>

#pragma pack(push, 0x10)
namespace rage {
  class Vector2 {
  public:
    float x;
    float y;

    constexpr Vector2(const float x, const float y) :
      x(x),
      y(y) {}

    constexpr Vector2() :
      x(0.0f),
      y(0.0f) {}
  };

  /**
   * A three component float vector that occupies sixteen bytes, which is the size the game's script
   * call context reserves per vector it is handed (see vector_ref_sources_ in
   * rage/src/script/native_call_context.hpp, and the assert on that struct's own size).
   *
   * The sixteen bytes are deliberately reached with four byte alignment rather than with alignas(16).
   * A native returning a vector is given a destination pointer by AngelScript and stores straight into
   * it, and that destination is chosen by the AngelScript compiler, which packs a function's variables
   * at dword granularity - asCCompiler::GetVariableOffset sums the sizes and does nothing else. The
   * support for 16 byte locals it has is behind an undefined WIP_16BYTE_ALIGN, and its own TODO list
   * still asks for the compiler to allocate variables on 16 byte boundaries. A script local, and the
   * destination of a return, can therefore sit at any four byte offset. Claiming 16 byte alignment
   * makes MSVC emit movdqa for the whole-object copy into that destination, which faults on every
   * offset the engine is free to choose - an EXCEPTION_ACCESS_VIOLATION with a faulting address
   * ending in 8 or 4 that reports itself as coming from whichever GET_* native returned first.
   *
   * The padding is a member of the class rather than alignas so that the size stays 16 without the
   * alignment. It is not a float on purpose: a class of exactly three floats is one MSVC may return in
   * XMM registers after a type's ABI classification, and both AngelScript and the generated native
   * wrappers call this a type returned through a hidden pointer. A non-float member keeps it that way.
   *
   * Do not make this type alignas(16) again to "fix" alignment. There is nothing to fix here, and the
   * alignment is what causes the crash. The one place a 16 byte aligned Vector3 is required is the
   * call context the game writes into, and that alignment is asked for there.
   */
  class Vector3 {
  public:
    float x;
    float y;
    float z;

    constexpr Vector3(const float x, const float y, const float z) :
      x(x),
      y(y),
      z(z) {}

    constexpr Vector3() :
      x(0.0f),
      y(0.0f),
      z(0.0f) {}

    bool operator==(const Vector3& other) const {
      return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    template <typename T>
    bool operator!=(const Vector3& other) const {
      return this->x != other.x || this->y != other.y || this->z != other.z;
    }

    Vector3 operator+(const Vector3& other) const {
      Vector3 vec;
      vec.x = this->x + other.x;
      vec.y = this->y + other.y;
      vec.z = this->z + other.z;
      return vec;
    }

    Vector3 operator-(const Vector3& other) const {
      Vector3 vec;
      vec.x = this->x - other.x;
      vec.y = this->y - other.y;
      vec.z = this->z - other.z;
      return vec;
    }

    Vector3 operator*(const Vector3& other) const {
      Vector3 vec;
      vec.x = this->x * other.x;
      vec.y = this->y * other.y;
      vec.z = this->z * other.z;
      return vec;
    }

    Vector3 operator*(const float& other) const {
      Vector3 vec;
      vec.x = this->x * other;
      vec.y = this->y * other;
      vec.z = this->z * other;
      return vec;
    }

    float GetMagnitude() const {
      return std::sqrt(x * x + y * y + z * z);
    }

    float GetDistance(const Vector3& other) const {
      return (*this - other).GetMagnitude();
    }

    /// Keeps the size at 16 bytes without asking for 16 byte alignment - see the class comment. It is
    /// public because a private member would cost the type its standard layout, and the property
    /// registration needs that to reach x, y and z by member pointer (as_bind.hpp asserts it).
    char padding_[4]{};
  };

  static_assert(sizeof(Vector3) == 0x10);
  // The dword-packing the class comment describes is exactly why this is not 16: an aligned store into
  // a script-chosen destination is an access violation waiting for the wrong frame layout.
  static_assert(alignof(Vector3) == 4);

  class Vector4 {
  public:
    float x, y, z, w;

    constexpr Vector4(const float x, const float y, const float z, const float w) :
      x(x),
      y(y),
      z(z),
      w(w) {}

    constexpr Vector4() :
      x(),
      y(),
      z(),
      w() {}
  };

  union matrix34 {
    float data[4][4]; // yes, the underlying representation is 4x4
    struct {
      struct {
        float x, y, z, w;
      } rows[3];
    };
  };

  union matrix44 {
    float data[4][4];

    struct {
      struct {
        float x, y, z, w;
      } rows[4];
    };
  };
}
#pragma pack(pop)
