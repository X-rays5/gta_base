//
// Created by X-ray on 09/01/22.
//

#include "translation_manager.hpp"
#include "../../../base/src/ui/translation/translation_manager.hpp"

namespace ui {

  Translation::Translation(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
      return;

    std::ifstream reader(path);
    if (!reader.is_open())
      return;

    rapidjson::IStreamWrapper json_file(reader);
    rapidjson::Document doc;
    if (doc.ParseStream(json_file).HasParseError())
      return;

    reader.close();

    if (!doc.IsObject())
      return;

    for (auto&& member : doc.GetObject()) {
      if (!member.value.IsString())
        continue;

      translation_.insert({member.name.GetString(), member.value.GetString()});
    }
  }

  bool Translation::SaveToFile(const std::filesystem::path& path) {
    std::ofstream writer(path);
    if (!writer.is_open())
      return false;

    rapidjson::Document doc;
    doc.SetObject();

    for (auto&& member : translation_) {
      rapidjson::Value key(member.first.c_str(), doc.GetAllocator());
      rapidjson::Value value(member.second.c_str(), doc.GetAllocator());
      doc.AddMember(key, value, doc.GetAllocator());
    }

    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(buffer);
    doc.Accept(json_writer);

    writer << buffer.GetString();

    writer.close();
    return true;
  }

  std::string_view Translation::operator[](const std::string& key) {
    auto it = translation_.find(key);

    if (it != translation_.end())
      return it->second;
    else
      return key;
  }

  TranslationManager::~TranslationManager() {
    kTRANSLATION_MANAGER = nullptr;
  }
}