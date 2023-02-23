//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_THEME_87A5ADA11F7B4BAAB1728AF8F2BF08F2_HPP
#define GTA_BASE_THEME_87A5ADA11F7B4BAAB1728AF8F2BF08F2_HPP
#include "../../settings/profile.hpp"
#include "../../misc/json.hpp"

namespace gta_base::ui::draw {
  struct Theme {
  public:
    std::atomic<float> base_x = 0.0f;
    static void SaveTheme(Theme theme) {
      rapidjson::Document json;

      auto path = GetPath();
      json::ToFile(json, path);
    }

    static Theme LoadTheme() {
      auto json = json::FromFile(GetPath());
    }

  private:
    static inline std::filesystem::path GetPath() {
      return common::GetThemesDir() / (settings::profile::GetSelectedTheme() + ".json");
    }
  };
}
#endif //GTA_BASE_THEME_87A5ADA11F7B4BAAB1728AF8F2BF08F2_HPP
