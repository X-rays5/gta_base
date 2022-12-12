//
// Created by X-ray on 11/03/22.
//

#include "data_loader.hpp"
#include <magic_enum.hpp>
#include "../../memory/pointers.hpp"
#include "../../misc/json.hpp"
#include "../util/fipack_reader.hpp"
#include "../../misc/thread_pool.hpp"
#include <json/json.h>

using namespace gta_base;

namespace rage::data {
  namespace {
    constexpr const char* build_version_key = "build_version";
    constexpr const char* online_version_key = "online_version";

    std::uint32_t VersionStrToInt(const std::string& ver) {
      return common::LexicalCast<std::uint32_t>(common::RemoveNonNumerical(ver));
    }

    void WriteVersionFile() {
      Json::StreamWriterBuilder json_writer_builder;
      Json::Value root;

      root[build_version_key] = VersionStrToInt(memory::kPOINTERS->gta_build_version_);
      root[online_version_key] = VersionStrToInt(memory::kPOINTERS->gta_online_version_);

      auto cache_file = common::GetCachedDir() / "version.json";
      std::ofstream out_file(cache_file);
      if (!out_file.is_open()) {
        LOG_ERROR("Failed to open {} for writing the cache version", cache_file.string());
      }

      out_file << Json::writeString(json_writer_builder, root);
    }

    void WriteVehicles(const Vehicles& vehicles) {
      Json::StreamWriterBuilder json_writer_builder;
      Json::Value root;

      for (int i = 0; i < vehicles.size(); i++) {
        auto& vehicle = vehicles[i];

        Json::Value obj;
        obj["raw_name"] = vehicle->raw_name;
        obj["display_name"] = vehicle->display_name;
        obj["raw_make"] = vehicle->raw_make;
        obj["display_make"] = vehicle->display_make;
        obj["class_name"] = vehicle->class_name;
        obj["model_hash"] = vehicle->model_hash;

        root[i] = obj;
      }

      auto cache_file = common::GetCachedDir() / "vehicles.json";
      std::ofstream out_file(cache_file);
      if (!out_file.is_open()) {
        LOG_ERROR("Failed to open {} for writing the vehicle cache", cache_file.string());
      }

      out_file << Json::writeString(json_writer_builder, root);
    }

    void WriteWeapons(const Weapons& weapons) {
      Json::StreamWriterBuilder json_writer_builder;
      Json::Value root;

      for (int i = 0; i < weapons.size(); i++) {
        auto& weapon = weapons[i];

        Json::Value obj;
        obj["raw_name"] = weapon->raw_name;
        obj["display_name"] = weapon->display_name;
        obj["type"] = weapon->type;
        obj["is_throwable"] = weapon->is_throwable;
        obj["is_gun"] = weapon->is_gun;
        obj["is_rechargeable"] = weapon->is_rechargeable;
        obj["is_vehicle_weapon"] = weapon->is_vehicle_weapon;
        obj["is_melee"] = weapon->is_melee;
        obj["is_unarmed"] = weapon->is_unarmed;
        obj["model_hash"] = weapon->model_hash;
        obj["reward_hash"] = weapon->reward_hash;
        obj["reward_ammo_hash"] = weapon->reward_ammo_hash;

        root[i] = obj;
      }

      auto cache_file = common::GetCachedDir() / "weapons.json";
      std::ofstream out_file(cache_file);
      if (!out_file.is_open()) {
        LOG_ERROR("Failed to open {} for writing the weapon cache", cache_file.string());
      }

      out_file << Json::writeString(json_writer_builder, root);
    }

    void WritePeds(const Peds& peds) {
      Json::StreamWriterBuilder json_writer_builder;
      Json::Value root;

      for (int i = 0; i < peds.size(); i++) {
        auto& ped = peds[i];

        Json::Value obj;
        obj["model_name"] = ped->model_name;
        obj["ped_type"] = ped->ped_type;
        obj["model_hash"] = ped->model_hash;

        root[i] = obj;
      }

      auto cache_file = common::GetCachedDir() / "peds.json";
      std::ofstream out_file(cache_file);
      if (!out_file.is_open()) {
        LOG_ERROR("Failed to open {} for writing the ped cache", cache_file.string());
      }

      out_file << Json::writeString(json_writer_builder, root);
    }
  }

  bool Loader::ShouldRebuildCache() {
    auto vehicle_cache = common::GetCachedDir() / "vehicles.json";
    auto weapon_cache = common::GetCachedDir() / "weapons.json";
    auto ped_cache = common::GetCachedDir() / "peds.json";

    if (!std::filesystem::exists(vehicle_cache) || !std::filesystem::exists(weapon_cache) || !std::filesystem::exists(ped_cache)) {
      return true;
    }

    auto json = json::FromFile(common::GetCachedDir() / "version.json");
    if (!json.IsObject())
      return true;

    auto build_ver = VersionStrToInt(memory::kPOINTERS->gta_build_version_);
    auto online_ver = VersionStrToInt(memory::kPOINTERS->gta_online_version_);
    auto cache_build_ver = json::GetSafe<std::uint32_t>(json, build_version_key);
    auto cache_online_ver = json::GetSafe<std::uint32_t>(json, online_version_key);

    return (build_ver != cache_build_ver || online_ver != cache_online_ver);
  }

  bool Loader::IsBadDlc(const std::string& dlc_name) {
    static const robin_hood::unordered_set<std::string> bad_dlc_names = {"mpG9EC"};

    if (bad_dlc_names.find(dlc_name) != bad_dlc_names.end()) {
      LOG_DEBUG("Skipping bad DLC: {}", dlc_name);
      return true;
    }

    return false;
  }

  robin_hood::unordered_set<std::string> Loader::GetItemFlags(const std::string& flag_str) {
    auto flags = common::SplitStr(flag_str, " ");
    robin_hood::unordered_set<std::string> ret;
    for (auto& flag : flags) {
      if (flag.empty())
        continue;
      ret.insert(flag.c_str());
    }

    return ret;
  }

  Data Loader::LoadData() {
    if (ShouldRebuildCache()) {
      LOG_INFO("Rebuilding gta data cache");
      auto data = LoadDataFromGameData();

      LOG_INFO("Writing gta data cache");
      WriteVersionFile();
      WriteVehicles(data.vehicles_);
      WriteWeapons(data.weapons_);
      WritePeds(data.peds_);

      return data;
    } else {
      LOG_INFO("Loading gta data cache");
      return {LoadVehiclesFromCache(), LoadWeaponsFromCache(), LoadPedsFromCache()};
    }
  }

  void Loader::ParseVehicles(const std::filesystem::path& file) {
    cur_rpf_wrapper_->ReadXmlFile(file, [&](const pugi::xml_document& doc){
      const auto& items = doc.select_nodes("/CVehicleModelInfo__InitDataList/InitDatas/Item");
      for (const auto& item_node : items) {
        const auto item = item_node.node();

        const auto hash = joaat(item.child("modelName").text().as_string());
        if (tmp_loading_veh_.contains(hash))
          continue;

        const auto name = item.child("gameName").text().as_string();
        const auto make = item.child("vehicleMakeName").text().as_string();
        const auto class_name = item.child("vehicleClass").text().as_string();

        Vehicle vehicle(name, make, class_name, hash);
        tmp_loading_veh_[hash] = std::make_shared<Vehicle>(vehicle);
      }
    });
  }

  void Loader::ParseWeapons(const std::filesystem::path& file) {
    cur_rpf_wrapper_->ReadXmlFile(file, [&](const pugi::xml_document& doc){
      const auto items = doc.select_nodes("/CWeaponInfoBlob/Infos/Item/Infos/Item[@type='CWeaponInfo']");
      for (const auto& item_node : items) {
        const auto item = item_node.node();

        const auto name = item.child("Name").text().as_string();
        const auto hash = joaat(name);

        if (tmp_loading_wep_.contains(hash))
          continue;

        const auto human_name_hash = item.child("HumanNameHash").text().as_string();
        if (human_name_hash == std::string("WT_INVALID"))
          continue;

        const auto display_name = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(human_name_hash);
        const auto weapon_flags = GetItemFlags(item.child("WeaponFlags").text().as_string());
        const bool is_throwable = weapon_flags.contains("Thrown");
        const bool is_gun = weapon_flags.contains("Gun");
        const bool is_rechargeable = weapon_flags.contains("DisplayRechargeTimeHUD");

        const std::string tmp_type = item.child("Group").text().as_string();
        std::string type;
        if (tmp_type.size() > 6)
          type = tmp_type.substr(6);

        if (type.empty() && std::string(name).starts_with("VEHICLE_"))
          type = "VEHICLE";

        const bool is_vehicle_weapon = type == "VEHICLE";
        const bool is_melee = type == "MELEE";
        const bool is_unarmed = type == "UNARMED";

        joaat_t reward_hash{};
        joaat_t ammo_reward_hash{};
        if (is_gun || !is_melee || !is_unarmed) {
          const std::string reward_prefix = "REWARD_";
          reward_hash = joaat(reward_prefix + name);

          if (is_gun && !is_rechargeable) {
            std::string weapon_id = name + 7;
            ammo_reward_hash = joaat(std::string(reward_prefix).append("AMMO_").append(weapon_id));
          }
        }

        Weapon weapon(name, display_name, type, is_throwable, is_gun, is_rechargeable, is_vehicle_weapon, is_melee, is_unarmed, hash, reward_hash, ammo_reward_hash);
        tmp_loading_wep_[hash] = std::make_shared<Weapon>(weapon);
      }
    });
  }

  void Loader::ParsePeds(const std::filesystem::path& file) {
    cur_rpf_wrapper_->ReadXmlFile(file, [&](const pugi::xml_document& doc){
      const auto items = doc.select_nodes("/CPedModelInfo__InitDataList/InitDatas/Item");
      for (const auto& item_node : items) {
        const auto item = item_node.node();

        const auto model_name = item.child("Name").text().as_string();
        const auto hash = joaat(model_name);

        if (tmp_loading_ped_.contains(hash))
          continue;

        const auto ped_type = item.child("Pedtype").text().as_string();

        Ped ped(model_name, ped_type, hash);
        tmp_loading_ped_[hash] = std::make_shared<Ped>(ped);
      }
    });
  }

  Data Loader::LoadDataFromGameData() {
    util::FipackReader::ForEachFipackFile([&](util::FipackReader& rpf_wrapper) -> std::size_t {
      cur_rpf_wrapper_ = &rpf_wrapper;
      const std::vector<std::filesystem::path> files = cur_rpf_wrapper_->GetFilePaths();
      for (const auto& file : files) {
        if (file.filename() == "setup2.xml") {
          cur_rpf_wrapper_->ReadXmlFile(file, [&](const pugi::xml_document& doc){
            auto item = doc.select_node("/SSetupData/nameHash");
            cur_dlc_name_ = item.node().text().as_string();
            LOG_DEBUG("Loading data from dlc: {}", cur_dlc_name_);
          });

          if (IsBadDlc(cur_dlc_name_)) {
            return 0;
          }
        } else if (file.filename() == "vehicles.meta") {
          ParseVehicles(file);
        } else if (const std::string file_str = file.string(); file_str.find("weapon") != std::string::npos && file.extension() == ".meta") {
          ParseWeapons(file);
        } else if (file.filename() == "peds.meta" || file.filename() == "peds.ymt") {
          ParsePeds(file);
        }
      }

      return files.size();
    });

    Vehicles vehicles;
    for (auto&& vehicle : tmp_loading_veh_) {
      vehicles.emplace_back(std::move(vehicle.second));
    }

    Weapons weapons;
    for (auto&& weapon : tmp_loading_wep_) {
      weapons.emplace_back(std::move(weapon.second));
    }

    Peds peds;
    for (auto&& ped : tmp_loading_ped_) {
      peds.emplace_back(std::move(ped.second));
    }

    return {vehicles, weapons, peds};
  }

  Vehicles Loader::LoadVehiclesFromCache() {
    LOG_INFO("Loading vehicles from cache");
    Vehicles res;
    auto file = common::GetCachedDir() / "vehicles.json";
    if (!std::filesystem::exists(file)) {
      LOG_ERROR("Failed to load vehicles from cache: file not found {}", file.string());
      return res;
    }

    auto json = json::FromFile(file);
    if (!json.IsArray()) {
      LOG_ERROR("Failed to load vehicles from cache: invalid json");
      return res;
    }

    for (auto&& vehicle_member : json.GetArray()) {
      res.push_back(std::make_shared<Vehicle>(vehicle_member.GetObj()));
    }

    return res;
  }

  Weapons Loader::LoadWeaponsFromCache() {
    LOG_INFO("Loading weapons from cache");
    Weapons res;
    auto file = common::GetCachedDir() / "weapons.json";
    if (!std::filesystem::exists(file)) {
      LOG_ERROR("Failed to load weapons from cache: file not found {}", file.string());
      return res;
    }

    auto json = json::FromFile(file);
    if (!json.IsArray()) {
      LOG_ERROR("Failed to load weapons from cache: invalid json");
      return res;
    }

    for (auto&& weapon_member : json.GetArray()) {
      res.push_back(std::make_shared<Weapon>(weapon_member.GetObj()));
    }

    return res;
  }

  Peds Loader::LoadPedsFromCache() {
    LOG_INFO("Loading peds from cache");
    Peds res;
    auto file = common::GetCachedDir() / "peds.json";
    if (!std::filesystem::exists(file)) {
      LOG_ERROR("Failed to load peds from cache: file not found {}", file.string());
      return res;
    }

    auto json = json::FromFile(file);
    if (!json.IsArray()) {
      LOG_ERROR("Failed to load peds from cache: invalid json");
      return res;
    }

    for (auto&& ped_member : json.GetArray()) {
      res.push_back(std::make_shared<Ped>(ped_member.GetObj()));
    }

    return res;
  }
}