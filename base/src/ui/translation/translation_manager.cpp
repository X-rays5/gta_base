//
// Created by X-ray on 09/01/22.
//

#include "translation_manager.hpp"
#include <inireader/inireader.hpp>

namespace gta_base {
  namespace ui {
    Translation::Translation(const std::filesystem::path& path) {
      if (!std::filesystem::exists(path))
        return;

      ini::Parser ini_reader;
      ini_reader.Parse(path);

      for (auto& [name, val] : ini_reader["translation"]) {
        if (!val.is<std::string>())
          continue;

        translation_[name] = val.as<std::string>();
      }
    }

    bool Translation::SaveToFile(const std::filesystem::path& path) {
      std::ofstream writer(path);
      if (!writer.is_open())
        return false;

      ini::Parser ini_writer;

      auto section = ini_writer.AddSection("translation");

      for (auto& [key, val] : translation_)
        section[key] = val;

      writer << ini_writer.Stringify();

      return true;
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
}