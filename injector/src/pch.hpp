//
// Created by X-ray on 04/04/2025.
//

#ifndef PCH_HPP_04130054
#define PCH_HPP_04130054
#include <base-common/logging/logging_macro.hpp>

#ifdef MSVC
#define FORCE_INLINE __forceinline
#elif defined(GNU)
#define FORCE_INLINE __attribute__((always_inline)) inline
#else
#define FORCE_INLINE
#endif
#endif //PCH_HPP_04130054
