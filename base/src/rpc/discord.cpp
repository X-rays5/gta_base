//
// Created by X-ray on 3/7/2022.
//

#include <chrono>
#include "discord_rpc.h"
#include "discord.hpp"
#include <xorstr.hpp>

namespace gta_base {
  namespace rpc {
    Discord::Discord() {
      DiscordEventHandlers handlers;
      std::memset(&handlers, 0, sizeof(handlers));
      Discord_Initialize(xorstr_("1003792099059183676"), &handlers, 1, nullptr);

      start_time_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

      kDISCORD = this;
    }

    Discord::~Discord() {
      kDISCORD = nullptr;

      Discord_ClearPresence();
      Discord_Shutdown();
    }

    void Discord::Tick() {
      DiscordRichPresence discord_rpc;
      std::memset(&discord_rpc, 0, sizeof(discord_rpc));

      discord_rpc.state = state_.c_str();
      discord_rpc.details = details_.c_str();
      discord_rpc.largeImageKey = large_image_key_.c_str();
      discord_rpc.largeImageText = large_image_text_.c_str();
      discord_rpc.smallImageKey = small_image_key_.c_str();
      discord_rpc.smallImageText = small_image_text_.c_str();
      discord_rpc.startTimestamp = start_time_;
      discord_rpc.instance = 1;

      Discord_UpdatePresence(&discord_rpc);
    }
  }
}
