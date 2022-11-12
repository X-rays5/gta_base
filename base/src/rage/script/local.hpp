//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_LOCAL_HPP
#define GTA_BASE_LOCAL_HPP
#include <script/scrThread.hpp>

namespace rage::script {
    class Local {
    public:
      explicit Local(rage::scrThread* thread, std::size_t idx);
      explicit Local(void* stack, std::size_t idx);

      Local At(std::ptrdiff_t idx);
      Local At(std::ptrdiff_t idx, std::size_t arr_size);

      template <typename T>
      std::enable_if_t<std::is_pointer_v<T>, T> As()
      {
        return static_cast<T>(Get());
      }

      template <typename T>
      std::enable_if_t<std::is_lvalue_reference_v<T>, T> As() {
        return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(Get());
      }

    private:
      std::size_t idx_;
      void* stack_;

    private:
      void* Get();
    };
  }
#endif //GTA_BASE_LOCAL_HPP
