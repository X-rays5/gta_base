//
// Created by X-ray on 13/09/2026.
//

#ifndef ASJIT_PCH_HPP_7C1E4B92
#define ASJIT_PCH_HPP_7C1E4B92

// Self-contained on purpose. This component is meant to be liftable into another repository whole,
// so nothing here may reach into the monorepo - not even for a logging header. AngelScript and
// AsmJit arrive through vendor.cmake and bring their own includes with them.
//
// WIN32_LEAN_AND_MEAN and NOMINMAX are set as compile definitions on the target rather than here;
// defining them again in the precompiled header is a redefinition the compiler warns about, and this
// target treats warnings as errors.

#include <cstddef>
#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

#endif //ASJIT_PCH_HPP_7C1E4B92
