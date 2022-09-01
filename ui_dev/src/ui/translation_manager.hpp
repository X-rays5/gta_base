//
// Created by X-ray on 09/01/22.
//

#pragma once
#ifndef GTA_BASE_TRANSLATION_MANAGER_HPP
#define GTA_BASE_TRANSLATION_MANAGER_HPP
#include <string>
#include <string_view>
#include <memory>
#include <filesystem>
#include <fstream>
#include <robin_hood.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#define ADD_TRANSLATION(key, trans) {key, trans}

namespace ui {
  namespace {
    class Translation {
    public:
      Translation() : translation_(default_translation) {}
      explicit Translation(robin_hood::unordered_map<std::string, std::string> translation) : translation_(std::move(translation)) {}
      explicit Translation(const std::filesystem::path& path);

      bool SaveToFile(const std::filesystem::path& path);

      std::string_view operator[](const std::string& key);

    private:
      inline static const robin_hood::unordered_map<std::string, std::string> default_translation = {
        ADD_TRANSLATION("option_godmode", "Godmode"),
        ADD_TRANSLATION("option_never_wanted", "Never Wanted"),
      };

      robin_hood::unordered_map<std::string, std::string> translation_;
    };
  }

  class TranslationManager {
  public:
    template<class... Args>
    explicit TranslationManager(Args&&... args) {
      active_translation_ = std::make_unique<Translation>(std::forward<Args>(args)...);
    }

    std::shared_ptr<Translation> GetActiveTranslation() {
      return active_translation_;
    }

    void SetActiveTranslation(std::shared_ptr<Translation> translation) {
      active_translation_ = std::move(translation);
    }

    std::string_view operator[](const std::string& key) {
      return (*active_translation_)[key];
    }

  private:
    std::shared_ptr<Translation> active_translation_;
  };
}

#undef ADD_TRANSLATION
#endif //GTA_BASE_TRANSLATION_MANAGER_HPP
