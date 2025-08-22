//
// Created by xray on 06/09/2023.
//

#include "memory.hpp"

namespace base::win32::memory {
  StatusOr<std::uintptr_t> GetModuleBaseAddress(const std::uint32_t pid, const std::string& mod_name) {
    std::uint64_t mod_base_addr = 0;
    const HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap == INVALID_HANDLE_VALUE) {
      return MakeFailure<ResultCode::kINVALID_HANDLE>();
    }

    MODULEENTRY32 mod_entry;
    mod_entry.dwSize = sizeof(mod_entry);
    if (Module32First(h_snap, &mod_entry)) {
      do {
        if (!_stricmp(mod_entry.szModule, mod_name.c_str())) {
          mod_base_addr = reinterpret_cast<std::uint64_t>(mod_entry.modBaseAddr);
          break;
        }
      } while (Module32Next(h_snap, &mod_entry));
    }

    CloseHandle(h_snap);

    return mod_base_addr;
  }

  StatusOr<MODULEENTRY32> GetModuleFromAddress(const std::uint32_t pid, const std::uintptr_t addr) {
    MODULEENTRY32 mod_entry;
    const HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap == INVALID_HANDLE_VALUE) {
      return MakeFailure<ResultCode::kINVALID_HANDLE>();
    }

    mod_entry.dwSize = sizeof(mod_entry);
    if (Module32First(h_snap, &mod_entry)) {
      do {
        if (reinterpret_cast<std::uint64_t>(mod_entry.modBaseAddr) <= addr && addr <= reinterpret_cast<std::uint64_t>(mod_entry.modBaseAddr) + mod_entry.modBaseSize) {
          break;
        }
      } while (Module32Next(h_snap, &mod_entry));
    }

    CloseHandle(h_snap);

    return mod_entry;
  }

  StatusOr<std::string> GetModuleNameFromAddress(const std::uint32_t pid, const std::uintptr_t addr) {
    auto mod_addr = GetModuleFromAddress(pid, addr);
    if (!mod_addr)
      return mod_addr.error().Forward();

    return mod_addr->szModule;
  }

  StatusOr<uintptr_t> GetModuleOffsetFromAddress(const std::uint32_t pid, const std::uintptr_t addr) {
    auto mod_addr = GetModuleFromAddress(pid, addr);
    if (!mod_addr)
      return mod_addr.error();

    return addr - reinterpret_cast<std::uintptr_t>(mod_addr->modBaseAddr);
  }

  StatusOr<MODULEENTRY32> GetModuleFromHModule(const HMODULE mod) {
    MODULEENTRY32 mod_entry{};
    mod_entry.dwSize = sizeof(MODULEENTRY32);
    const HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hSnapshot == INVALID_HANDLE_VALUE) {
      return MakeFailure<ResultCode::kINVALID_HANDLE>();
    }

    if (Module32First(hSnapshot, &mod_entry)) {
      do {
        if (mod_entry.hModule == mod) {
          break;
        }
      } while (Module32Next(hSnapshot, &mod_entry));
    }
    CloseHandle(hSnapshot);
    return mod_entry;
  }
}
