//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_MEMORY_6E2CBAF0C25F49E69A8329E5694C9C0B_HPP
#define GTA_BASE_MEMORY_6E2CBAF0C25F49E69A8329E5694C9C0B_HPP
#include <TlHelp32.h>

namespace base::win32::memory {
  absl::StatusOr<std::uintptr_t> GetModuleBaseAddress(std::uint32_t pid, const std::string& mod_name);
  absl::StatusOr<MODULEENTRY32> GetModuleFromAddress(std::uint32_t pid, std::uintptr_t addr);
  absl::StatusOr<std::string> GetModuleNameFromAddress(std::uint32_t pid, std::uintptr_t addr);
  absl::StatusOr<uintptr_t> GetModuleOffsetFromAddress(std::uint32_t pid, std::uintptr_t addr);
}

#endif //GTA_BASE_MEMORY_6E2CBAF0C25F49E69A8329E5694C9C0B_HPP
