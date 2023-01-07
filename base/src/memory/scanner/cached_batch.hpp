//
// Created by X-ray on 12/05/22.
//

#pragma once
#ifndef GTA_BASE_CACHED_BATCH_HPP
#define GTA_BASE_CACHED_BATCH_HPP
#include <string>
#include <functional>
#include <utility>
#include "../../rage/joaat.hpp"
#include "range.hpp"
#include "sig_batch_scan.hpp"

namespace gta_base::memory::scanner {
class CachedBatch {
 public:
  struct ToUpdate {
    std::uintptr_t offset;
    std::string joaat;
  };
  using to_update_t = std::vector<ToUpdate>;

 public:
  void add(const std::string &name, char *pattern, std::function<void(Handle)> callback);

  void Run(Range region);
 private:

  struct CachedEntry {
    CachedEntry(Batch::entry entry, rage::joaat_t pattern_joaat) : entry(std::move(entry)), pattern_joaat(pattern_joaat) {}

    Batch::entry entry;
    rage::joaat_t pattern_joaat;
  };

  std::vector<CachedEntry> entries_;

 private:
  std::optional<to_update_t> LoadFromJson(Range &region, rapidjson::Value &json);
  to_update_t LoadNoCache(Range &region);
};
}
#endif //GTA_BASE_CACHED_BATCH_HPP
