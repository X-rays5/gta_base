//
// Created by X-ray on 11/13/22.
//

#include "vehicle.hpp"
#include "tab_includes.hpp"
#include "../../rage/util/vehicle.hpp"

namespace gta_base::ui::tabs {
  static std::string selected_category_spawner;
  void VehicleTab() {
    kMANAGER->AddSubmenu(Submenus::Vehicle, "tab/title/vehicle", [](Submenu* sub) {
      sub->AddOption(option::SubmenuOption("tab/title/vehicle_spawner", "", Submenus::VehicleSpawner));
    });

    kMANAGER->AddSubmenu(Submenus::VehicleSpawner, "tab/title/vehicle_spawner", [](Submenu* sub){
      globals::gta_data.IterateVehicleClasses([&](const std::string& class_name, const rage::data::Vehicles& vehicles){
        sub->AddOption(option::SubmenuOption(class_name, "", Submenus::VehicleSpawnerSelectedCategory, [&]{
          selected_category_spawner = class_name;
        }));
      });
    });

    kMANAGER->AddSubmenu(Submenus::VehicleSpawnerSelectedCategory, selected_category_spawner, [](Submenu* sub){
      if (selected_category_spawner.empty())
        return;

      auto& vehicle = globals::gta_data.GetVehiclesForClass(selected_category_spawner);

      for (auto& veh : vehicle) {
        sub->AddOption(option::ExecuteOption(veh->display_name, "", [&]{
          fiber::kPOOL->AddJob([&]{
            rage::util::vehicle::Spawn(veh->model_hash, rage::util::vehicle::GetSpawnLocation(false), ENTITY::GET_ENTITY_HEADING(globals::local_player.ped_id));
          });
        }));
      }
    });
  }
}