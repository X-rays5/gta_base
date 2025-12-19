//
// Created by X-ray on 04/08/2024.
//

#include "rich_presence.hpp"
#include <discord_rpc.h>

#include <base-common/util/time.hpp>

namespace base::menu::discord {
  namespace {
    void HandleReady(const DiscordUser* user) {
      LOG_INFO("Connected to discord as: {}", user->username);
    }

    void HandleDisconnect(int err_code, const char* message) {
      LOG_ERROR("Discord disconnected: {} - {}", err_code, message);
    }

    void HandleError(int err_code, const char* message) {
      LOG_ERROR("Discord error: {} - {}", err_code, message);
    }
  }

  RichPresence::RichPresence() {
    DiscordEventHandlers handlers = {};
    handlers.ready = HandleReady;
    handlers.disconnected = HandleDisconnect;
    handlers.errored = HandleError;

    Discord_Initialize(xorstr_("1003792099059183676"), &handlers, 1, nullptr);
    start_time_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    kRICH_PRESENCE = this;
  }

  RichPresence::~RichPresence() {
    kRICH_PRESENCE = nullptr;

    Discord_ClearPresence();
    Discord_Shutdown();
  }

  void RichPresence::Tick() {
    common::concurrency::ScopedSpinlock lock(lock_);
    if (common::util::time::GetTimeStamp() - last_tick_time_ < 5000) {
      return;
    }
    last_tick_time_ = common::util::time::GetTimeStamp();

    DiscordRichPresence discord_presence = {};
    discord_presence.state = activity_.state.c_str();
    discord_presence.details = activity_.details.c_str();
    discord_presence.startTimestamp = start_time_;
    discord_presence.largeImageKey = activity_.large_image_key.c_str();
    discord_presence.largeImageText = activity_.large_image_text.c_str();
    discord_presence.smallImageKey = activity_.small_image_key.c_str();
    discord_presence.smallImageText = activity_.small_image_text.c_str();
    discord_presence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
    discord_presence.partySize = activity_.party_size;
    discord_presence.partyMax = activity_.party_max;
    discord_presence.instance = 1;

    Discord_UpdatePresence(&discord_presence);
  }
}
