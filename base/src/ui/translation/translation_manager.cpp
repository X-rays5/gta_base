//
// Created by X-ray on 09/01/22.
//

#include "translation_manager.hpp"
#include "../../misc/json.hpp"
#include <map>

namespace gta_base::ui {
  // This should only be used as a tmp placeholder during init
  Translation::Translation() {
    translation_ = translation::default_translation;
  }

  Translation::Translation(const translation_t& translation) {
    bool outdated;
    translation_ = MergeDefaultTranslation(translation, &outdated);

    if (outdated) {
      LOG_WARN("Translation file is outdated, but can't be automatically updated.");
    }
  }

  Translation::Translation(const std::filesystem::path& path) {
    bool outdated;
    translation_ = MergeDefaultTranslation(LoadFromJson(path), &outdated);

    if (outdated) {
      LOG_INFO("Translation file is outdated, updating...");
      SaveToFile(path);
    }
  }

  bool Translation::SaveToFile(const std::filesystem::path& path) {
    rapidjson::Document json;
    json.SetObject();

    // While this slows it down a bit. Saving doesn't need to be extremely fast. And at least the output will be consistent.
    std::map<std::string, std::string> sorted_translation;
    for (const auto& [key, value]: translation_) {
      sorted_translation[key] = value;
    }

    for (auto& [key, val]: sorted_translation) {
      auto json_key = json::StringToJsonVal(key, json.GetAllocator());
      auto json_val = json::StringToJsonVal(val, json.GetAllocator());

      json.AddMember(json_key, json_val, json.GetAllocator());
    }

    return json::ToFile(json, path);
  }

  std::string_view Translation::operator[](const std::string& key) {
    auto it = translation_.find(key);

    if (it != translation_.end()) {
      return it->second;
    } else {
      return key;
    }
  }

  Translation::translation_t Translation::LoadFromJson(const std::filesystem::path& path) {
    LOG_DEBUG("Loading translation from file: {}", path.string());
    if (!std::filesystem::exists(path))
      return {};

    auto json = json::FromFile(path);
    if (!json.IsObject()) {
      return {};
    }

    translation_t translation;
    for (auto&& [name, val]: json.GetObject()) {
      if (!val.IsString())
        continue;

      translation[name.GetString()] = val.GetString();
    }

    return std::move(translation);
  }

  Translation::translation_t Translation::MergeDefaultTranslation(const translation_t& merge_into_default, bool* outdated) {
    translation_t translation = translation::default_translation;

    std::size_t valid{};
    for (auto&& [orig, trans]: merge_into_default) {
      if (translation.find(orig) != translation.end()) {
        valid++;
      } else {
        LOG_WARN("Unknown translation key: {}", orig);
      }
      translation[orig] = trans;
    }

    if (translation::default_translation.size() > valid) {
      if (outdated)
        *outdated = true;
    } else {
      if (outdated)
        *outdated = false;
    }

    return std::move(translation);
  }
}