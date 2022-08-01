//
// Created by X-ray on 3/7/2022.
//

#include <chrono>
#include "discord_register.h"
#include "discord_rpc.h"
#include "discord.hpp"

#define DISCORD_APP_ID "example"

namespace gta_base {
  namespace rpc {
    Discord::Discord() {
      DiscordEventHandlers handlers;
      std::memset(&handlers, 0, sizeof(handlers));
      Discord_Initialize(DISCORD_APP_ID, &handlers, 1, "0");

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

      discord_rpc.state = state_;
      discord_rpc.details = details_;
      discord_rpc.largeImageKey = large_image_key_;
      discord_rpc.largeImageText = large_image_text_;
      discord_rpc.smallImageKey = small_image_key_;
      discord_rpc.smallImageText = small_image_text_;
      discord_rpc.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      discord_rpc.instance = 1;

      Discord_UpdatePresence(&discord_rpc);
    }
  }
}
