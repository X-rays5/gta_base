//
// Created by X-ray on 09/01/22.
//

#include "translation_manager.hpp"
#include "../../misc/json.hpp"

namespace gta_base::ui {
    Translation::Translation(const std::filesystem::path& path) {
      if (!std::filesystem::exists(path))
        return;

      auto json = json::FromFile(path);
      if (!json.IsObject()) {
        *this = Translation();
        return;
      }

      for (auto&& [name, val] : json.GetObject()) {
        if (!val.IsString())
          continue;

        translation_[name.GetString()] = val.GetString();
      }
    }

    bool Translation::SaveToFile(const std::filesystem::path& path) {

      rapidjson::Document json;
      json.SetObject();

      for (auto& [key, val] : translation_) {
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
  }