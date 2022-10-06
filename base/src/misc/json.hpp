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
    rapidjson::Value StringToJsonVal(const std::string& str, RAPIDJSON_DEFAULT_ALLOCATOR allocator);

    std::string Stringify(rapidjson::Document& json);
    rapidjson::Document FromFile(const std::filesystem::path& path);
    bool ToFile(rapidjson::Document& json, const std::filesystem::path& path);
  }
#endif //GTA_BASE_JSON_HPP
