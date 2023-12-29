//
// Created by X-ray on 07/31/22.
//

#pragma once
#ifndef GTA_BASE_VMT_HPP
#define GTA_BASE_VMT_HPP
#include <unordered_map>
#include <memory>
#include "detour.hpp"

namespace base::hooking {
  class VmtHook {
  public:
    explicit VmtHook(LPVOID vtable);

    void Hook(const std::string& name, std::size_t idx, LPVOID dst);

    void Enable(std::size_t idx);
    void EnableAll();
    void Disable(std::size_t idx);
    void DisableAll();

    template <typename T, typename... Args> requires std::is_function_v<std::remove_pointer_t<T>>
    std::invoke_result_t<T, Args...> GetOriginal(std::size_t index, Args&&... args) {
      if (auto it = hooks_.find(index); it == hooks_.end()) {
        LOG_WARN("Tried to get original at idx: {} which doesn't exist", index);
      } else {
        return it->second->GetOriginal<T>(std::forward<Args>(args)...);
      }

      if constexpr (std::is_void_v<std::invoke_result_t<T, Args...>>)
        return;
      else
        return {};
    }

  private:
    LPVOID* vtable_;
    std::unordered_map<std::size_t, std::unique_ptr<DetourHook>> hooks_;
  };
}
#endif //GTA_BASE_VMT_HPP
