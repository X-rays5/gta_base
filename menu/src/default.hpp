//
// Created by X-ray on 27/03/2026.
//

#pragma once

namespace base::menu {
  class Default {
    public:

    Default();
    ~Default();

    struct DefaultProfiles {
      std::string default_opt_settings = "default";
      std::string default_translations = "default";
      std::string default_theme = "default";
    };

    std::string GetDefaultOptSettings() {
      return default_profiles_.default_opt_settings;
    }

    void SetDefaultOptSettings(const std::string& settings) {
      default_profiles_.default_opt_settings = settings;
      Save();
    }

    std::string GetDefaultTranslations() {
      return default_profiles_.default_translations;
    }

    void SetDefaultTranslations(const std::string& translations) {
      default_profiles_.default_translations = translations;
      Save();
    }

    std::string GetDefaultTheme() {
      return default_profiles_.default_theme;
    }

    void SetDefaultTheme(const std::string& theme) {
      default_profiles_.default_theme = theme;
      Save();
    }

  private:
    DefaultProfiles default_profiles_{};

    void Save();
    void Load();
  };
  inline Default* kDEFAULT_PROFILES{};
};
