//
// Created by X-ray on 16/09/2026.
//

#include "debug.hpp"

#ifndef NDEBUG

#include <string>
#include "../../../natives/natives_gen9.hpp"
#include "../../../script/game_task_executor.hpp"
#include "../../components/components.hpp"
#include "../../notification/manager.hpp"
#include "../../menu_renderer.hpp"

namespace base::menu::ui::layout {
  namespace {
    /// One probe, reported as it comes back. The message is the bare `{}` and the detail is handed to it
    /// already built, because NOTIFY_INFO puts its message through both the localizer and fmt: this way
    /// neither has anything to do, and the detail can never be read as a format string.
    void Report(const std::string& detail) {
      NOTIFY_INFO("Invoker", "{}", detail);
    }

    /// The invoker's scalar path, so a native call that is broken outright can be told apart from one
    /// whose handle conversion is broken.
    void ProbeScalar() {
      const auto player = natives::PLAYER::PLAYER_ID();
      const auto level = natives::PLAYER::GET_PLAYER_WANTED_LEVEL(player);

      Report(fmt::format("player {} wanted level -> {}", player, level));
    }

    /// A handle as an argument. The parameter is an Entity, so the Ped is upcast on the way in.
    void ProbeHandleArgument() {
      const auto ped = natives::PLAYER::PLAYER_PED_ID();

      Report(fmt::format("DOES_ENTITY_EXIST(ped {}) -> {}", ped, natives::ENTITY::DOES_ENTITY_EXIST(ped)));
    }

    /// A handle as a return value: the object has to be built from the identity the game left behind.
    void ProbeHandleReturn() {
      const auto ped = natives::PLAYER::PLAYER_PED_ID();

      Report(fmt::format("PLAYER_PED_ID -> {} valid {} exists {}", ped, ped.IsValid(), natives::ENTITY::DOES_ENTITY_EXIST(ped)));
    }

    /// The same ped from two natives, one of which takes a Player.
    void ProbeReturnCrossCheck() {
      const auto direct = natives::PLAYER::PLAYER_PED_ID();
      const auto fromPlayer = natives::PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(natives::PLAYER::PLAYER_ID());

      Report(fmt::format("PLAYER_PED_ID {} vs GET_PLAYER_PED_SCRIPT_INDEX {} -> {}",
                         direct, fromPlayer, direct.Get() == fromPlayer.Get() ? "equal" : "DIFFERENT"));
    }

    /// A Vehicle where the native asks for an Entity - a subclass taken as its base.
    void ProbeDerivedAsBase() {
      const auto vehicle = natives::PED::GET_VEHICLE_PED_IS_IN(natives::PLAYER::PLAYER_PED_ID(), false);
      if (!vehicle.IsValid()) {
        Report("not in a vehicle");
        return;
      }

      Report(fmt::format("vehicle {} as Entity -> exists {}", vehicle, natives::ENTITY::DOES_ENTITY_EXIST(vehicle)));
    }

    /// A handle the game writes through a pointer. The target starts invalid on purpose: an identity that
    /// comes back non-zero is the game having written it, which is the whole of what the slot is for.
    void ProbeOutPointer() {
      const auto vehicle = natives::PED::GET_VEHICLE_PED_IS_IN(natives::PLAYER::PLAYER_PED_ID(), false);
      if (!vehicle.IsValid()) {
        Report("not in a vehicle");
        return;
      }

      natives::Vehicle trailer{};
      const auto has_trailer = natives::VEHICLE::GET_VEHICLE_TRAILER_VEHICLE(vehicle, &trailer);

      Report(fmt::format("GET_VEHICLE_TRAILER_VEHICLE(vehicle {}) -> {} trailer {} exists {}",
                         vehicle, has_trailer, trailer, trailer.IsValid() && natives::ENTITY::DOES_ENTITY_EXIST(trailer)));
    }

    /// The null branch: GET_ENTITY_SCRIPT documents its second parameter as unused.
    void ProbeOutPointerNull() {
      const auto ped = natives::PLAYER::PLAYER_PED_ID();
      const auto* script = natives::ENTITY::GET_ENTITY_SCRIPT(ped, nullptr);

      Report(fmt::format("GET_ENTITY_SCRIPT(ped {}, null) -> {}", ped, script ? script : "none"));
    }

    /// A second out signature, ten parameters wide, where nothing is found more often than not - which is
    /// the interesting half: the target has to come back as it went in.
    void ProbeOutPointerClosestPed() {
      const auto ped = natives::PLAYER::PLAYER_PED_ID();
      const auto coords = natives::ENTITY::GET_ENTITY_COORDS(ped, true);

      natives::Ped found{};
      const auto got_one = natives::PED::GET_CLOSEST_PED(coords.x, coords.y, coords.z, 25.f, true, false, &found, true, true, -1);

      Report(fmt::format("GET_CLOSEST_PED at ({:.1f}, {:.1f}, {:.1f}) -> {} found {} exists {}",
                         coords.x, coords.y, coords.z, got_one, found,
                         found.IsValid() && natives::ENTITY::DOES_ENTITY_EXIST(found)));
    }

    /// Natives are only callable from the game's own thread, so every row hands its probe to the executor
    /// and the menu row returns without waiting for it.
    template <typename Probe>
    void AddProbe(Submenu* sub, const std::string& label, const std::string& description, const Probe probe) {
      sub->AddComponent(components::ExecuteComponent(label, description, [probe] {
        script::kGAME_TASK_EXECUTOR->QueueTask(probe);
      }));
    }

    void BuildInvokerSub() {
      Submenu invoker_submenu("ui/sub/debug_invoker", [](Submenu* sub) {
        AddProbe(sub, "Baseline scalar", "GET_PLAYER_WANTED_LEVEL on the local player: a native call that owes the handle conversion nothing.", ProbeScalar);
        AddProbe(sub, "Handle argument", "A Ped handed to a native asking for an Entity.", ProbeHandleArgument);
        AddProbe(sub, "Handle return", "PLAYER_PED_ID, then the object it produced asked whether it exists.", ProbeHandleReturn);
        AddProbe(sub, "Return cross-check", "The same ped out of two natives, one of which takes a Player.", ProbeReturnCrossCheck);
        AddProbe(sub, "Derived as base", "A Vehicle from GET_VEHICLE_PED_IS_IN passed to an Entity parameter.", ProbeDerivedAsBase);
        AddProbe(sub, "Out pointer", "GET_VEHICLE_TRAILER_VEHICLE writing a handle through a pointer, seeded invalid.", ProbeOutPointer);
        AddProbe(sub, "Out pointer, null", "GET_ENTITY_SCRIPT with the pointer the database documents as unused.", ProbeOutPointerNull);
        AddProbe(sub, "Out pointer, closest ped", "GET_CLOSEST_PED writing a ped through a pointer, seeded invalid.", ProbeOutPointerClosestPed);
      });

      kMENU_RENDERER->AddSubmenu(SubmenuIDs::kDEBUG_INVOKER, std::move(invoker_submenu));
    }
  }

  void InitDebugLayout() {
    BuildInvokerSub();

    Submenu debug_submenu("ui/sub/debug", [](Submenu* sub) {
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kDEBUG_INVOKER));
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kDEBUG, std::move(debug_submenu));
  }
}

#endif
