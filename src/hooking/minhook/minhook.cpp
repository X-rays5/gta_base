//
// Created by X-ray on 3/6/2022.
//

#include "minhook.hpp"
#include <MinHook.h>

namespace gta_base {
  namespace hooking {
    MinHook::MinHook() {
      MH_Initialize();
    }

    MinHook::~MinHook() {
      MH_Uninitialize();
    }

    void MinHook::AddHook() {
      //MH_CreateHook();
    }
  }
}