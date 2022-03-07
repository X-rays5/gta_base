//
// Created by X-ray on 3/6/2022.
//

#include <utility>
#include "minhook.hpp"

namespace gta_base {
  namespace hooking {
    MinHook::MinHook() {
      MH_Initialize();
    }

    MinHook::~MinHook() {
      for (auto &hook : hooks) {
        MH_RemoveHook(hook.second);
      }
      MH_Uninitialize();
    }

    MH_STATUS MinHook::AddHook(const std::string& name, LPVOID src, LPVOID dst, LPVOID* og) {
      MH_STATUS result = MH_CreateHook(src, dst, og);
      hooks[name] = src;

      return result;
    }

    MH_STATUS MinHook::RemoveHook(const std::string& name) {
      auto hook_src = hooks[name];
      hooks.erase(name);

      return MH_RemoveHook(hook_src);
    }
  }
}