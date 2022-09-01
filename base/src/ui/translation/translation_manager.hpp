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
#include "../../logger/logger.hpp"

#define ADD_TRANSLATION(key, trans) {key, trans}

namespace gta_base {
  namespace ui {
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
          ADD_TRANSLATION("wanted_level", "Wanted Level"),
          ADD_TRANSLATION("wanted_level_desc", "Set the wanted level"),
        };

        robin_hood::unordered_map<std::string, std::string> translation_;
      };

    class TranslationManager;
    inline TranslationManager* kTRANSLATION_MANAGER{};

    class TranslationManager {
    public:
      template<class... Args>
      explicit TranslationManager(Args&&... args) {
        active_translation_ = std::make_unique<Translation>(std::forward<Args>(args)...);

        kTRANSLATION_MANAGER = this;
      }

      ~TranslationManager() {
        kTRANSLATION_MANAGER = nullptr;
      }

      inline std::shared_ptr<Translation> GetActiveTranslation() {
        return active_translation_;
      }

      inline void SetActiveTranslation(std::shared_ptr<Translation> translation) {
        active_translation_ = std::move(translation);
      }

      inline std::string_view Get(const std::string& key) {
        return (*active_translation_)[key];
      }

      inline std::string_view operator[](const std::string& key) {
        return (*active_translation_)[key];
      }

    private:
      std::shared_ptr<Translation> active_translation_;
    };
  }
}

#undef ADD_TRANSLATION
#endif //GTA_BASE_TRANSLATION_MANAGER_HPP
