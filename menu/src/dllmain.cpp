//
// Created by X-ray on 06/05/2023.
//

#include <Windows.h>
#include "main.hpp"

namespace {
  HINSTANCE dll_inst;
}

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD call_reason, LPVOID) {
  if (call_reason == DLL_PROCESS_ATTACH) {
    dll_inst = dll_handle;
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
      int exit_code = base::menu_main();

      FreeLibraryAndExitThread(dll_inst, exit_code);
    }, nullptr, 0, nullptr);
  }
}