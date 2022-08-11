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
HANDLE gta_base::common::globals::main_thread = nullptr;

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD call_reason , LPVOID _) {
  using namespace gta_base::common;

  if (call_reason == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(dll_handle);

    globals::dll_handle = dll_handle;
    globals::main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD {
      while (!IsTargetProcess())
        std::this_thread::sleep_for(std::chrono::seconds(1));

      auto logger_inst = std::make_unique<gta_base::Logger>();
      LOG_INFO << "Logging initialized";
      try {
        BaseMain();
      } catch (std::exception& e) {
        LOG_FATAL << e.what();
      }
      LOG_DEBUG << globals::dll_handle << " " << &globals::main_thread;
      LOG_INFO << "Shutting logging down...";
      logger_inst.reset();

      CloseHandle(globals::main_thread);
      FreeLibraryAndExitThread(globals::dll_handle, 0);
    }, nullptr, 0, nullptr);
  }

  return true;
}