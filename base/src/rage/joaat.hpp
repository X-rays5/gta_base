//
// Created by X-ray on 07/28/22.
//

#pragma once
#ifndef GTA_BASE_JOAAT_HPP
#define GTA_BASE_JOAAT_HPP
#include <cstddef>
#include <string_view>
#include <type_traits>
#include <rage/joaat.hpp>

namespace rage {
  using joaat_t = std::uint32_t;

  template<std::size_t CharCount>
  struct constexpr_joaat {
    char data[CharCount];

    template<std::size_t... Indices>
    constexpr constexpr_joaat(const char* str, std::index_sequence<Indices...>) :
      data{(str[Indices])...} {
    }

    constexpr joaat_t operator()() {
      joaat_t hash = 0;

      for (std::size_t i = 0; i < CharCount; ++i) {
        hash += joaat_to_lower(data[i]);
        hash += (hash << 10);
        hash ^= (hash >> 6);
      }

      hash += (hash << 3);
      hash ^= (hash >> 11);
      hash += (hash << 15);

      return hash;
    }
  };

  inline joaat_t joaat(const char* str) {
    joaat_t hash = 0;

    while (*str) {
      hash += joaat_to_lower(*(str++));
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
  }
}

#define RAGE_JOAAT_IMPL(str) (::rage::constexpr_joaat<sizeof(str) - 1>((str), std::make_index_sequence<sizeof(str) - 1>())())
#define RAGE_JOAAT(str) (std::integral_constant<::rage::joaat_t, RAGE_JOAAT_IMPL(str)>::value)

#endif //GTA_BASE_JOAAT_HPP
