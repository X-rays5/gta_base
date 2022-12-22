//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_DATA_LOADER_HPP
#define GTA_BASE_DATA_LOADER_HPP
#include <optional>
#include <mutex>
#include "data_types.hpp"
#include "../util/fipack_reader.hpp"

namespace rage::data {
  class Loader {
  public:

    inline std::optional<Data> Load() {
      try {
        if (!mtx_.try_lock()) {
          LOG_ERROR("Failed to load game data, loader is busy");
          return std::nullopt;
        }

        data_ = LoadData();
        LOG_INFO("Loaded {} vehicles", data_.vehicles_.size());
        LOG_INFO("Loaded {} weapons", data_.weapons_.size());
        LOG_INFO("Loaded {} peds", data_.peds_.size());

        mtx_.unlock();
        return data_;
      } catch (const std::exception& e) {
        LOG_ERROR("Failed to load game data, {}", e.what());
        mtx_.unlock();
        return std::nullopt;
      }
    }

    inline std::optional<Data> Get() {
      if (data_.IsEmpty()) {
        return {};
      }
      return data_;
    }

    static std::string DlcNameToPrettyName(const std::string& dlc_name);

  private:
    std::recursive_mutex mtx_;
    Data data_;
    util::FipackReader* cur_rpf_wrapper_ = nullptr;
    std::string cur_dlc_name_;

    robin_hood::unordered_map<joaat_t, Vehicle_ptr_t> tmp_loading_veh_;
    robin_hood::unordered_set<std::string> tmp_loading_veh_dlc_;
    robin_hood::unordered_map<joaat_t, Weapon_ptr_t> tmp_loading_wep_;
    robin_hood::unordered_map<joaat_t, Ped_ptr_t> tmp_loading_ped_;
    robin_hood::unordered_set<std::string> tmp_loading_ped_dlc_;

  private:
    static bool ShouldRebuildCache();
    static bool IsBadDlc(const std::string& dlc_name);
    static robin_hood::unordered_set<std::string> GetItemFlags(const std::string& flag_str);
    Data LoadData();
    Data LoadDataFromGameData();
    void ParseVehicles(const std::filesystem::path& file);
    void ParseWeapons(const std::filesystem::path& file);
    void ParsePeds(const std::filesystem::path& file);
    static Vehicles LoadVehiclesFromCache();
    static Weapons LoadWeaponsFromCache();
    static Peds LoadPedsFromCache();
  };
}
#endif //GTA_BASE_DATA_LOADER_HPP
