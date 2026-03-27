//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include "settings_data.hpp"

namespace base::menu::feature {
  class Settings {
  public:
    Settings();
    ~Settings();

    Status Load(const std::string& profileName);
    Status Save(const std::string& profileName) const;

    std::weak_ptr<SettingsData> getSettings();

  private:
    std::shared_ptr<SettingsData> settings_data_;
  };
  inline Settings* kSETTINGS{};
}