//
// Created by X-ray on 08/02/22.
//

#include "discord.hpp"
#include "../../rpc/discord.hpp"
#include "../../misc/common.hpp"
#include "../../rage/util/get.hpp"

namespace gta_base {
  namespace scripts {

    void Discord::Init() {
      if (rpc::kDISCORD) {
        rpc::kDISCORD->SetLargeImage("gta-logo");
        initialized_ = true;
      }
    }

    void Discord::RunTick() {
      if (std::int64_t time_now = common::GetEpoch(); time_now - last_update_ > 5000) {
        if (common::IsSessionStarted()) {
          auto player_mgr = rage::GetNetworkPlayerMgr();
          rpc::kDISCORD->SetStatus(fmt::format("Playing online ({} of {})", player_mgr->m_player_count, player_mgr->m_player_limit));
        } else {
          rpc::kDISCORD->SetStatus("Playing offline");
        }

        last_update_ = time_now;
      }

      rpc::kDISCORD->Tick();
    }
  }
}