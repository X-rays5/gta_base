//
// Created by X-ray on 11/12/22.
//

#pragma once
#ifndef GTA_BASE_PCH_HPP
#define GTA_BASE_PCH_HPP
#include "natives/natives.hpp"
#include <Windows.h>
#include <fmt/format.h>
#include <cstdint>
#include "logger/logger.hpp"
#include "misc/common.hpp"
#include "misc/globals.hpp"
#include "settings/settings.hpp"

#ifdef _MSC_VER
#define FORCE_INLINE __forceinline
#elif defined(__clang__) || defined(__GNUC__)
#define FORCE_INLINE __attribute__((always_inline)) inline
#elif
#define FORCE_INLINE
#endif
#endif //GTA_BASE_PCH_HPP
