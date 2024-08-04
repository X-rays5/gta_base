//
// Created by X-ray on 13/04/2024.
//

#include "batch.hpp"

#include <ranges>

#include "../../util/thread_pool.hpp"
#include "../../util/profile.hpp"

namespace base::memory::signature {
  namespace {
    bool ScanPattern(BatchJob job) {
      StatusOr<Address> address = job.pattern->Scan();
      if (address.error()) {
        LOG_ERROR("Failed to find {}", job.name);
        return false;
      }

      LOG_DEBUG("Found '{}' GTA5.exe+0x{:x}", job.name, address.value().As<DWORD64>() - reinterpret_cast<std::uintptr_t>(GetModuleHandle(job.pattern->GetModule().c_str())));
      // Call callback to use found address
      job.cb(&address.value());

      return true;
    }
  }

  void BatchScan::Scan() {
    if (scanned_)
      return;
    scanned_ = true;

    util::Profile profile;
#ifdef NDEBUG
    profile.Disabled(true);
#endif

    std::vector<std::future<bool>> futures;
    for (auto&& job : std::views::values(patterns_)) {
      futures.emplace_back(util::kTHREAD_POOL->emplace_back([&] {
        profile.Begin(job.name);
        const auto res = ScanPattern(job);
        profile.End(job.name);
        return res;
      }));
    }

    for (auto&& fut : futures) {
      if (!fut.get()) {
        LOG_CRITICAL("Failed to find a pattern. Aborting...");
        exit(1);
      }
    }

    PROFILER_SAVE(profile);
  }
}
