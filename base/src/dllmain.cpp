//
// Created by X-ray on 3/6/2022.
//

#include <stdexcept>
#include <thread>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "main.hpp"
#include "misc/common.hpp"
#include "logger/logger.hpp"

HINSTANCE gta_base::common::globals::dll_handle = nullptr;

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD call_reason , LPVOID) {
  using namespace gta_base::common;

  if (call_reason == DLL_PROCESS_ATTACH) {
    globals::dll_handle = dll_handle;
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
      if (!IsTargetProcess()) {
        FreeLibraryAndExitThread(globals::dll_handle, 0);
      }

      auto logger_inst = std::make_unique<gta_base::Logger>();
      LOG_INFO("Logging initialized");
      try {
        BaseMain();
      } catch (std::exception& e) {
        LOG_FATAL(e.what());
      }

      LOG_INFO("Shutting logging down...");
      logger_inst.reset();

      FreeLibraryAndExitThread(globals::dll_handle, 0);
    }, nullptr, 0, nullptr);
  } else if (call_reason == DLL_PROCESS_DETACH) {
    globals::running = false;
    gta_base::kLOGGER->~Logger(); // avoid stacktrace on normal exit
  }

  return true;
}