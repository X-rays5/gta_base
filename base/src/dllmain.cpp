//
// Created by X-ray on 3/6/2022.
//

#include <stdexcept>
#include <thread>
#include "main.hpp"

HINSTANCE gta_base::globals::dll_handle = nullptr;

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD call_reason , LPVOID) {
  using namespace gta_base::common;
  using namespace gta_base;

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
    if (spdlog::default_logger_raw())
      spdlog::default_logger_raw()->flush();
  }

  return true;
}