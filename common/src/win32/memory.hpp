//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_MEMORY_6E2CBAF0C25F49E69A8329E5694C9C0B_HPP
#define GTA_BASE_MEMORY_6E2CBAF0C25F49E69A8329E5694C9C0B_HPP
#include <cstdint>
#include <string>
#include <TlHelp32.h>
#include "../util/result.hpp"

namespace base::win32::memory {
 /**
  * \brief Get the base address of a module in a process
  * \param pid pid of the process
  * \param mod_name name of the module
  * \return The base address of the module
  */
 StatusOr<std::uintptr_t> GetModuleBaseAddress(std::uint32_t pid, const std::string& mod_name);
 /**
  * \brief   Get the module entry of a module from an address
  * \param pid pid of the process
  * \param addr address inside the module
  * \return The module entry of the module
  */
 StatusOr<MODULEENTRY32> GetModuleFromAddress(std::uint32_t pid, std::uintptr_t addr);
 /**
  * \brief Get the name of a module from an address
  * \param pid pid of the process
  * \param addr address inside the module
  * \return The name of the module
  */
 StatusOr<std::string> GetModuleNameFromAddress(std::uint32_t pid, std::uintptr_t addr);
 /**
  * \brief Get the offset of an address from the base address of the module
  * \param pid pid of the process
  * \param addr address inside the module
  * \return The offset of the address from the base address of the module
  */
 StatusOr<uintptr_t> GetModuleOffsetFromAddress(std::uint32_t pid, std::uintptr_t addr);
 /**
  * \brief Get the module entry of a module from a HMODULE
  * \param mod handle to the module
  * \return The module entry of the module
  */
 StatusOr<MODULEENTRY32> GetModuleFromHModule(HMODULE mod);
}

#endif //GTA_BASE_MEMORY_6E2CBAF0C25F49E69A8329E5694C9C0B_HPP
