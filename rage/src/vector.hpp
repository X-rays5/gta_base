//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <cmath>

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

  class alignas(16) Vector3 {
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
  };

  static_assert(sizeof(Vector3) == 0x10);

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
