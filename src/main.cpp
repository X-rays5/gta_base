//
// Created by X-ray on 3/6/2022.
//

#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "logger/logger.hpp"
#include "common/common.hpp"
#include "memory/sigscanner.hpp"
#include "hooking/hooking.hpp"


bool gta_base::common::globals::running = true;
void BaseMain() {
  using namespace gta_base;

  kLOGGER = std::make_unique<Logger>();
  kHOOKING = std::make_unique<Hooking>();

  using is_elevated_t = LPVOID(*)();
  is_elevated_t IsElevated{};
  IsElevated = memory::Signature("40 53 48 83 EC 30 33 DB 48 89 5C 24 ? 48 FF 15 ? ? ? ? 0F 1F 44 00 ? 48 8B C8 4C 8D 44 24 ? 8D 53 08 48 FF 15").Scan("IsElevated").As<is_elevated_t>();

  LOG_DEBUG("is elevated -> {}", IsElevated());
  LOG_TRACE("Hello {}!", "World");
  while (common::globals::running) {
    if (common::KeyState(VK_ESCAPE))
      break;
    Sleep(500);
  }


  kHOOKING.reset();

  LOG_INFO("successful unload");
  gta_base::kLOGGER.reset();
}
