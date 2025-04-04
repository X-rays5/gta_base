//
// Created by X-ray on 08/01/22.
//

#pragma once
#ifndef GTA_BASE_DETOUR_HPP
#define GTA_BASE_DETOUR_HPP
#include <string>
#include <polyhook2/Detour/NatDetour.hpp>

namespace base::menu::hooking {
  class DetourHook {
  public:
    explicit DetourHook(std::string name, void* src, void* dst);
    explicit DetourHook(std::string name, const std::string& module, const std::string& src, void* dst);

    ~DetourHook() noexcept;
    DetourHook(DetourHook&& that) = delete;
    DetourHook& operator=(DetourHook&& that) = delete;
    DetourHook(const DetourHook&) = delete;
    DetourHook& operator=(const DetourHook&) = delete;

    [[nodiscard]] std::string GetName() const;

    void Enable();
    void Disable();

    template <typename T, typename... Args> requires std::is_function_v<std::remove_pointer_t<T>>
    std::invoke_result_t<T, Args...> CallOriginal(Args&&... args) {
      void* og = reinterpret_cast<void*>(og_);
      if (!og) {
        LOG_WARN("nullptr og {}", name_);

        if constexpr (std::is_void_v<std::invoke_result_t<T, Args...>>)
          return;
        else
          return {};
      }

      return reinterpret_cast<T>(og)(std::forward<Args>(args)...);
    }

  private:
    std::string name_;
    std::uintptr_t og_;
    std::unique_ptr<PLH::NatDetour> detour_;
  };
}
#endif //GTA_BASE_DETOUR_HPP
