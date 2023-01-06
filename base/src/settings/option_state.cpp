//
// Created by X-ray on 01/06/23.
//

#include "option_state.hpp"
#include <vector>
#include "../misc/json.hpp"
#include "../ui/manager.hpp"

namespace gta_base::settings::option_state {
  void Save(const std::filesystem::path& file) {
    LOG_INFO("Saving settings {}", file.stem().string());

    std::vector<std::pair<std::string, std::string>> save_vals;
    ui::kMANAGER->IterateAllOptions([&](const std::shared_ptr<ui::Submenu>& sub, const std::shared_ptr<ui::option::BaseOption>& opt){
      if (opt->HasFlag(ui::OptionFlag::kSaveable)) {
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

      json::ToFile(json, file);
    } catch (std::exception& e) {
      LOG_ERROR(e.what());
    }
  }

  void SaveSingle(const std::filesystem::path& file, const std::string& key, const std::string& val) {
    if (!std::filesystem::exists(file)) {
      std::ofstream writer(file);
      writer << "{}";
    }

    try {
      auto json = json::FromFile(file);
      if (json.HasMember(key.c_str()))
        json.RemoveMember(key.c_str());

      json.AddMember(json::StringToJsonVal(key, json.GetAllocator()), json::StringToJsonVal(val, json.GetAllocator()), json.GetAllocator());

      json::ToFile(json, file);
    } catch (std::exception &e) {
      LOG_ERROR(e.what());
    }
  }

  void Load(const std::filesystem::path& file ) {
    if(!std::filesystem::exists(file))
      return;

    LOG_INFO("Loading settings {}", file.stem().string());

    std::vector<std::pair<std::string, std::string>> load_vals;

    try {
      auto json = json::FromFile(file);
      if(!json.IsObject())
        return;

      ui::kMANAGER->IterateAllOptions([&](const std::shared_ptr<ui::Submenu>& sub, const std::shared_ptr<ui::option::BaseOption>& opt) {
        if(opt->HasFlag(ui::OptionFlag::kSaveable)) {
          if(json.HasMember(opt->GetNameKey().data())) {
            opt->SetSavedVal(json[opt->GetNameKey().data()].GetString());
          }
        }
      });
    } catch(std::exception& e) {
      LOG_ERROR(e.what());
    }
  }
}