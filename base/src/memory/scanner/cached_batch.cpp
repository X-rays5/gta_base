//
// Created by X-ray on 12/05/22.
//

#include "cached_batch.hpp"

#define UPDATE_NO_CACHE(mem_reg) auto cache_res = LoadNoCache(region); \
  if (!cache_res.empty()) { \
    rapidjson::Document write_json; \
    write_json.SetObject(); \
    WriteCacheFile(write_json, mod.szExePath, common::GetFileMd5Hash(mod.szExePath), cache_res); \
  }

namespace gta_base::memory::scanner {
  namespace {
    inline std::filesystem::path GetFilePath(const std::filesystem::path& mod_path) {
      return fmt::format("{}/{}.patterns", common::GetCachedPatternsDir().string(), common::RemoveForbiddenFilenameChars(mod_path));
    }

    std::optional<std::ifstream> GetPatternCacheFile(const std::filesystem::path& mod_path) {
      auto file_path = GetFilePath(mod_path);

      if (!std::filesystem::exists(file_path))
        return std::nullopt;

      std::ifstream file(file_path);
      if (!file.is_open())
        return std::nullopt;

      return std::move(file);
    }

    void WriteCacheFile(rapidjson::Document& json, const std::filesystem::path& mod_path, const std::string& md5, const CachedBatch::to_update_t& to_update) {
      auto file_path = GetFilePath(mod_path);
      LOG_DEBUG("Writing pattern cache file: {}", file_path.string());

      if (!json.HasMember("md5"))
        json.AddMember("md5", rapidjson::Value(md5.c_str(), static_cast<rapidjson::SizeType>(md5.size()), json.GetAllocator()), json.GetAllocator());

      rapidjson::Value* patterns;
      if (!json.HasMember("patterns")) {
        json.AddMember("patterns", rapidjson::Value(rapidjson::kObjectType), json.GetAllocator());

        patterns = &json["patterns"];
      } else {
        patterns = &json["patterns"];
      }
      for (auto&& entry : to_update) {
        auto key = rapidjson::Value(entry.joaat.c_str(), static_cast<rapidjson::SizeType>(entry.joaat.size()), json.GetAllocator());
        patterns->AddMember(key, entry.offset, json.GetAllocator());
      }

      json::ToFile(json, GetFilePath(mod_path));
    }

    Handle FindPattern(Range& region, Batch::entry& entry) {
      if (auto result = region.scan(entry.pattern_)) {
        if (entry.cb_) {
          std::invoke(std::move(entry.cb_), result);
          LOG_DEBUG("Found '{}' GTA5.exe+{:x}", entry.name_, (result.as<DWORD64>() - region.begin().as<DWORD64>()));

          return result;
        } else {
          LOG_CRITICAL("Failed to find '{}'.", entry.name_);

          return {};
        }
      } else {
        LOG_CRITICAL("Failed to find '{}'.", entry.name_);

        return {};
      }
    }
  }

  void CachedBatch::add(const std::string& name, char* pattern, std::function<void(Handle)> callback) {
    entries_.emplace_back(Batch::entry(name, Pattern(pattern), std::move(callback)), rage::joaat(pattern));
  }

  void CachedBatch::Run(Range region) {
    auto mod = common::GetModuleFromHModule(region.begin().as<HMODULE>());

    auto opt_file = GetPatternCacheFile(mod.szExePath);

    if (opt_file.has_value()) {
      auto json = json::FromFileStream(opt_file.value());
      if (!json.IsObject() && !json.HasMember("patterns")) {
        LOG_ERROR("Pattern cache json invalid rebuilding...");
        UPDATE_NO_CACHE(region);
        return;
      }

      if (json::GetSafe<const char*>(json.GetObj(), "md5") != common::GetFileMd5Hash(mod.szExePath)) {
        LOG_INFO("Pattern cache md5 mismatch rebuilding...");
        UPDATE_NO_CACHE(region);
        return;
      }

      LOG_DEBUG("Loading pattern cache...");
      auto res = LoadFromJson(region, json["patterns"].GetObj());
      if (res.has_value()) {
        LOG_DEBUG("Adding {} entries to the pattern cache", res.value().size());
        WriteCacheFile(json, mod.szExePath, common::GetFileMd5Hash(mod.szExePath), res.value());
      }
    } else {
      UPDATE_NO_CACHE(region);
    }
  }

  std::optional<CachedBatch::to_update_t> CachedBatch::LoadFromJson(Range& region, rapidjson::Value& json) {
    bool all_found = true;
    to_update_t to_update;

    for (auto&& entry : entries_) {
      if (!json.HasMember(std::to_string(entry.pattern_joaat).c_str())) {
        LOG_DEBUG("Cache miss for pattern '{}'", entry.entry.name_);
        cache_invalid:

        auto addr = FindPattern(region, entry.entry);
        if (addr == nullptr) {
          all_found = false;
          break;
        }

        to_update.push_back({addr.as<DWORD64>() - region.begin().as<DWORD64>(), std::to_string(entry.pattern_joaat)});
      } else {
        Handle res(region.begin().as<std::uintptr_t>() + json::GetSafe<std::uintptr_t>(json, std::to_string(entry.pattern_joaat)));

        if (res.as<std::uintptr_t>() == region.begin().as<std::uintptr_t>()) {
          LOG_ERROR("{} pattern cache invalid", entry.entry.name_);
          goto cache_invalid;
        }

        if (!entry.entry.cb_) {
          LOG_CRITICAL("{} missing cb", entry.entry.name_);
          all_found = false;
          break;
        }

        entry.entry.cb_(res);
        LOG_DEBUG("Found cached '{}' GTA5.exe+{:x}", entry.entry.name_, (res.as<DWORD64>() - region.begin().as<DWORD64>()));
      }
    }

    entries_.clear();
    if (!all_found) {
      #ifndef NDEBUG
      __debugbreak();
      #endif
      LOG_CRITICAL("Failed to find some patterns.");
      std::abort();
    } else {
      if (!to_update.empty()) {
        return to_update;
      }

      return std::nullopt;
    }
  }

  CachedBatch::to_update_t CachedBatch::LoadNoCache(Range& region) {
    bool all_found = true;
    to_update_t to_update;

    for (auto&& entry : entries_) {
      auto addr = FindPattern(region, entry.entry);
      if (addr == nullptr) {
        all_found = false;
        break;
      }

      to_update.push_back({addr.as<DWORD64>() - region.begin().as<DWORD64>(), std::to_string(entry.pattern_joaat)});
    }

    entries_.clear();
    if (!all_found) {
      #ifndef NDEBUG
      __debugbreak();
      #endif
      LOG_CRITICAL("Failed to find some patterns.");
      std::abort();
    } else {
      return to_update;
    }
  }
}