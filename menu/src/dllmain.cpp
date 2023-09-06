//
// Created by X-ray on 06/05/2023.
//

#include <memory>
#include <Windows.h>
#include "main.hpp"
#include "util/vfs.hpp"
#include "logging/logger.hpp"

namespace {
  HINSTANCE dll_inst;
}

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD call_reason, LPVOID) {
  if (call_reason == DLL_PROCESS_ATTACH) {
    dll_inst = dll_handle;
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {

      base::util::vfs::SetWorkingDir();

      int exit_code = EXIT_FAILURE;
      try {
       auto logger_inst = std::make_unique<base::logging::Manager>();
        exit_code = base::menu_main();
        logger_inst.reset();
      } catch (std::exception& e) {
        MessageBoxA(nullptr, fmt::format("An exception occurred on the main thread: {}", e.what()).c_str(), "Critical exception main thread", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
      }

      FreeLibraryAndExitThread(dll_inst, exit_code);
    }, nullptr, 0, nullptr);
  }
}