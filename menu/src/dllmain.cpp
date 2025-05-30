//
// Created by X-ray on 06/05/2023.
//

#include <memory>
#include <Windows.h>
#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logger.hpp>
#include "main.hpp"

namespace {
  HINSTANCE dll_inst;
}

BOOL WINAPI DllMain(const HINSTANCE dll_handle, const DWORD call_reason, LPVOID) {
  DisableThreadLibraryCalls(dll_handle);

  if (call_reason == DLL_PROCESS_ATTACH) {
    dll_inst = dll_handle;
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
      SetThreadDescription(GetCurrentThread(), L"main");
      base::common::fs::vfs::SetWorkingDir(BASE_SUBCOMPONENT);

      int exit_code = EXIT_FAILURE;
      try {
        auto logger_inst = std::make_unique<base::common::logging::Manager>();

        if (!base::win32::GetGameHwnd().error() && base::win32::IsTargetProcess()) {
          exit_code = base::menu::menu_main();
        } else {
          LOG_CRITICAL("Process doesn't seem to be GTA V, aborting...");
        }

        LOG_INFO("This is it... Goodbye!");
        logger_inst.reset();
      } catch (std::exception& e) {
        MessageBoxA(nullptr, fmt::format("An exception occurred on the main thread: {}", e.what()).c_str(), "Critical exception main thread", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
      }

      FreeLibraryAndExitThread(dll_inst, exit_code);
    }, nullptr, 0, nullptr);
  }

  return TRUE;
}
