//
// Created by X-ray on 04/04/2025.
//

#include "inject.hpp"
#include <base-common/win32/misc.hpp>

namespace base::injector {
  namespace {
    Status InjectImpl(const DWORD pid, const std::filesystem::path& dll) {
      // Convert to proper string format for LoadLibraryA
      const std::string dll_path_ansi = dll.string(); // Use ANSI encoding instead of UTF-8
      const size_t dll_path_size = dll_path_ansi.size() + 1;

      const HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
      if (hProc == nullptr) {
        return MakeFailure<ResultCode::kINVALID_HANDLE>("Failed to open process: {}", win32::GetLastErrorStr());
      }

      // Allocate remote memory
      const LPVOID remote_mem = VirtualAllocEx(hProc, nullptr, dll_path_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
      if (remote_mem == nullptr) {
        CloseHandle(hProc);
        return MakeFailure<ResultCode::kALLOCATION_ERROR>("Failed to allocate memory: {}", win32::GetLastErrorStr());
      }

      // Write the dll path
      SIZE_T bytes_written = 0;
      if (!WriteProcessMemory(hProc, remote_mem, dll_path_ansi.c_str(), dll_path_size, &bytes_written) || bytes_written != dll_path_size) {
        VirtualFreeEx(hProc, remote_mem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to write DLL path: {}", win32::GetLastErrorStr());
      }

      // Create and wait for remote thread
      const HANDLE remote_thread = CreateRemoteThread(hProc, nullptr, 0,
                                                      reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), remote_mem, 0, nullptr);

      if (remote_thread == nullptr) {
        VirtualFreeEx(hProc, remote_mem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to create remote thread: {}", win32::GetLastErrorStr());
      }

      // Wait for thread completion and check result
      WaitForSingleObject(remote_thread, 30000);
      DWORD exit_code = 0;
      GetExitCodeThread(remote_thread, &exit_code);

      // Cleanup
      CloseHandle(remote_thread);
      VirtualFreeEx(hProc, remote_mem, 0, MEM_RELEASE);
      CloseHandle(hProc);

      // Check if LoadLibraryA succeeded (returned handle to the loaded module)
      if (exit_code == 0) {
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("LoadLibraryA failed to load the DLL");
      }

      return {};
    }
  }

  Status Inject(const DWORD pid, const std::filesystem::path& dll) {
    if (!std::filesystem::is_regular_file(dll)) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("DLL path is not a file");
    }

    return InjectImpl(pid, dll);
  }
}
