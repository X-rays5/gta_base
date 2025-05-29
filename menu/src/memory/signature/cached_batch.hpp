//
// Created by X-ray on 07/04/2025.
//

#ifndef CACHED_BATCH_HPP_04230824
#define CACHED_BATCH_HPP_04230824
#include "batch.hpp"
#include <base-common/concurrency/spinlock.hpp>
#include <ankerl/unordered_dense.h>

namespace base::menu::memory::signature {
  struct CacheFile {
    std::string module_md5;
    std::unordered_map<std::string, std::uintptr_t> cached_patterns;
  };

  class CachedBatchScan final : public BatchScan {
  public:
    CachedBatchScan();
    virtual ~CachedBatchScan() override = default;

    virtual void Scan() override;

  private:
    common::concurrency::Spinlock cache_lock_;
    ankerl::unordered_dense::map<std::string, std::string> md5_db_;

  private:
    bool ProcessCachedPattern(const BatchJob& job);
    static StatusOr<common::memory::Address> ScanPattern(const BatchJob& job);

    StatusOr<common::memory::Address> GetCacheEntry(const std::string& module, const std::string& md5, std::string pattern);
    bool SetCacheEntry(const std::string& module, std::string pattern, const std::uintptr_t addr);
  };
}

#endif //CACHED_BATCH_HPP_04230824
