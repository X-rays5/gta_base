//
// Created by X-ray on 3/6/2022.
//

#include "hooking.hpp"
#include "wndproc.hpp"

namespace gta_base {
  Hooking::Hooking() {
    mh_hook_ = hooking::MinHook();

    hooking::HookWndProc();
  }

  Hooking::~Hooking() {
    hooking::UnhookWndProc();
  }
}