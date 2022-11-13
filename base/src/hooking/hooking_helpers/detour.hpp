//
// Created by X-ray on 08/01/22.
//

#pragma once
#ifndef GTA_BASE_DETOUR_HPP
#define GTA_BASE_DETOUR_HPP
#include <string>

namespace gta_base::hooking {
    class DetourHook {
    public:
      explicit DetourHook(std::string name, void *target, void *detour);

      ~DetourHook() noexcept;

      DetourHook(DetourHook&& that) = delete;
      DetourHook& operator=(DetourHook&& that) = delete;
      DetourHook(DetourHook const&) = delete;
      DetourHook& operator=(DetourHook const&) = delete;

      inline std::string GetName() const;

      void Enable();

      void Disable();

      template<typename T>
      T GetOriginal() {
        return static_cast<T>(original_);
      }
    private:
      std::string name_;
      void *target_;
      void *detour_;
      void *original_{};

    private:
      void FixHookAddress();

      static DWORD ExpHandler(PEXCEPTION_POINTERS exp, std::string const& name);
    };
  }
#endif //GTA_BASE_DETOUR_HPP
