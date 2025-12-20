//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <rage/script/thread.hpp>
#include <base-common/conversion/joaat.hpp>

namespace base::menu::game {
  rage::script::Thread* FindScriptThread(common::conversion::joaat_t hash);
  void RunAsScript(rage::script::Thread* thread, const std::function<void()>& callback);
}