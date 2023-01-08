//
// Created by X-ray on 11/07/22.
//

#include "data_types.hpp"

namespace rage::data {
  void Data::InitVehiclesIdx() {
    for (auto&& vehicle: vehicles_) {
      auto veh = vehicle.second;

      vehicles_model_hash_idx_[veh->model_hash] = veh;
      vehicles_dlc_idx_[veh->dlc_name][veh->display_name] = veh;
      vehicles_class_idx_[veh->class_name][veh->display_name] = veh;
    }
  }

  void Data::InitWeaponsIdx() {
    for (auto&& weapon: weapons_) {
      weapons_model_hash_idx_[weapon->model_hash] = weapon;
      weapons_dlc_idx_[weapon->dlc_name].emplace_back(weapon);
    }
  }

  void Data::InitPedsIdx() {
    for (auto&& ped: peds_) {
      peds_model_hash_idx_[ped->model_hash] = ped;
      peds_dlc_idx_[ped->dlc_name].emplace_back(ped);
    }
  }
}
