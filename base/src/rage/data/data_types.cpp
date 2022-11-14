//
// Created by X-ray on 11/07/22.
//

#include "data_types.hpp"

namespace rage::data {
  void Data::InitVehiclesIdx() {
    for (auto&& vehicle : vehicles_) {
      vehicles_model_hash_idx_[vehicle->model_hash] = vehicle;
      vehicles_class_idx_[vehicle->class_name].emplace_back(vehicle);
    }
  }

  void Data::InitWeaponsIdx() {
    for (auto&& weapon : weapons_) {
      weapons_model_hash_idx_[weapon->model_hash] = weapon;
    }
  }

  void Data::InitPedsIdx() {
    for (auto&& ped : peds_) {
      peds_model_hash_idx_[ped->model_hash] = ped;
    }
  }
}
