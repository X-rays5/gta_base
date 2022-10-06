//
// Created by X-ray on 09/18/22.
//

#include "settings.hpp"
#include <vector>
#include "json.hpp"
#include "../ui/manager.hpp"

namespace gta_base {
  Settings kSETTINGS{};

  namespace settings {
    namespace {
      inline std::filesystem::path SettingsPath() {
        return common::GetSettingsDir() / "settings.json";
      }
    }

    void Save() {
      LOG_INFO("Saving settings");

      std::vector<std::pair<std::string, std::string>> save_vals;
      ui::kMANAGER->IterateAllOptions([&](const std::shared_ptr<ui::Submenu>& sub, const std::shared_ptr<ui::option::BaseOption>& opt){
        if (opt->HasFlag(ui::OptionFlag::kSavable)) {
          LOG_DEBUG("save {}", opt->GetName());
          std::pair<std::string, std::string> save = {std::string(opt->GetNameKey()), opt->GetSaveVal()};
          save_vals.emplace_back(save);
        }
      });

      try {
        rapidjson::Document json;
        json.SetObject();
        for (auto&& save_entry : save_vals) {
          auto key = json::StringToJsonVal(save_entry.first, json.GetAllocator());
          auto val = json::StringToJsonVal(save_entry.second, json.GetAllocator());
          json.AddMember(key, val, json.GetAllocator());
        }

        json::ToFile(json, SettingsPath());
      } catch (std::exception& e) {
        LOG_ERROR(e.what());
      }
    }

    void Load() {
      if (!std::filesystem::exists(SettingsPath()))
        return;

      LOG_INFO("Loading settings");

      std::vector<std::pair<std::string, std::string>> load_vals;

      try {
        auto json = json::FromFile(SettingsPath());
        if (!json.IsObject())
          return;

        ui::kMANAGER->IterateAllOptions([&](const std::shared_ptr<ui::Submenu>& sub, const std::shared_ptr<ui::option::BaseOption>& opt) {
          if (opt->HasFlag(ui::OptionFlag::kSavable)) {
            if (json.HasMember(opt->GetNameKey().data())) {
              opt->SetSavedVal(json[opt->GetNameKey().data()].GetString());
            }
          }
        });
      } catch (std::exception& e) {
        LOG_ERROR(e.what());
      }
    }
  }
}