//
// Created by X-ray on 11/03/22.
//

#include "data_loader.hpp"
#include "../../memory/pointers.hpp"
#include "../../misc/common.hpp"
#include "../../misc/json.hpp"
#include "data_manager.hpp"

// file structure:
// json
// build_version: gta build
// online_version: online build
// data: array

using namespace gta_base;

namespace rage::data::loader {
  constexpr static const char* build_version_key = "build_version";
  constexpr static const char* online_version_key = "online_version";

  bool CanStartLoad() {
    return kDATA_MANAGER->GetState() == Manager::kIDLE;
  }

  Data LoadAll() {
    if (!CanStartLoad()) {
      return {};
    }
    kDATA_MANAGER->SetState(Manager::kLOADING);

    kDATA_MANAGER->GetLoaderMtx()->lock();
    LoadVehicles(true);
    LoadWeapons(true);
    LoadPeds(true);
    kDATA_MANAGER->GetLoaderMtx()->unlock();

    kDATA_MANAGER->SetState(Manager::kIDLE);

    return {};
  }

  Vehicles LoadVehiclesFromCache(rapidjson::Document& json);
  Vehicles LoadVehiclesFromGameData();
  Vehicles LoadVehicles(bool load_all) {
    kDATA_MANAGER->GetLoaderMtx()->lock();
    kDATA_MANAGER->SetState(Manager::kLOADING);

    Vehicles res;
    rapidjson::Document json;
    auto path = common::GetCachedDir() / "vehicles.json";

    if (!std::filesystem::is_regular_file(path)) {
      res = LoadVehiclesFromGameData();
      goto done;
    }

    json = json::FromFile(path);
    if (!json.IsObject()) {
      LOG_WARN("wrong format {} rebuilding vehicle cache", path.filename().string());
      res = LoadVehiclesFromGameData();
      goto done;
    }

    if (strcmp(json::GetSafe<const char*>(json, build_version_key), memory::kPOINTERS->gta_build_version_) != 0 || strcmp(json::GetSafe<const char*>(json, online_version_key), memory::kPOINTERS->gta_online_version_) != 0) {
      LOG_INFO("wrong build version {} rebuilding vehicle cache", path.filename().string());
      res = LoadVehiclesFromGameData();
      goto done;
    }

    res = LoadVehiclesFromCache(json);

    done:
      if (load_all) {
        kDATA_MANAGER->SetState(Manager::kIDLE);
        kDATA_MANAGER->GetLoaderMtx()->unlock();
      }
      return res;
  }

  Weapons LoadWeaponsFromCache(rapidjson::Document& json);
  Weapons LoadWeaponsFromGameData();
  Weapons LoadWeapons(bool load_all) {
    kDATA_MANAGER->GetLoaderMtx()->lock();
    kDATA_MANAGER->SetState(Manager::kLOADING);

    Weapons res;
    rapidjson::Document json;
    auto path = common::GetCachedDir() / "weapons.json";

    if (!std::filesystem::is_regular_file(path)) {
      res = LoadWeaponsFromGameData();
      goto done;
    }

    json = json::FromFile(path);
    if (!json.IsObject()) {
      LOG_WARN("wrong format {} rebuilding weapon cache", path.filename().string());
      res = LoadWeaponsFromGameData();
      goto done;
    }

    if (strcmp(json::GetSafe<const char*>(json, build_version_key), memory::kPOINTERS->gta_build_version_) != 0 || strcmp(json::GetSafe<const char*>(json, online_version_key), memory::kPOINTERS->gta_online_version_) != 0) {
      LOG_INFO("wrong build version {} rebuilding weapon cache", path.filename().string());
      res = LoadWeaponsFromGameData();
      goto done;
    }

    res = LoadWeaponsFromCache(json);

    done:
      if (load_all) {
        kDATA_MANAGER->SetState(Manager::kIDLE);
        kDATA_MANAGER->GetLoaderMtx()->unlock();
      }
      return res;
  }

  Peds LoadPedsFromCache(rapidjson::Document& json);
  Peds LoadPedsFromGameData();
  Peds LoadPeds(bool load_all) {
    kDATA_MANAGER->GetLoaderMtx()->lock();
    kDATA_MANAGER->SetState(Manager::kLOADING);

    Peds res;
    rapidjson::Document json;
    auto path = common::GetCachedDir() / "peds.json";

    if (!std::filesystem::is_regular_file(path)) {
      res = LoadPedsFromGameData();
      goto done;
    }

    json = json::FromFile(path);
    if (!json.IsObject()) {
      LOG_WARN("wrong format {} rebuilding ped cache", path.filename().string());
      res = LoadPedsFromGameData();
      goto done;
    }

    if (strcmp(json::GetSafe<const char*>(json, build_version_key), memory::kPOINTERS->gta_build_version_) != 0 || strcmp(json::GetSafe<const char*>(json, online_version_key), memory::kPOINTERS->gta_online_version_) != 0) {
      LOG_INFO("wrong build version {} rebuilding ped cache", path.filename().string());
      res = LoadPedsFromGameData();
      goto done;
    }

    res = LoadPedsFromCache(json);

    done:
      if (load_all) {
        kDATA_MANAGER->SetState(Manager::kIDLE);
        kDATA_MANAGER->GetLoaderMtx()->unlock();
      }
      return res;
  }

  Vehicles LoadVehiclesFromCache(rapidjson::Document& json) {
    kDATA_MANAGER->SetState(Manager::kLOADING);
    LOG_INFO("Loading vehicles from cache");

    return {};
  }

  Vehicles LoadVehiclesFromGameData() {
    kDATA_MANAGER->SetState(Manager::kREBUILDING);
    LOG_INFO("Loading vehicles from game data");

    return {};
  }

  Weapons LoadWeaponsFromCache(rapidjson::Document& json) {
    kDATA_MANAGER->SetState(Manager::kLOADING);
    LOG_INFO("Loading weapons from cache");

    return {};
  }

  Weapons LoadWeaponsFromGameData() {
    kDATA_MANAGER->SetState(Manager::kREBUILDING);
    LOG_INFO("Loading weapons from game data");

    return {};
  }

  Peds LoadPedsFromCache(rapidjson::Document& json) {
    kDATA_MANAGER->SetState(Manager::kLOADING);
    LOG_INFO("Loading peds from cache");

    return {};
  }

  Peds LoadPedsFromGameData() {
    kDATA_MANAGER->SetState(Manager::kREBUILDING);
    LOG_INFO("Loading peds from game data");

    return {};
  }
}