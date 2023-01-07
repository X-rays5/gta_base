//
// Created by X-ray on 09/18/22.
//

#include "main_tab.hpp"
#include "tab_includes.hpp"
#include "debug.hpp"
#include "misc.hpp"
#include "network.hpp"
#include "self.hpp"
#include "settings.hpp"
#include "world.hpp"
#include "weapon.hpp"
#include "vehicle.hpp"

namespace gta_base::ui::tabs {
void MainTab() {
  try {
    kMANAGER->AddSubmenu(Submenus::Home, "tab/title/home", [](Submenu *sub) {
      sub->AddOption(option::ExecuteOption("fix veh", "", [] {
        fiber::kPOOL->AddJob([] {
          if (PED::IS_PED_IN_ANY_VEHICLE(globals::local_player.ped_id, false)) {
            VEHICLE::SET_VEHICLE_FIXED(globals::local_player.vehicle_id);
            VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(globals::local_player.vehicle_id);
            VEHICLE::SET_VEHICLE_DIRT_LEVEL(globals::local_player.vehicle_id, 0);
          }
        });
      }));
      sub->AddOption(option::SubmenuOption("tab/title/self", "", Submenus::Player));
      sub->AddOption(option::SubmenuOption("tab/title/weapon", "", Submenus::Weapon));
      sub->AddOption(option::SubmenuOption("tab/title/vehicle", "", Submenus::Vehicle));
      sub->AddOption(option::SubmenuOption("tab/title/network", "", Submenus::Network));
      sub->AddOption(option::SubmenuOption("tab/title/misc", "", Submenus::Misc));
      sub->AddOption(option::SubmenuOption("tab/title/setting", "", Submenus::Settings));
      #ifndef NDEBUG
      sub->AddOption(option::SubmenuOption("Debug", "", Submenus::Debug));
      #endif
    });

    DebugTab();
    MiscTab();
    NetworkTab();
    SelfTab();
    SettingsTab();
    WorldTab();
    WeaponsTab();
    VehicleTab();
  } catch (std::exception &e) {
    LOG_ERROR("Caught exception {}", e.what());
  } catch (...) {
    LOG_ERROR("Caught unknown exception");
  }
}
}