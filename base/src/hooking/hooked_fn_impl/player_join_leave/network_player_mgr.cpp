//
// Created by X-ray on 09/08/22.
//

#include <network/CNetworkPlayerMgr.hpp>
#include "../../hooking.hpp"
#include "../../../player_mgr/manager.hpp"
#include "../../../ui/manager.hpp"
#include "../../../logger/logger.hpp"
#include "../../../misc/globals.hpp"


namespace gta_base {
  std::atomic<std::int64_t> globals::session_join_time = 0;
  std::atomic<std::int64_t> globals::session_leave_time = 1;
  void Hooks::NetworkPlayerMgrInit(CNetworkPlayerMgr* that, std::uint64_t a2, std::uint32_t a3, std::uint32_t a4[4]) {

    kHOOKING->network_player_mgr_init_hook_.GetOriginal<decltype(&Hooks::NetworkPlayerMgrInit)>()(that, a2, a3, a4);

    globals::session_join_time = common::GetEpoch();

    ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "","Entering session");
    LOG_INFO("NetworkPlayerMgrInit: Entering session");

    auto player = reinterpret_cast<::CNetGamePlayer*>(that->m_local_net_player);
    kPLAYER_MGR->AddPlayer(player);
    kPLAYER_MGR->SetSelf(player);
  }

  void Hooks::NetworkPlayerMgrShutdown(CNetworkPlayerMgr* that) {
    globals::session_leave_time = common::GetEpoch();

    ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "Leaving session");
    LOG_INFO("NetworkPlayerMgrShutdown: Leaving session");

    kPLAYER_MGR->Clear();

    kHOOKING->network_player_mgr_shutdown_hook_.GetOriginal<decltype(&Hooks::NetworkPlayerMgrShutdown)>()(that);
  }
}