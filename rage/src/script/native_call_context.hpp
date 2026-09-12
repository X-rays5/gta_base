//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <cstdint>
#include <type_traits>
#include <utility>

#include "vector.hpp"
#include "../vector.hpp"

namespace rage::script {
  class NativeCallContext {
  public:
    constexpr void reset() {
      arg_count_ = 0;
      num_vector_refs_ = 0;
    }

    template <typename T>
    constexpr void PushArg(T&& value) {
      static_assert(sizeof(T) <= sizeof(std::uint64_t));
      *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(static_cast<std::uint64_t*>(args_) + arg_count_++) = std::forward<T>(value);
    }

    template <typename T>
    constexpr T& GetArg(const std::size_t index) {
      static_assert(sizeof(T) <= sizeof(std::uint64_t));
      return *reinterpret_cast<T*>(static_cast<std::uint64_t*>(args_) + index);
    }

    template <typename T>
    constexpr void SetArg(const std::size_t index, T&& value) {
      static_assert(sizeof(T) <= sizeof(std::uint64_t));
      *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(static_cast<std::uint64_t*>(args_) + index) = std::forward<T>(value);
    }

    template <typename T>
    constexpr T* GetReturnValue() {
      return static_cast<T*>(return_val_);
    }

    template <typename T>
    constexpr void SetReturnValue(T&& value) {
      *static_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(return_val_) = std::forward<T>(value);
    }

    void FixVectors() {
      for (int i = 0; i < num_vector_refs_; i++) {
        *vector_ref_targets_[i] = vector_ref_sources_[i];
      }
      num_vector_refs_ = 0;
    }

    Vector3& GetSourceVector(const int index) {
      return vector_ref_sources_[index];
    }

  protected:
    void* return_val_; // 0x00
    std::uint32_t arg_count_; // 0x08
    void* args_; // 0x10
    std::int32_t num_vector_refs_; // 0x18
    Vector* vector_ref_targets_[4]; // 0x20
    // This is the one place a Vector3 has to be 16 byte aligned, and the alignment is asked for here
    // rather than on the type. The game writes these - they are the temporaries it is handed when a
    // native takes a vector by value - and it is the game's code, compiled against its own alignas(16)
    // Vector3, that does the storing. Vector3 itself is four byte aligned on purpose: see the class
    // comment in rage/src/vector.hpp for the AngelScript return path that alignment breaks. The array
    // still starts at 0x40 and every element is 16 bytes, so the layout is unchanged; asking for the
    // alignment keeps it that way if a member above ever changes size.
    alignas(16) Vector3 vector_ref_sources_[4]; // 0x40
  };

  static_assert(sizeof(NativeCallContext) == 0x80);
  static_assert(alignof(Vector3) <= alignof(NativeCallContext));

  using NativeHash = std::uint64_t;
  using NativePair = std::pair<NativeHash, NativeHash>;
  using NativeHandler = void (*)(NativeCallContext*);
}
