//
// Created by xray on 06/09/2023.
//

#include "memory.hpp"

namespace base::win32::memory {
  std::uintptr_t GetModuleBaseAddress(std::uint32_t pid, const std::string& mod_name) {
    std::uint64_t mod_base_addr = 0;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

    if (h_snap != INVALID_HANDLE_VALUE) {
      MODULEENTRY32 mod_entry;
      mod_entry.dwSize = sizeof(mod_entry);
      if (Module32First(h_snap, &mod_entry)) {
        do {
          if (!_stricmp(mod_entry.szModule, mod_name.c_str())) {
            mod_base_addr = (std::uint64_t) mod_entry.modBaseAddr;
            break;
          }
        } while (Module32Next(h_snap, &mod_entry));
      }
    }
    CloseHandle(h_snap);

    return mod_base_addr;
  }

  MODULEENTRY32 GetModuleFromAddress(std::uint32_t pid, std::uintptr_t addr) {
    MODULEENTRY32 mod_entry;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap != INVALID_HANDLE_VALUE) {
      mod_entry.dwSize = sizeof(mod_entry);
      if (Module32First(h_snap, &mod_entry)) {
        do {
          if ((std::uint64_t) mod_entry.modBaseAddr <= addr && addr <= (std::uint64_t) mod_entry.modBaseAddr + mod_entry.modBaseSize) {
            break;
          }
        } while (Module32Next(h_snap, &mod_entry));
      }
    }
    CloseHandle(h_snap);
    return mod_entry;
  }

  std::string GetModuleNameFromAddress(std::uint32_t pid, std::uintptr_t addr) {
    return GetModuleFromAddress(pid, addr).szModule;
  }

  uintptr_t GetModuleOffsetFromAddress(std::uint32_t pid, std::uintptr_t addr) {
    return addr - reinterpret_cast<std::uintptr_t>(GetModuleFromAddress(pid, addr).modBaseAddr);
  }
}