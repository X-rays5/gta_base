//
// Created by X-ray on 17/09/2026.
//

#include "teleport_to_last_vehicle.hpp"
#include <algorithm>
#include "../../../../game/streaming.hpp"

namespace base::menu::options {
  namespace {
    /// How far past the vehicle the player is put down, in metres, measured from the vehicle's own model
    /// and not from its origin. Arriving on the vehicle is not an arrival: a ped put down inside one loses
    /// it - and the last vehicle is one the player has just got out of, so it stands exactly where they
    /// left it, which is where a teleport to it lands. A fixed distance from the origin cannot clear every
    /// model: the same two metres that is past the tail of a compact is the middle of a sedan and well
    /// inside a limousine.
    constexpr float kCLEARANCE = 1.f;

    void ToLastVehicle() {
      const auto local_player = game::LocalPlayer::Get();
      if (const auto last_vehicle_result = local_player.GetLastVehicle(); last_vehicle_result.has_value()) {
        if (const auto last_vehicle = last_vehicle_result.value(); last_vehicle.Exists()) {
          const auto ped = local_player.GetPed();

          // Being in it is what the option asks for, and the only teleport left would be the vehicle
          // moved out from under the player to a point behind itself.
          if (ped.IsInAnyVehicle(false)) {
            if (const auto current = ped.GetVehicle(false); current.has_value() && current->Get() == last_vehicle.Get()) {
              return;
            }
          }

          // The model box is measured from the vehicle origin, so the distance from that origin back to
          // the tail is one of the box's two Y corners. Which one is the tail depends on which way the
          // model faces along its own Y axis, and the larger of the two clears the rear either way. The
          // offset frame puts y positive forward, so the tail is behind the vehicle at negative y.
          const auto extents = last_vehicle.GetModelExtents();
          const auto tail = std::max(-extents.minimum.y, extents.maximum.y);
          auto arrival = last_vehicle.GetOffsetCoords(0.f, -tail - kCLEARANCE, 0.f);
          arrival.z = game::streaming::GetZCoordsAtPoint(arrival);
          if (ped.Teleport(arrival, true))
            return;
        }
      }

      NOTIFY_ERR("label/teleport", "label/teleport/no_last_vehicle");
    }
  }

  void TeleportToLastVehicle::runCommand(std::shared_ptr<argparse::ArgumentParser>) {
    ToLastVehicle();
  }

  bool TeleportToLastVehicle::IsHotkeyAble() const {
    return true;
  }

  bool TeleportToLastVehicle::HandleHotkey() {
    ToLastVehicle();
    return true;
  }

  void TeleportToLastVehicle::CreateOptionUi(const std::string& label, ui::Submenu* sub) {
    GTA_BASE_OPTION_EXECUTE_UI_COMP(sub, label);
  }
}