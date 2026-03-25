//
// Created by X-ray on 09/04/2024.
//

#include "pattern.hpp"
#pragma warning(push)
#pragma warning(disable : 4456)
#pragma warning(disable : 4189)
#pragma warning(disable : 4244)
#pragma warning(disable : 4245)
#include <Pattern16.h>
#pragma warning(pop)
#include <psapi.h>

namespace base::menu::memory::signature {
  namespace {
    std::string FixIdaSignature(std::string signature) {
      size_t start_pos = 0;
      while ((start_pos = signature.find("? ", start_pos)) != std::string::npos) {
        signature.replace(start_pos, 2, "?? ");
        start_pos += 3;
      }

      return signature;
    }
  }

  StatusOr<common::memory::Address> Pattern::Scan() {
    HMODULE hmod = GetModuleHandle(module_name_.empty() ? nullptr : module_name_.c_str());
    if (!hmod) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to get hMod for: {}", module_name_);
    }

    MODULEINFO mod_info;
    if (!GetModuleInformation(GetCurrentProcess(), hmod, &mod_info, sizeof(mod_info))) {
      LOG_ERROR("Failed to get module information for sig scan");
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to get ModuleInfo for: {}", module_name_);
    }

    std::string tmp_sig = FixIdaSignature(signature_);
    LOG_DEBUG("Starting scan in '{}' from 0x{:X} to 0x{:X} with signature '{}'", module_name_.empty() ? common::globals::target_process_name : module_name_, reinterpret_cast<std::uintptr_t>(hmod), reinterpret_cast<std::uintptr_t>(mod_info.lpBaseOfDll) + mod_info.SizeOfImage, tmp_sig);
    const auto addr = common::memory::Address(Pattern16::scan(hmod, mod_info.SizeOfImage, tmp_sig));
    if (!static_cast<std::uintptr_t>(addr)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to find signature");
    }

    return addr;
  }
}
