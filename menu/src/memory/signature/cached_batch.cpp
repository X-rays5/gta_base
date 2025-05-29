//
// Created by X-ray on 07/04/2025.
//

#include "cached_batch.hpp"
#include <future>
#include <ranges>
#include <glaze/glaze.hpp>
#include <base-common/fs/vfs.hpp>
#include "../../util/thread_pool.hpp"
#include "../../util/common.hpp"
#include <ctninja/joaat.h>

namespace base::menu::memory::signature {
  namespace {
    std::filesystem::path GetPathToCache(const std::string& module) {
      const std::filesystem::path module_path = module;
      return common::fs::vfs::GetPatternCacheDir() / (module_path.filename().string() + ".patterns");
    }
  }

  CachedBatchScan::CachedBatchScan() : BatchScan() {}

  void CachedBatchScan::Scan() {
    if (scanned_)
      return;
    scanned_ = true;

    std::vector<std::future<bool>> futures;
    for (auto&& job : std::views::values(patterns_)) {
      futures.emplace_back(util::kTHREAD_POOL->emplace_back([&] {
        return ProcessCachedPattern(job);
      }));
    }

    for (auto&& fut : futures) {
      if (!fut.get()) {
        LOG_CRITICAL("Failed to find a pattern. Aborting...");
        abort();
      }
    }
  }

  bool CachedBatchScan::ProcessCachedPattern(const BatchJob& job) {
    auto mod_base = win32::memory::GetModuleBaseAddress(GetCurrentProcessId(), job.pattern->GetModule());
    if (!mod_base) {
      LOG_ERROR("Failed to get module base address: {}", mod_base);
      return false;
    }

    auto module = win32::memory::GetModuleFromAddress(GetCurrentProcessId(), mod_base.value() + 1);
    if (!module) {
      LOG_ERROR("Failed to get module from address: {}", mod_base.value());
      return false;
    }

    auto md5 = md5_db_.find(module.value().szModule);
    if (md5 == md5_db_.end()) {
      auto hash_res = util::common::GetFileMd5Hash(module.value().szExePath);
      if (!hash_res) {
        LOG_ERROR("Failed to get file hash: {}", hash_res.error());
        return false;
      }

      md5_db_[module.value().szModule] = hash_res.value();
    }

    auto cache_res = GetCacheEntry(module.value().szModule, md5_db_[module.value().szModule], job.pattern->GetSignature());
    if (!cache_res && cache_res.error().GetResult() != ResultCode::kNOT_FOUND) {
      LOG_ERROR("Failed to get cache entry: {}", cache_res.error());
      return false;
    } else if (!cache_res && cache_res.error().GetResult() == ResultCode::kNOT_FOUND) {
      LOG_DEBUG("Cache entry not found, scanning pattern...");
    }

    if (cache_res) {
      job.cb(&cache_res.value());
    }

    auto scan_res = ScanPattern(job);
    if (!scan_res) {
      LOG_CRITICAL("Failed to scan pattern: {}", scan_res.error());
      return false;
    }

    if (!SetCacheEntry(module.value().szModule, job.pattern->GetSignature(), scan_res.value().As<std::uintptr_t>())) {
      LOG_WARN("Failed to set cache entry");
    }

    return true;
  }

  StatusOr<common::memory::Address> CachedBatchScan::ScanPattern(const BatchJob& job) {
    StatusOr<common::memory::Address> address = job.pattern->Scan();
    if (address.error()) {
      LOG_ERROR("Failed to find {}", job.name);
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to find {}", job.name);
    }

    LOG_DEBUG("Found '{}' GTA5.exe+0x{:x}", job.name, address.value().As<DWORD64>() - reinterpret_cast<std::uintptr_t>(GetModuleHandle(job.pattern->GetModule().c_str())));
    // Call callback to use found address
    job.cb(&address.value());

    return address.value();
  }

  StatusOr<common::memory::Address> CachedBatchScan::GetCacheEntry(const std::string& module, const std::string& md5, std::string pattern) {
    common::concurrency::ScopedSpinlock lock(cache_lock_);

    pattern = std::to_string(ctninja::joaat(pattern.c_str()));
    LOG_DEBUG("Getting cache entry for module: {}, pattern hash: {}", module, pattern);

    std::string buff;
    CacheFile cache_file;

    auto cache_file_path = GetPathToCache(module);
    LOG_DEBUG("Cache file path: {}", cache_file_path.string());

    if (!std::filesystem::is_regular_file(cache_file_path)) {
      LOG_DEBUG("Cache file not found: {}", cache_file_path.string());
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to find {}", cache_file_path);
    }

    if (const auto ec = glz::read_file_beve(cache_file, cache_file_path.string(), buff)) {
      LOG_ERROR("Failed to read cache file: {}, error: {}", cache_file_path.string(), glz::format_error(ec, std::string{}));
      return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, std::string{}));
    }

    if (cache_file.module_md5 != md5) {
      LOG_WARN("Cache file MD5 mismatch. Expected: {}, found: {}", md5, cache_file.module_md5);
      std::filesystem::remove(cache_file_path);
      return MakeFailure<ResultCode::kNOT_FOUND>("MD5 mismatch", cache_file_path);
    }

    const auto itr = cache_file.cached_patterns.find(pattern);
    if (itr == cache_file.cached_patterns.end()) {
      LOG_DEBUG("Cache pattern not found: {}", pattern);
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to find in cache", pattern);
    }

    LOG_DEBUG("Cache entry found for module: {}, pattern hash: {}, address: 0x{:x}", module, pattern, itr->second);
    return common::memory::Address(itr->second);
  }

  bool CachedBatchScan::SetCacheEntry(const std::string& module, std::string pattern, const std::uintptr_t addr) {
    common::concurrency::ScopedSpinlock lock(cache_lock_);

    pattern = std::to_string(ctninja::joaat(pattern.c_str()));
    LOG_DEBUG("Setting cache entry for module: {}, pattern hash: {}, address: 0x{:x}", module, pattern, addr);

    std::string buff;
    CacheFile cache_file;

    const auto cache_file_path = GetPathToCache(module);
    LOG_DEBUG("Cache file path: {}", cache_file_path.string());

    if (std::filesystem::is_regular_file(cache_file_path)) {
      if (auto ec = glz::read_file_beve(cache_file, cache_file_path.string(), buff)) {
        LOG_ERROR("Failed to read cache file: {}, error: {}", cache_file_path.string(), ec);
        return false;
      }
      LOG_DEBUG("Cache file read successfully from: {}", cache_file_path.string());
    } else {
      LOG_INFO("Creating cache file: {}", cache_file_path.string());
    }

    if (cache_file.module_md5.empty()) {
      auto md5_it = md5_db_.find(module);
      if (md5_it != md5_db_.end()) {
        cache_file.module_md5 = md5_it->second;
        LOG_DEBUG("Using MD5 from md5_db_: {}", cache_file.module_md5);
      } else {
        LOG_ERROR("MD5 not found in md5_db_ for module: {}", module);
        return false;
      }
    }

    cache_file.cached_patterns[pattern] = addr;
    if (auto ec = glz::write_file_beve(cache_file, cache_file_path.string(), buff)) {
      LOG_ERROR("Failed to write to file: {}", ec);
      return false;
    }

    LOG_DEBUG("Cache entry set successfully for module: {}, pattern hash: {}, address: 0x{:x}", module, pattern, addr);
    return true;
  }
}
