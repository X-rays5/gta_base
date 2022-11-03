//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_DATA_LOADER_HPP
#define GTA_BASE_DATA_LOADER_HPP
#include <vector>

namespace rage::data::loader {
  struct Vehicle {

  };
  using Vehicles = std::vector<Vehicle>;

  struct Weapon {

  };
  using Weapons = std::vector<Weapon>;

  struct Ped {

  };
  using Peds = std::vector<Ped>;

  struct Data {
    Vehicles vehicles;
    Weapons weapons;
    Peds peds;
  };

  Data LoadAll();
  Vehicles LoadVehicles(bool load_all = false);
  Weapons LoadWeapons(bool load_all = false);
  Peds LoadPeds(bool load_all = false);
}
#endif //GTA_BASE_DATA_LOADER_HPP
