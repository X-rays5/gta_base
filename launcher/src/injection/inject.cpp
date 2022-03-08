//
// Created by X-ray on 3/8/2022.
//

#include <filesystem>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <TlHelp32.h>

namespace launcher {
  namespace injection {
    namespace {
      DWORD GetProcByName(const char* name) {
        DWORD pid = NULL;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
          return NULL;

        PROCESSENTRY32 process_entry = {0};
        process_entry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snapshot, &process_entry)) {
          do {
            if (strcmp(process_entry.szExeFile, name) == 0) {
              pid = process_entry.th32ProcessID;
              break;
            }
          } while (Process32Next(snapshot, &process_entry));
        }
        CloseHandle(snapshot);
        return pid;
      }
    }

    bool Inject(const char* target_process_name, const std::filesystem::path& dll_path) {
      if(DWORD process_id = GetProcByName(target_process_name); process_id != NULL) {
        HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
        if(!process_handle && process_handle != INVALID_HANDLE_VALUE)
          return false;

        void* alloc = VirtualAllocEx(process_handle, nullptr, dll_path.string().length(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if(!alloc)
          return false;

        if(!WriteProcessMemory(process_handle, alloc, dll_path.string().c_str(), dll_path.string().length(), nullptr))
          return false;

        HANDLE thread_handle = CreateRemoteThread(process_handle, nullptr, 0, (LPTHREAD_START_ROUTINE) LoadLibraryA, alloc, NULL, nullptr);
        if(!thread_handle)
          return false;

        CloseHandle(thread_handle);
        CloseHandle(process_handle);

        return true;
      }
      return false;
    }
  }// namespace injection
}// namespace launcher
