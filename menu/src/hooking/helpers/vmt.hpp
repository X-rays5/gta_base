//
// Created by X-ray on 07/31/22.
//

#pragma once
#ifndef GTA_BASE_VMT_HPP
#define GTA_BASE_VMT_HPP
#include <memory>
#include <polyhook2/Virtuals/VFuncSwapHook.hpp>

namespace base::menu::hooking {
  struct IDXHook {
    std::uint16_t idx;
    void* dst;
  };

  class VmtHook {
  public:
    explicit VmtHook(const std::string& vtable_name, LPVOID vtable, std::initializer_list<IDXHook> hooks);
    ~VmtHook();

    void EnableAll();
    void DisableAll();

    template <typename T, typename... Args> requires std::is_function_v<std::remove_pointer_t<T>>
    std::invoke_result_t<T, Args...> CallOriginal(std::uint16_t index, Args&&... args) {
      if (auto it = vtable_og_.find(index); it == vtable_og_.end()) {
        LOG_WARN("Tried to get original at idx: {} which doesn't exist", index);
      } else {
        if (it->second == NULL) {
          LOG_WARN("nullptr og {}", vtable_name_);
        } else {
          return reinterpret_cast<T>(it->second)(std::forward<Args>(args)...);
        }
      }

      if constexpr (std::is_void_v<std::invoke_result_t<T, Args...>>)
        return;
      else
        return {};
    }

  private:
    std::string vtable_name_;
    LPVOID vtable_;
    PLH::VFuncMap vtable_redirect_;
    PLH::VFuncMap vtable_og_;
    std::unique_ptr<PLH::VFuncSwapHook> vtable_hook_;
  };
}
#endif //GTA_BASE_VMT_HPP
