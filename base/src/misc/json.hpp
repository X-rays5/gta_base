//
// Created by X-ray on 10/06/22.
//

#pragma once
#ifndef GTA_BASE_JSON_HPP
#define GTA_BASE_JSON_HPP
#include <string>
#include <filesystem>
#include "rapidjson/document.h"

namespace gta_base::json {
  template<typename T>
  T GetSafe(rapidjson::Value& json, const std::string& key) {
    if (!json.IsObject())
      return T{};

    if (json.HasMember(key.c_str()))
      if (json[key.c_str()].Is<T>())
        return json[key.c_str()].Get<T>();

    return T{};
  }

  template<typename T>
  inline T GetSafe(rapidjson::Document& json, const std::string& key) {
    if (!json.IsObject())
      return T{};

    return GetSafe<T>(json.GetObj(), key);
  }

  rapidjson::Value StringToJsonVal(const std::string& str, RAPIDJSON_DEFAULT_ALLOCATOR allocator);

  std::string Stringify(rapidjson::Document& json);
  rapidjson::Document FromFile(const std::filesystem::path& path);
  bool ToFile(rapidjson::Document& json, const std::filesystem::path& path);
}
#endif //GTA_BASE_JSON_HPP
