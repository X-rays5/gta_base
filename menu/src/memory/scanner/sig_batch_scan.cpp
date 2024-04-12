#include "sig_batch_scan.hpp"

namespace base::memory::scanner {
  void Batch::add(std::string name, char* pattern, std::function<void(Handle)> callback) {
    add(std::move(name), Pattern(pattern), std::move(callback));
  }

  void Batch::add(std::string name, Pattern pattern, std::function<void(Handle)> callback) {
    entries_.emplace_back(std::move(name), std::move(pattern), std::move(callback));
  }

  Status Batch::run(Range region) {
    LOG_DEBUG("Starting batch scan in region: 0x{:x} - 0x{:x}", (std::uintptr_t)region.begin(), (std::uintptr_t)region.end());
    bool all_found = true;
    for (auto& entry : entries_) {
      if (auto result = region.scan(entry.pattern_); result.has_value()) {
        if (entry.cb_) {
          std::invoke(entry.cb_, result.value());
          LOG_DEBUG("Found '{}' GTA5.exe+0x{:x}", entry.name_, result.value().as<DWORD64>() - region.begin().as<DWORD64>());
        } else {
          all_found = false;
          LOG_CRITICAL("What's supposed to happen with this address... '{}'.", entry.name_);
        }
      } else {
        all_found = false;
        LOG_CRITICAL("Failed to find '{}'.", entry.name_);
      }
    }

    entries_.clear();
    if (!all_found) {
      LOG_CRITICAL("Failed to find some patterns.");
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to find some patterns.");
    }

    return {};
  }
}
