//
// Created by X-ray on 10/06/22.
//

#include "json.hpp"
#include <fstream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <magic_enum.hpp>
#include "../logger/logger.hpp"

namespace gta_base::json {
  rapidjson::Value StringToJsonVal(const std::string& str, RAPIDJSON_DEFAULT_ALLOCATOR allocator) {
    return {str.c_str(), static_cast<rapidjson::SizeType>(str.size()), allocator};
  }

  std::string Stringify(rapidjson::Document& json) {
    rapidjson::StringBuffer strbuf;
    strbuf.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    json.Accept(writer);

    return strbuf.GetString();
  }

  rapidjson::Document FromFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
      LOG_ERROR("File doesn't exist: {}", path.string());
      return {};
    }

    std::ifstream reader(path);
    if (!reader.is_open()) {
      LOG_ERROR("Failed to open reader: {}", path.string());
      return {};
    }
    rapidjson::IStreamWrapper json_file(reader);
    rapidjson::Document json;
    if (json.ParseStream(json_file).HasParseError()) {
      LOG_ERROR("Failed to parse json: {}", magic_enum::enum_name(json.GetParseError()));
      return {};
    }

    return json;
  }

  bool ToFile(rapidjson::Document& json, const std::filesystem::path& path) {
    std::ofstream writer(path);
    if (!writer.is_open()) {
      LOG_ERROR("Failed to open file {} for writing", path.string());
      return false;
    }

    if (!json.IsObject())
      json.SetObject();

    rapidjson::StringBuffer str_buf;
    str_buf.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(str_buf);
    json.Accept(json_writer);

    writer << str_buf.GetString();

    return true;
  }
}