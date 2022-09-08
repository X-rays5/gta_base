//
// Created by X-ray on 08/28/22.
//

#pragma once
#ifndef GTA_BASE_BREAKUP_KICK_HPP
#define GTA_BASE_BREAKUP_KICK_HPP
#include "../../memory/pointers.hpp"
#include "../../player_mgr/manager.hpp"
#include "get.hpp"

namespace rage {
  void BreakupKick(std::shared_ptr<gta_base::player_mgr::Player> target) {
    using namespace gta_base::memory;

    snMsgRemoveGamersFromSessionCmd cmd{};
    auto network = GetNetwork();

    cmd.m_session_id = network->m_game_session_ptr->m_rline_session.m_session_id;
    cmd.m_num_peers = 1;
    cmd.m_peer_ids[0] = target->SessionPeer()->m_peer_data.m_peer_id;

    kPOINTERS->HandleRemoveGamerCmd(network->m_game_session_ptr, target->SessionPlayer(), &cmd);
    for (auto&& entry : gta_base::kPLAYER_MGR->GetPlayerList()) {
      auto player = entry.second;
      if (player->SessionPlayer()->m_player_data.m_gamer_handle.m_rockstar_id != target->SessionPlayer()->m_player_data.m_gamer_handle.m_rockstar_id && player->SessionPlayer()->m_player_data.m_gamer_handle.m_rockstar_id != gta_base::kPLAYER_MGR->GetSelf()->SessionPlayer()->m_player_data.m_gamer_handle.m_rockstar_id) {
        LOG_DEBUG("Sending command to let {} go to {}", target->Name(), player->Name());
        kPOINTERS->SendRemoveGamerCmd(network->m_game_session_ptr->m_net_connection_mgr, kPOINTERS->GetConnectionPeer(network->m_game_session_ptr->m_net_connection_mgr, player->SessionPlayer()->m_player_data.m_peer_id), network->m_game_session_ptr->m_connection_identifier, &cmd, 0x1000000);
      }
    }
  }
}
#endif //GTA_BASE_BREAKUP_KICK_HPP
