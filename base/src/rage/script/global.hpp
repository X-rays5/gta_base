//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_GLOBAL_HPP
#define GTA_BASE_GLOBAL_HPP
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include "../global_idx.hpp"

namespace rage {
  namespace script {
    class Global {
    public:
      explicit Global(std::size_t idx);
      explicit Global(GlobalIdx idx);


      Global At(std::ptrdiff_t idx) const;
      Global At(GlobalIdx idx) const;
      Global At(std::ptrdiff_t idx, std::size_t arr_size) const;
      Global At(GlobalIdx idx, GlobalIdx arr_size) const;

      template <typename T>
      std::enable_if_t<std::is_pointer_v<T>, T> As() {
        return static_cast<T>(Get());
      }

      template <typename T>
      std::enable_if_t<std::is_lvalue_reference_v<T>, T> As() {
        return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(Get());
      }

    private:
      std::size_t idx_;

    private:
      void* Get() const;
    };
  }
}
#endif //GTA_BASE_GLOBAL_HPP
