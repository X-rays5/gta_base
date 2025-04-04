//
// Created by X-ray on 03/04/2025.
//

#ifndef PCH_HPP_04225410
#define PCH_HPP_04225410

#define WIN32_LEAN_AND_MEAN
#include "logging/logging_macro.hpp"

#ifdef MSVC
#define FORCE_INLINE __forceinline
#elif defined(GNU)
#define FORCE_INLINE __attribute__((always_inline)) inline
#else
#define FORCE_INLINE
#endif
#endif //PCH_HPP_04225410
