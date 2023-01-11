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
#include <xorstr.hpp>

namespace gta_base::ui {
  namespace translation {
    extern const robin_hood::unordered_map<std::string, std::string> default_translation;
  }

  class Translation {
  public:
    using translation_t = robin_hood::unordered_map<std::string, std::string>;

  public:
    Translation();

    explicit Translation(const translation_t& translation);

    explicit Translation(const std::filesystem::path& path);

    bool SaveToFile(const std::filesystem::path& path);

    FORCE_INLINE std::string_view operator[](const std::string& key) {
      auto it = translation_.find(key);

      if (it != translation_.end()) {
        return it->second;
      } else {
        return key;
      }
    }

  private:
    translation_t translation_;

  private:
    static translation_t LoadFromJson(const std::filesystem::path& path);

    static translation_t MergeDefaultTranslation(const translation_t& merge_into_default, bool* outdated);
  };

  class TranslationManager;

  inline TranslationManager* kTRANSLATION_MANAGER{};

  class TranslationManager {
  public:
    template<class... Args>
    FORCE_INLINE explicit TranslationManager(Args&& ... args) {
      active_translation_ = std::make_unique<Translation>(std::forward<Args>(args)...);

      kTRANSLATION_MANAGER = this;
    }

    ~TranslationManager() {
      kTRANSLATION_MANAGER = nullptr;
    }

    FORCE_INLINE std::shared_ptr<Translation> GetActiveTranslation() {
      std::unique_lock lock(mtx_);
      return active_translation_;
    }

    FORCE_INLINE void SetActiveTranslation(std::shared_ptr<Translation> translation) {
      std::unique_lock lock(mtx_);
      active_translation_ = std::move(translation);
    }

    FORCE_INLINE std::string Get(const std::string& key) {
      std::unique_lock lock(mtx_);
      return std::string((*active_translation_)[key]);
    }

    FORCE_INLINE std::string operator[](const std::string& key) {
      return Get(key);
    }

    FORCE_INLINE static std::vector<std::filesystem::path> GetTranslationList() {
      std::filesystem::directory_iterator it(common::GetTranslationDir());
      std::vector<std::filesystem::path> translation_list;

      for (const auto& entry: it) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
          translation_list.push_back(entry.path());
        }
      }

      return translation_list;
    }

  private:
    std::recursive_mutex mtx_;
    std::shared_ptr<Translation> active_translation_;
  };
}

FORCE_INLINE std::string operator ""_Translate(const char* str, std::size_t) {
  return gta_base::ui::kTRANSLATION_MANAGER->Get(str);
}

FORCE_INLINE std::string operator ""_T(const char* str, std::size_t) {
  return gta_base::ui::kTRANSLATION_MANAGER->Get(str);
}

#endif //GTA_BASE_TRANSLATION_MANAGER_HPP
