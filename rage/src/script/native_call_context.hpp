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
    Vector3 vector_ref_sources_[4]; // 0x40
  };

  static_assert(sizeof(NativeCallContext) == 0x80);

  using NativeHash = std::uint64_t;
  using NativePair = std::pair<NativeHash, NativeHash>;
  using NativeHandler = void (*)(NativeCallContext*);
}
