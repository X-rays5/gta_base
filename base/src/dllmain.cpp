//
// Created by X-ray on 3/6/2022.
//

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "main.hpp"
#include "misc/common.hpp"

HANDLE gta_base::common::globals::main_thread = nullptr;
HINSTANCE gta_base::common::globals::dll_handle = nullptr;

#if !defined(NDEBUG)
#define CALL_MAIN(func) try {func();} catch(std::runtime_error &e){MessageBoxA(gta_base::common::GetHwnd(gta_base::common::globals::target_window_class, gta_base::common::globals::target_window_name), e.what(), "Error", MB_OK);}catch(...) {MessageBoxA(gta_base::common::GetHwnd(gta_base::common::globals::target_window_class, gta_base::common::globals::target_window_name), "Caught an exception", "Error", MB_OK);}
#else
#define CALL_MAIN(func) func();
#endif

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD load_reason ,LPVOID _) {
  using namespace gta_base::common;

  if (load_reason == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(dll_handle);

    globals::dll_handle = dll_handle;
    globals::main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD {
      if (!gta_base::common::IsTargetProcess())
        goto exit;

      CALL_MAIN(BaseMain)

      exit:
      CloseHandle(globals::main_thread);
      FreeLibraryAndExitThread(globals::dll_handle, 0);
    }, nullptr, 0, nullptr);
  }

  return true;
}