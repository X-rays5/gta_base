//
// Created by X-ray on 10/06/22.
//

#pragma once
#ifndef GTA_BASE_JSON_HPP
#define GTA_BASE_JSON_HPP
#include <string>
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include "common.hpp"

#define RAPIDJSON_WRITER_REMOVE_PTR_FROM_KEY(key) gta_base::common::StripVarName(key).c_str()
#define RAPIDJSON_WRITER_K(key) writer.String(RAPIDJSON_WRITER_REMOVE_PTR_FROM_KEY(#key))
#define RAPIDJSON_WRITER_KV_STR(val) RAPIDJSON_WRITER_K(val); writer.String(val)
#define RAPIDJSON_WRITER_KV_UINT(val) RAPIDJSON_WRITER_K(val); writer.Uint(val)
#define RAPIDJSON_WRITER_KV_INT(val) RAPIDJSON_WRITER_K(val); writer.Int(val)
#define RAPIDJSON_WRITER_KV_UINT64(val) RAPIDJSON_WRITER_K(val); writer.Uint64(val)
#define RAPIDJSON_WRITER_KV_INT64(val) RAPIDJSON_WRITER_K(val); writer.Int64(val)
#define RAPIDJSON_WRITER_KV_BOOL(val) RAPIDJSON_WRITER_K(val); writer.Bool(val)

namespace gta_base::json {
  template<typename T>
  inline T GetSafe(rapidjson::Value& json, const std::string& key) {
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
  rapidjson::Document FromFileStream(std::ifstream& stream);
  rapidjson::Document FromFile(const std::filesystem::path& path);
  bool ToFile(rapidjson::Document& json, const std::filesystem::path& path, std::size_t indent = 4);
}
#endif //GTA_BASE_JSON_HPP
