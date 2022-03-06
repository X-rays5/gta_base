//
// Created by X-ray on 3/6/2022.
//

#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "logger/logger.hpp"
#include "common/common.hpp"

bool gta_base::common::globals::running = true;
void BaseMain() {
  using namespace gta_base;

  gta_base::kLOGGER = std::make_unique<gta_base::Logger>();

  LOG_TRACE("Hello World!");
  while (common::globals::running) {
    if (common::KeyState(VK_DELETE))
      break;
    Sleep(500);
  }

  gta_base::kLOGGER.reset();
}
