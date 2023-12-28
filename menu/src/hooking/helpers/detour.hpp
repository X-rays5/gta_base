//
// Created by X-ray on 08/01/22.
//

#pragma once
#ifndef GTA_BASE_DETOUR_HPP
#define GTA_BASE_DETOUR_HPP
#include <string>
#include <subhook.h>

namespace base::hooking {
  class DetourHook {
  public:
    explicit DetourHook(std::string name, void* src, void* dst);
    explicit DetourHook(std::string name, std::string module, std::string src, void* dst);

    ~DetourHook() noexcept;
    DetourHook(DetourHook&& that) = delete;
    DetourHook& operator=(DetourHook&& that) = delete;
    DetourHook(const DetourHook&) = delete;
    DetourHook& operator=(const DetourHook&) = delete;

    [[nodiscard]] std::string GetName() const;

    void Enable();
    void Disable();

    template <typename T>
    T GetOriginal() {
      return reinterpret_cast<T>(subhook_get_trampoline(detour_));
    }

  private:
    std::string name_;
    subhook_t detour_;
  };
}
#endif //GTA_BASE_DETOUR_HPP
