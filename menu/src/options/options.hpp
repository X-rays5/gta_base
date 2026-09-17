//
// Created by X-ray on 27/03/2026.
//

#pragma once

#include "opt/self/force_wanted_level.hpp"
#include "opt/self/godmode.hpp"
#include "opt/self/never_wanted.hpp"
#include "opt/self/self_heal.hpp"
#include "opt/self/self_max_armour.hpp"
#include "opt/self/self_remove_armour.hpp"
#include "opt/self/semi_godmode.hpp"
#include "opt/self/suicide.hpp"
#include "opt/self/teleport/teleport_into_last_vehicle.hpp"
#include "opt/self/teleport/teleport_to_last_vehicle.hpp"
#include "opt/self/teleport/teleport_to_objective.hpp"
#include "opt/self/teleport/teleport_to_waypoint.hpp"
#include "opt/vehicle/vehicle_repair.hpp"

namespace base::menu::options {
  struct AvailableOptions {
    AvailableOptions();

    std::shared_ptr<GodModeOption> god_mode_option;
    std::shared_ptr<SemiGodModeOpt> semi_god_mode_option;
    std::shared_ptr<SelfHealOption> self_heal_option;
    std::shared_ptr<SuicideOption> suicide_option;
    std::shared_ptr<SelfMaxArmourOption> self_max_armour_option;
    std::shared_ptr<SelfRemoveArmourOption> self_remove_armour_option;
    std::shared_ptr<ForceWantedLevelOption> force_wanted_level_option;
    std::shared_ptr<NeverWantedOption> never_wanted_option;
    std::shared_ptr<TeleportToObjective> teleport_to_objective_option;
    std::shared_ptr<TeleportToWaypoint> teleport_to_waypoint_option;
    std::shared_ptr<TeleportToLastVehicle> teleport_to_last_vehicle_option;
    std::shared_ptr<TeleportIntoLastVehicle> teleport_into_last_vehicle;
    std::shared_ptr<VehicleRepairOption> vehicle_repair_option;
  };
}