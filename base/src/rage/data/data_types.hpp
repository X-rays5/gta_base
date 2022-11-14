//
// Created by X-ray on 11/07/22.
//

#pragma once
#ifndef GTA_BASE_DATA_TYPES_HPP
#define GTA_BASE_DATA_TYPES_HPP
#include <utility>
#include <vector>
#include <memory>
#include <robin_hood.h>
#include <magic_enum.hpp>
#include <map>
#include "../../misc/json.hpp"
#include "../joaat.hpp"

namespace rage::data {
  struct Vehicle {
    enum class Class {
      kUnknown,
      kSuper, // VC_SUPER
      kSport, // VC_SPORT
      kSportClassic, // VC_SPORT_CLASSIC
      kMuscle, // VC_MUSCLE
      kSedan, // VC_SEDAN
      kCompact, // VC_COMPACT
      kCoupe, // VC_COUPE
      kSuv, // VC_SUV
      kOffroad, // VC_OFF_ROAD
      kOpenWheel, // VC_OPEN_WHEEL
      kVan, // VC_VAN
      kMotorcycle, // VC_MOTORCYCLE
      kCycle, // VC_CYCLE
      kService, // VC_SERVICE
      kUtility, // VC_UTILITY
      kIndustrial, // VC_INDUSTRIAL
      kCommercial, // VC_COMMERCIAL
      kEmergency, // VC_EMERGENCY
      kMilitary, // VC_MILITARY
      kRail, // VC_RAIL
      kBoat, // VC_BOAT
      kPlane, // VC_PLANE
      kHelicopter, // VC_HELICOPTER
    };

    Vehicle(const std::string& raw_name, const std::string& raw_make, const std::string& class_name, joaat_t hash) :
      raw_name(raw_name), display_name(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(raw_name.c_str())),
      raw_make(raw_make), display_make(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(raw_make.c_str())),
      class_name(CorrectClassName(class_name)), vehicle_class(GetClass(this->class_name)), model_hash(hash) {}

    explicit Vehicle(rapidjson::Value& obj) {
      if (!obj.IsObject())
        LOG_ERROR("Failed to parse vehicle data, invalid json object");

      const_cast<std::string&>(raw_name) = obj["raw_name"].GetString();
      const_cast<std::string&>(display_name) = obj["display_name"].GetString();
      const_cast<std::string&>(raw_make) = obj["raw_make"].GetString();
      const_cast<std::string&>(display_make) = obj["display_make"].GetString();
      const_cast<std::string&>(class_name) = obj["class_name"].GetString();
      const_cast<Class&>(vehicle_class) = GetClass(class_name);
      const_cast<joaat_t&>(model_hash) = obj["model_hash"].GetUint64();
    }

    const std::string raw_name;
    const std::string display_name;
    const std::string raw_make;
    const std::string display_make;
    const std::string class_name;
    const Class vehicle_class{};
    const joaat_t model_hash{};

  private:
    static std::string CorrectClassName(const std::string& class_name) {
      std::string res = class_name;

      if (res.find("VC_") != std::string::npos)
        res = res.substr(3);

      static const robin_hood::unordered_map<std::string, std::string> class_map_ = {
        {"COMPACTS", "COMPACT"}
      };

      if (class_map_.contains(res))
        return class_map_.at(res);

      return res;
    }

    static Class GetClass(const std::string& class_name) {
      static const robin_hood::unordered_map<std::string, Class> class_map_ = {
        {"SUPER", Class::kSuper},
        {"SPORT", Class::kSport},
        {"SPORT_CLASSIC", Class::kSportClassic},
        {"MUSCLE", Class::kMuscle},
        {"SEDAN", Class::kSedan},
        {"COMPACT", Class::kCompact},
        {"COUPE", Class::kCoupe},
        {"COMPACTS", Class::kCompact},
        {"SUV", Class::kSuv},
        {"OFF_ROAD", Class::kOffroad},
        {"OPEN_WHEEL", Class::kOpenWheel},
        {"VAN", Class::kVan},
        {"MOTORCYCLE", Class::kMotorcycle},
        {"CYCLE", Class::kCycle},
        {"SERVICE", Class::kService},
        {"UTILITY", Class::kUtility},
        {"INDUSTRIAL", Class::kIndustrial},
        {"COMMERCIAL", Class::kCommercial},
        {"EMERGENCY", Class::kEmergency},
        {"MILITARY", Class::kMilitary},
        {"RAIL", Class::kRail},
        {"BOAT", Class::kBoat},
        {"PLANE", Class::kPlane},
        {"HELICOPTER", Class::kHelicopter},
      };

      auto itr = class_map_.find(class_name);
      if (itr != class_map_.end()) {
        return itr->second;
      }

      LOG_WARN("Unknown vehicle class: {}", class_name);
      return Class::kUnknown;
    }
  };
  using Vehicle_ptr_t = std::shared_ptr<Vehicle>;
  using Vehicles = std::vector<Vehicle_ptr_t>;

  struct Weapon {
    Weapon(std::string raw_name, std::string display_name, std::string type, bool is_throwable, bool is_gun, bool is_rechargeable, bool is_melee, bool is_unarmed, joaat_t hash, joaat_t reward_hash, joaat_t reward_ammo_hash) :
      raw_name(std::move(raw_name)), display_name(std::move(display_name)),
      type(std::move(type)),
      is_throwable(is_throwable), is_gun(is_gun), is_rechargeable(is_rechargeable), is_melee(is_melee), is_unarmed(is_unarmed),
      model_hash(hash), reward_hash(reward_hash), reward_ammo_hash(reward_ammo_hash) {}

    explicit Weapon(rapidjson::Value& obj) {
      if (!obj.IsObject())
        LOG_ERROR("Failed to parse weapon data, invalid json object");

      const_cast<std::string&>(raw_name) = obj["raw_name"].GetString();
      const_cast<std::string&>(display_name) = obj["display_name"].GetString();
      robin_hood::unordered_set<std::string> tmp_flags;
      const_cast<std::string&>(type) = obj["type"].GetString();
      const_cast<bool&>(is_throwable) = obj["is_throwable"].GetBool();
      const_cast<bool&>(is_gun) = obj["is_gun"].GetBool();
      const_cast<bool&>(is_rechargeable) = obj["is_rechargeable"].GetBool();
      const_cast<bool&>(is_melee) = obj["is_melee"].GetBool();
      const_cast<bool&>(is_unarmed) = obj["is_unarmed"].GetBool();
      const_cast<joaat_t&>(model_hash) = obj["model_hash"].GetUint();
      const_cast<joaat_t&>(reward_hash) = obj["reward_hash"].GetUint();
      const_cast<joaat_t&>(reward_ammo_hash) = obj["reward_ammo_hash"].GetUint();
    }

    const std::string raw_name;
    const std::string display_name;
    const std::string type;
    const bool is_throwable{};
    const bool is_gun{};
    const bool is_rechargeable{};
    const bool is_melee{};
    const bool is_unarmed{};
    const joaat_t model_hash{};
    const joaat_t reward_hash{};
    const joaat_t reward_ammo_hash{};
  };
  using Weapon_ptr_t = std::shared_ptr<Weapon>;
  using Weapons = std::vector<Weapon_ptr_t>;

  struct Ped {
    Ped(std::string model_name, std::string ped_type, joaat_t hash) :
      model_name(std::move(model_name)), ped_type(std::move(ped_type)),
      model_hash(hash) {}

    explicit Ped(rapidjson::Value& obj) {
      if (!obj.IsObject())
        LOG_ERROR("Failed to parse ped data, invalid json object");

      const_cast<std::string&>(model_name) = obj["model_name"].GetString();
      const_cast<std::string&>(ped_type) = obj["ped_type"].GetString();
      const_cast<joaat_t&>(model_hash) = obj["model_hash"].GetUint();
    }

    const std::string model_name;
    const std::string ped_type;
    const joaat_t model_hash{};
  };
  using Ped_ptr_t = std::shared_ptr<Ped>;
  using Peds = std::vector<Ped_ptr_t>;

  class Data {
  public:
    const Vehicles vehicles_;
    const Weapons weapons_;
    const Peds peds_;

  public:
    Data() = default;
    Data(Vehicles vehicles, Weapons weapons, Peds peds) {
      vehicles.shrink_to_fit();
      weapons.shrink_to_fit();
      peds.shrink_to_fit();

      const_cast<Vehicles&>(vehicles_) = std::move(vehicles);
      const_cast<Weapons&>(weapons_) = std::move(weapons);
      const_cast<Peds&>(peds_) = std::move(peds);

      InitVehiclesIdx();
      InitWeaponsIdx();
      InitPedsIdx();
    };

    Data(const Data& data) = default;
    Data& operator=(const Data& data) = default;

    Data(Data&& data) noexcept {
      const_cast<Vehicles&>(vehicles_) = data.vehicles_;
      const_cast<Weapons&>(weapons_) = data.weapons_;
      const_cast<Peds&>(peds_) = data.peds_;

      const_cast<robin_hood::unordered_map<joaat_t, Vehicle_ptr_t>&>(vehicles_model_hash_idx_) = std::move(data.vehicles_model_hash_idx_);
      const_cast<std::map<std::string, Vehicles>&>(vehicles_class_idx_) = std::move(data.vehicles_class_idx_);
      const_cast<robin_hood::unordered_map<joaat_t, Weapon_ptr_t>&>(weapons_model_hash_idx_) = std::move(data.weapons_model_hash_idx_);
      const_cast<robin_hood::unordered_map<joaat_t, Ped_ptr_t>&>(peds_model_hash_idx_) = std::move(data.peds_model_hash_idx_);
    }

    Data& operator=(Data&& data) noexcept {
      const_cast<Vehicles&>(vehicles_) = data.vehicles_;
      const_cast<Weapons&>(weapons_) = data.weapons_;
      const_cast<Peds&>(peds_) = data.peds_;

      const_cast<robin_hood::unordered_map<joaat_t, Vehicle_ptr_t>&>(vehicles_model_hash_idx_) = std::move(data.vehicles_model_hash_idx_);
      const_cast<std::map<std::string, Vehicles>&>(vehicles_class_idx_) = std::move(data.vehicles_class_idx_);
      const_cast<robin_hood::unordered_map<joaat_t, Weapon_ptr_t>&>(weapons_model_hash_idx_) = std::move(data.weapons_model_hash_idx_);
      const_cast<robin_hood::unordered_map<joaat_t, Ped_ptr_t>&>(peds_model_hash_idx_) = std::move(data.peds_model_hash_idx_);

      return *this;
    }

    inline const Vehicles& GetVehiclesForClass(const std::string& veh_class) {
      return vehicles_class_idx_.find(veh_class)->second;
    }

    inline void IterateVehicleClasses(std::function<void(const std::string&, const Vehicles&)> func) {
      for (const auto& [key, value] : vehicles_class_idx_) {
        func(key, value);
      }
    }

    inline Vehicle_ptr_t GetVehicleByHash(rage::joaat_t hash) {
      return vehicles_model_hash_idx_.find(hash)->second;
    }

    /// @note calling GetVehicleByHash is preferred when RAGE_JOAAT macro can be used for compile time hashing
    inline Vehicle_ptr_t GetVehicleByName(const std::string& name) {
      return GetVehicleByHash(rage::joaat(name));
    }

    inline Weapon_ptr_t GetWeaponByHash(rage::joaat_t hash) {
      return weapons_model_hash_idx_.find(hash)->second;
    }

    /// @note calling GetWeaponByHash is preferred when RAGE_JOAAT macro can be used for compile time hashing
    inline Weapon_ptr_t GetWeaponByName(const std::string& name) {
      return GetWeaponByHash(rage::joaat(name));
    }

    inline Ped_ptr_t GetPedByHash(rage::joaat_t hash) {
      return peds_model_hash_idx_.find(hash)->second;
    }

    /// @note calling GetPedByHash is preferred when RAGE_JOAAT macro can be used for compile time hashing
    inline Ped_ptr_t GetPedByName(const std::string& name) {
      return GetPedByHash(rage::joaat(name));
    }

    inline bool IsEmpty() {
      return vehicles_.empty() && weapons_.empty() && peds_.empty();
    }

  private:
    robin_hood::unordered_map<joaat_t, Vehicle_ptr_t> vehicles_model_hash_idx_;
    std::map<std::string, Vehicles> vehicles_class_idx_;

    robin_hood::unordered_map<joaat_t, Weapon_ptr_t> weapons_model_hash_idx_;

    robin_hood::unordered_map<joaat_t, Ped_ptr_t> peds_model_hash_idx_;

  private:

    void InitVehiclesIdx();
    void InitWeaponsIdx();
    void InitPedsIdx();
  };
}
#endif //GTA_BASE_DATA_TYPES_HPP
