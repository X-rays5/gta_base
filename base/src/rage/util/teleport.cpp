//
// Created by X-ray on 09/18/22.
//

#include "teleport.hpp"
#include "../../fiber/pool.hpp"
#include "get.hpp"
#include "blip.hpp"
#include "streaming.hpp"
#include "../../ui/notification/notification.hpp"
#include "entity.hpp"

namespace rage::util {
    bool Teleport(rage::fvector3 coords, bool with_vehicle) {
      if (PED::IS_PED_IN_ANY_VEHICLE(gta_base::globals::local_player.ped_id, false) && with_vehicle) {
        auto veh = PED::GET_VEHICLE_PED_IS_IN(gta_base::globals::local_player.ped_id, false);
        if (!TakeControlOfEntity(veh)) {
          gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kInfo, "Teleport", "Failed to take control of vehicle. Teleporting might not work.");
        }
        ENTITY::SET_ENTITY_COORDS(veh, coords.x, coords.y, coords.z, 1, 0, 0, 1);
        VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.f);
        return true;
      } else if (GetLocalPed()) {
        ENTITY::SET_ENTITY_COORDS(gta_base::globals::local_player.ped_id, coords.x, coords.y, coords.z, 1, 0, 0, 0);
        return true;
      }
      gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kFail, "Teleport", "Call to Teleport has failed");
      return false;
    }

    bool TeleportIntoVehicle(Vehicle veh_id) {
      if (!ENTITY::IS_ENTITY_A_VEHICLE(veh_id)) {
        gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kFail, "TeleportIntoVehicle", "Supplied veh_id is not a vehicle");
        return false;
      }

      int seat_idx = 255;
      if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh_id, -1, true)) {
        seat_idx = -1;
      } else if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh_id, -2, true))
        seat_idx = -2;

      if (seat_idx == 255) {
        gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kFail, "TeleportIntoVehicle", "The target vehicle doesn't have any free seats");
        return false;
      }

      PED::SET_PED_INTO_VEHICLE(gta_base::globals::local_player.ped_id, veh_id, seat_idx);

      return true;
    }

    bool TeleportToBlip(int sprite, bool with_vehicle) {
      rage::scrVector location;

      if (!(location = GetBlipLocation(sprite))) {
        gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kFail, "TeleportToBlip", "Failed to get blip location");
      }

      return Teleport((fvector3)LoadGroundAtCoord(location), with_vehicle);
    }

    bool TeleportToWayPoint(bool with_vehicle) {
      auto coords = GetWaypointLocation();
      if (coords) {
        return Teleport((fvector3)LoadGroundAtCoord(coords), with_vehicle);
      }

      gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kFail, "TeleportToWayPoint", "Failed to get waypoint location");
      return false;
    }

    bool TeleportToObjective(bool with_vehicle) {
      auto coords = GetObjectiveLocation();
      if (coords) {
        return Teleport((fvector3)LoadGroundAtCoord(coords), with_vehicle);
      }

      gta_base::ui::kNOTIFICATIONS->Create(gta_base::ui::Notification::Type::kFail, "TeleportToObjective", "Failed to get objective location");
      return false;
    }
  }