//
// Created by X-ray on 02/11/23.
//

#pragma once
#ifndef GTA_BASE_ENUM_FLAGS_2B15F4D00B284250ACA236AB401CCE55_HPP
#define GTA_BASE_ENUM_FLAGS_2B15F4D00B284250ACA236AB401CCE55_HPP
#include <bitset>
#include <optional>
#include <magic_enum.hpp>

namespace gta_base::misc {
  template<typename T> requires std::is_enum_v<T>
  class Flags {
  public:
    explicit Flags(const T& val) {
      bitset_.set(GetIdx(val));
    }

    Flags& operator&=(const T& val) noexcept {
      bool tmp = bitset_.test(GetIdx(val));
      bitset_.reset();
      bitset_.set(GetIdx(val), tmp);

      return *this;
    }

    Flags& operator&=(const Flags& b) noexcept {
      bitset_ &= b.bitset_;

      return *this;
    }

    Flags& operator|=(const T& val) noexcept {
      bitset_.set(GetIdx(val));

      return *this;
    }

    Flags& operator|=(const Flags& b) noexcept {
      bitset_ |= b.bitset_;

      return *this;
    }

    Flags operator&(const T& val) const {
      Flags res(*this);
      res &= val;

      //GTA_BASE_ASSERT(res.bitset_.count() <= 1, "{}", res.bitset_.count());

      return res;
    }

    Flags operator&(const Flags& b) const {
      Flags res(*this);
      res.bitset_ &= b.bitset_;

      return res;
    }

    Flags operator|(const T& val) const {
      Flags res(*this);
      res |= val;

      //GTA_BASE_ASSERT(res.bitset_.count() >= 1, "{}", res.bitset_.count());

      return res;
    }

    Flags operator|(const Flags& b) const {
      Flags res(*this);
      res.bitset_ |= b.bitset_;

      return res;
    }

    Flags operator~() const {
      Flags copy(*this);
      copy.bitset_.flip();

      return copy;
    }

    operator bool() const {
      return bitset_.any();
    }

    bool operator==(const T& val) const {
      Flags tmp(val);

      return bitset_ == tmp.bitset_;
    }

    bool operator==(const Flags& b) const {
      return bitset_ == b.bitset_;
    }

    constexpr bool operator[](const T& val) const { return bitset_[GetIdx(val)]; }

  private:
    std::bitset<magic_enum::enum_count<T>()> bitset_;

  private:
    FORCE_INLINE constexpr std::size_t GetIdx(const T& val) {
      return magic_enum::enum_index(val);
    }
  };
}
#endif //GTA_BASE_ENUM_FLAGS_2B15F4D00B284250ACA236AB401CCE55_HPP
