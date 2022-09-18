//
// Created by X-ray on 08/08/22.
//

#pragma once
#ifndef GTA_BASE_SCR_VECTOR_HPP
#define GTA_BASE_SCR_VECTOR_HPP
#include <rage/vector.hpp>

namespace rage
{

#pragma pack(push, 1)
  class scrVector {
  public:
    scrVector() = default;

    [[maybe_unused]] scrVector(float x, float y, float z) :
      x(x), y(y), z(z)
    {}

    scrVector(fvector3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

    scrVector operator+(const scrVector& other) const
    {
      scrVector vec;
      vec.x = this->x + other.x;
      vec.y = this->y + other.y;
      vec.z = this->z + other.z;
      return vec;
    }

    scrVector operator-(const scrVector& other) const
    {
      scrVector vec;
      vec.x = this->x - other.x;
      vec.y = this->y - other.y;
      vec.z = this->z - other.z;
      return vec;
    }

    scrVector operator*(const scrVector& other) const
    {
      scrVector vec;
      vec.x = this->x * other.x;
      vec.y = this->y * other.y;
      vec.z = this->z * other.z;
      return vec;
    }

    scrVector operator*(const float& other) const
    {
      scrVector vec;
      vec.x = this->x * other;
      vec.y = this->y * other;
      vec.z = this->z * other;
      return vec;
    }

    scrVector& operator=(const float& new_val) {
      x = new_val;
      y = new_val;
      z = new_val;

      return *this;
    }

    explicit operator bool() const {
      return x != 0.0f || y != 0.0f || z != 0.0f;
    }

    explicit operator fvector3() const {
      return {x,y,z};
    }

  public:
    float x{};
  private:
    char m_padding1[0x04]{};
  public:
    float y{};
  private:
    char m_padding2[0x04]{};
  public:
    float z{};
  private:
    char m_padding3[0x04]{};
  };
#pragma pack(pop)
}

#endif //GTA_BASE_SCR_VECTOR_HPP
