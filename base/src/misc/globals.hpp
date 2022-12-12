//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_GLOBALS_HPP
#define GTABASE_GLOBALS_HPP
#include <atomic>
#include <robin_hood.h>
#include <unordered_map>
#undef AddJob
#include "../rage/types.hpp"
#include "../fiber/pool.hpp"
#include "../rage/data/data_types.hpp"

namespace gta_base {
  namespace common {
    struct KeyState {
      bool down = false;
      std::uint64_t released_at = 0;
    };

    enum class LocalPlayerEvent {
      kEnterVehicle,
      kLeaveVehicle,
      kRespawned,
      kDied,
    };

    struct LocalPlayer {
      Player id{};
      Ped ped_id{};
      bool alive = false;
      rage::scrVector player_pos{};
      Vehicle vehicle_id{};
      rage::scrVector vehicle_pos{};
      std::unordered_multimap<LocalPlayerEvent, std::function<void()>> event_handlers;

      void SendEvent(LocalPlayerEvent event) {
        auto entry = event_handlers.find(event);

        while (entry != event_handlers.end()) {
          auto handler = entry->second;
          fiber::kPOOL->AddJob([&handler]{
            std::invoke(handler);
          });
          entry++;
        }
      }
    };
  }

  namespace globals {
    constexpr static const auto name = "gta-base";
    constexpr static const auto version = "1.0.0";
    constexpr static const auto target_window_name = "";
    constexpr static const auto target_window_class = "grcWindow";
    constexpr static const auto compile_date = __DATE__;
    constexpr static const auto compile_time = __TIME__;
    constexpr static const auto lua_manifest_file_name = "manifest.lua";

    extern std::atomic<bool> running;
    extern HINSTANCE dll_handle;
    extern bool block_input;
    extern robin_hood::unordered_map<std::uint32_t, common::KeyState> key_state;
    extern std::atomic<std::int64_t> session_join_time;
    extern std::atomic<std::int64_t> session_leave_time;
    extern common::LocalPlayer local_player;
    extern rage::data::Data gta_data;
  }
}
#endif //GTABASE_GLOBALS_HPP
