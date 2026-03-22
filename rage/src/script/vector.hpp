//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include "../vector.hpp"

namespace rage::script {
  class Vector {
  public:
    Vector() = default;

    Vector(const Vector3 vec) :
      x(vec.x),
      y(vec.y),
      z(vec.z) {}

    Vector(const float x, const float y, const float z) :
      x(x),
      y(y),
      z(z) {}

    Vector operator+(const Vector& other) const {
      Vector vec;
      vec.x = this->x + other.x;
      vec.y = this->y + other.y;
      vec.z = this->z + other.z;
      return vec;
    }

    Vector operator-(const Vector& other) const {
      Vector vec;
      vec.x = this->x - other.x;
      vec.y = this->y - other.y;
      vec.z = this->z - other.z;
      return vec;
    }

    Vector operator*(const Vector& other) const {
      Vector vec;
      vec.x = this->x * other.x;
      vec.y = this->y * other.y;
      vec.z = this->z * other.z;
      return vec;
    }

    Vector operator*(const float& other) const {
      Vector vec;
      vec.x = this->x * other;
      vec.y = this->y * other;
      vec.z = this->z * other;
      return vec;
    }

    bool operator==(const Vector& other) {
      return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    bool operator!=(const Vector& other) {
      return this->x != other.x || this->y != other.y || this->z != other.z;
    }

    operator rage::Vector3() {
      return {x, y, z};
    }

    operator bool() {
      return this->x != 0.0f || this->y != 0.0f || this->z != 0.0f;
    }

    alignas(8) float x{};
    alignas(8) float y{};
    alignas(8) float z{};
  };
}
