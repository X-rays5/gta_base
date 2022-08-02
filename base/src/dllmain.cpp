//
// Created by X-ray on 3/6/2022.
//

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "main.hpp"
#include "misc/common.hpp"

HANDLE main_thread_ = nullptr;
HINSTANCE dll_handle_ = nullptr;

#if !defined(NDEBUG)
#define CALL_MAIN(func) try {func();} catch(std::runtime_error &e){MessageBoxA(nullptr, e.what(), "Error", MB_OK);}catch(...) {MessageBoxA(nullptr, "Caught an exception", "Error", MB_OK);}
#else
#define CALL_MAIN(func) func();
#endif

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD load_reason ,LPVOID _) {
  if (load_reason == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(dll_handle);

    dll_handle_ = dll_handle;
    main_thread_ = CreateThread(nullptr, 0, [](PVOID) -> DWORD {
      if (!gta_base::common::IsTargetProcess())
        goto exit;

      CALL_MAIN(BaseMain)

      exit:
      CloseHandle(main_thread_);
      FreeLibraryAndExitThread(dll_handle_, 0);
    }, nullptr, 0, nullptr);

    return TRUE;
  } else if (load_reason == DLL_PROCESS_DETACH) {
    gta_base::common::globals::running = false;
    if (main_thread_)
      WaitForSingleObject(main_thread_, INFINITE);

    return TRUE;
  }
}