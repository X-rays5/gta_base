//
// Created by xray on 06/09/2023.
//

#include "memory.hpp"

namespace base::win32::memory {
  absl::StatusOr<std::uintptr_t> GetModuleBaseAddress(std::uint32_t pid, const std::string& mod_name) {
    std::uint64_t mod_base_addr = 0;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap == INVALID_HANDLE_VALUE) {
      return absl::InvalidArgumentError("PID is an invalid handle");
    }

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

    CloseHandle(h_snap);

    return mod_base_addr;
  }

  absl::StatusOr<MODULEENTRY32> GetModuleFromAddress(std::uint32_t pid, std::uintptr_t addr) {
    MODULEENTRY32 mod_entry;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap == INVALID_HANDLE_VALUE) {
      return absl::InvalidArgumentError("PID is an invalid handle");
    }

    mod_entry.dwSize = sizeof(mod_entry);
    if (Module32First(h_snap, &mod_entry)) {
      do {
        if ((std::uint64_t) mod_entry.modBaseAddr <= addr && addr <= (std::uint64_t) mod_entry.modBaseAddr + mod_entry.modBaseSize) {
          break;
        }
      } while (Module32Next(h_snap, &mod_entry));
    }

    CloseHandle(h_snap);

    return mod_entry;
  }

  absl::StatusOr<std::string> GetModuleNameFromAddress(std::uint32_t pid, std::uintptr_t addr) {
    auto mod_addr = GetModuleFromAddress(pid, addr);
    if (!mod_addr.ok())
      return mod_addr.status();

    return mod_addr->szModule;
  }

  absl::StatusOr<uintptr_t> GetModuleOffsetFromAddress(std::uint32_t pid, std::uintptr_t addr) {
    auto mod_addr = GetModuleFromAddress(pid, addr);
    if (!mod_addr.ok())
      return mod_addr.status();

    return addr - reinterpret_cast<std::uintptr_t>(mod_addr->modBaseAddr);
  }
}