//
// Created by X-ray on 04/08/2024.
//

#include "rich_presence.hpp"
#include <discord-rpc.hpp>

#include <base-common/util/time.hpp>

namespace base::menu::discord {
  namespace {
    void HandleReady(const ::discord::User& user) {
      SetThreadDescription(GetCurrentThread(), L"DiscordRpcIOWorker");
      LOG_INFO("Connected to discord as: {}", user.global_name.has_value() ? user.global_name.value() : user.username);
    }

    void HandleDisconnect(int err_code, std::string_view message) {
      LOG_ERROR("Discord disconnected: {} - {}", err_code, message);
    }

    void HandleError(int err_code, std::string_view message) {
      LOG_ERROR("Discord error: {} - {}", err_code, message);
    }

    void TickLoop() {
      SetThreadDescription(GetCurrentThread(), L"RichPresenceTickLoop");

      LOG_INFO("RichPresenceTickLoop started");
      while (globals::kRUNNING) {
        if (kRICH_PRESENCE) {
          kRICH_PRESENCE->Tick();
        }
      }
    }
  }

  RichPresence::RichPresence() {
    auto& rpcManager = ::discord::RPCManager::get();
    rpcManager.setClientID(xorstr_("1003792099059183676"));
    rpcManager.onDisconnected(HandleDisconnect);
    rpcManager.onErrored(HandleError);
    rpcManager.onReady(HandleReady);
    rpcManager.initialize();

    start_time_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    tick_thread_ = std::thread(TickLoop);

    kRICH_PRESENCE = this;
  }

  RichPresence::~RichPresence() {
    if (tick_thread_.joinable())
      tick_thread_.join();

    kRICH_PRESENCE = nullptr;

    ::discord::RPCManager::get().shutdown();
  }

  void RichPresence::Tick() {
    if (common::util::time::GetTimeStamp() - last_tick_time_ < 5000) {
      return;
    }
    last_tick_time_ = common::util::time::GetTimeStamp();

    ::discord::Presence discord_presence = {};
    discord_presence.setActivityType(::discord::ActivityType::Game);
    discord_presence.setPartyPrivacy(::discord::PartyPrivacy::Private);
    discord_presence.setStatusDisplayType(::discord::StatusDisplayType::Name);
    discord_presence.setInstance(false);
    discord_presence.setPartyID("ae488379-351d-4a4f-ad32-2b9b01c91657");

    {
      common::concurrency::ScopedSpinlock lock(lock_);
      discord_presence.setState(activity_.state);
      discord_presence.setDetails(activity_.details);
      discord_presence.setStartTimestamp(start_time_);
      discord_presence.setLargeImageKey(activity_.large_image_key);
      discord_presence.setLargeImageText(activity_.large_image_text);
      discord_presence.setSmallImageKey(activity_.small_image_key);
      discord_presence.setSmallImageText(activity_.small_image_text);
      discord_presence.setPartySize(activity_.party_size);
      discord_presence.setPartyMax(activity_.party_max);
    }

    auto& rpcManager = ::discord::RPCManager::get();
    rpcManager.setPresence(discord_presence).refresh();
  }
}
