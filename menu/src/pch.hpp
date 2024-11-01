//
// Created by X-ray on 06/05/2023.
//

#pragma once
#ifndef BASE_PCH_02FC1A25A084431E8F0AB7249ED42555_HPP
#define BASE_PCH_02FC1A25A084431E8F0AB7249ED42555_HPP
#include <Windows.h>
#include <xorstr.hpp>
#include "util/result.hpp"
#include "globals.hpp"
#include "logging/logging_macro.hpp"
#include "render/font/IconsFontAwesome6.hpp"
#include "win32/all.hpp"

#ifdef MSVC
#define FORCE_INLINE __forceinline
#elif defined(GNU)
#define FORCE_INLINE __attribute__((always_inline)) inline
#elif
#define FORCE_INLINE
#endif
#endif //BASE_PCH_02FC1A25A084431E8F0AB7249ED42555_HPP
