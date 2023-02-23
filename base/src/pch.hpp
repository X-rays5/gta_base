//
// Created by X-ray on 11/12/22.
//

#pragma once
#ifndef GTA_BASE_PCH_HPP
#define GTA_BASE_PCH_HPP
#include <Windows.h>
#include <fmt/format.h>
#include <cstdint>
#include "logger/logger.hpp"
#include "misc/common.hpp"
#include "misc/globals.hpp"
#include "settings/settings.hpp"

#ifdef MSVC
#define FORCE_INLINE __forceinline
#elif defined(GNU)
#define FORCE_INLINE __attribute__((always_inline)) inline
#else
#define FORCE_INLINE
#endif

template<size_t N>
struct template_str {
  constexpr template_str(const char(& str)[N]) {
    std::copy_n(str, N, value);
  }

  char value[N];
};

#endif //GTA_BASE_PCH_HPP
