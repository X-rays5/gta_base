//
// Created by X-ray on 3/6/2022.
//

#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "logger/logger.hpp"
#include "common/common.hpp"
#include "hooking/hooking.hpp"
#include "memory/pointers.hpp"


bool gta_base::common::globals::running = true;
void BaseMain() {
  using namespace gta_base;

  kLOGGER = std::make_unique<Logger>();
  memory::kPOINTERS = std::make_unique<memory::Pointers>();

  kHOOKING = std::make_unique<Hooking>();

  LOG_TRACE("Hello {}!", "World");
  while (common::globals::running) {
    if (common::KeyState(VK_ESCAPE))
      break;
    Sleep(500);
  }


  kHOOKING.reset();

  memory::kPOINTERS.reset();
  LOG_INFO("successful unload");
  gta_base::kLOGGER.reset();
}
