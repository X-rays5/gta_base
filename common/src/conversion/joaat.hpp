//
// Created by X-ray on 20/12/2025.
//

#pragma once

namespace base::common::conversion {
  using joaat_t = std::uint32_t;

  constexpr char to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
  }

  constexpr joaat_t joaat(const char* str, const std::size_t length = 0xFFFFFFFF) {
    joaat_t hash = 0;
    std::size_t i = 0;

    while (str[i] != '\0' && i < length) {
      hash += static_cast<joaat_t>(to_lower(str[i]));
      hash += (hash << 10);
      hash ^= (hash >> 6);
      ++i;
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
  }
}

consteval base::common::conversion::joaat_t operator""_J(const char* str, std::size_t length) {
  return base::common::conversion::joaat(str, length);
}