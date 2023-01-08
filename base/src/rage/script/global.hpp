//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_GLOBAL_HPP
#define GTA_BASE_GLOBAL_HPP
#include <cstddef>
#include <type_traits>
#include "../global_idx.hpp"

namespace rage::script {
  class Global {
  public:
    template<typename T>
    struct Entry {
      explicit Entry(T* val) : val_(val) {}

      Entry& operator=(T val) {
        *val_ = std::move(val);
        return *this;
      }

      T operator*() {
        return *val_;
      }

      operator T() {
        return *val_;
      }

      operator T*() {
        return val_;
      }

    private:
      T* val_;
    };

  public:
    explicit Global(std::size_t idx);

    [[nodiscard]] Global At(std::ptrdiff_t idx) const;
    [[nodiscard]] Global At(std::ptrdiff_t idx, std::size_t arr_size) const;

    template<typename T>
    Entry<T> As() {
      return Entry(reinterpret_cast<T*>(Get()));
    }

  private:
    std::size_t idx_;

  private:
    [[nodiscard]] void* Get() const;
  };
}
#endif //GTA_BASE_GLOBAL_HPP
