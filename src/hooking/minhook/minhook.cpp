//
// Created by X-ray on 3/6/2022.
//

#include <utility>
#include "../../logger/logger.hpp"
#include "minhook.hpp"

namespace gta_base {
  namespace hooking {
    MinHook::MinHook() {
      MH_Initialize();
    }

    MinHook::~MinHook() {
      MH_DisableHook(MH_ALL_HOOKS);
      for (auto &hook : hooks) {
        MH_RemoveHook(hook.second);
      }
      MH_Uninitialize();
    }

    MH_STATUS MinHook::AddHook(const std::string& name, LPVOID src, LPVOID dst, LPVOID* og) {
      MH_CreateHook(src, dst, og);
      MH_STATUS result = MH_EnableHook(src);
      hooks[name] = src;

      if (result == MH_OK) {
#ifdef NDEBUG
        LOG_INFO("[{}] hooked", name);
#else
        LOG_DEBUG("[{}] hooked src -> {}, dst -> {}, og -> {}", name, (void*)src, (void*)dst, (void*)og);
#endif
      }

      return result;
    }

    MH_STATUS MinHook::RemoveHook(const std::string& name) {
      auto hook_src = hooks[name];
      hooks.erase(name);

      MH_DisableHook(hook_src);
      return MH_RemoveHook(hook_src);
    }
  }
}