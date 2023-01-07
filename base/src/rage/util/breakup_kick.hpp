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

  auto network = GetNetwork();
  rage::snMsgRemoveGamersFromSessionCmd cmd{};
  cmd.m_session_id = network->m_game_session_ptr->m_rline_session.m_session_id;
  cmd.m_num_peers = 1;
  cmd.m_peer_ids[0] = target->SessionPeer()->m_peer_data.m_peer_id;

  kPOINTERS->HandleRemoveGamerCmd(network->m_game_session_ptr, target->SessionPlayer(), &cmd);
  for (auto &[id, plyr] : gta_base::kPLAYER_MGR->GetPlayerList()) {
    if (plyr->Id() != target->Id())
      LOG_DEBUG("remove {} to {}", target->Name(), plyr->Name());
    kPOINTERS->SendRemoveGamerCmd(network->m_game_session_ptr->m_net_connection_mgr,
                                  kPOINTERS->GetConnectionPeer(network->m_game_session_ptr->m_net_connection_mgr, plyr->SessionPlayer()->m_player_data.m_peer_id),
                                  network->m_game_session_ptr->m_connection_identifier, &cmd, 0x1000000);
  }
}
}
#endif //GTA_BASE_BREAKUP_KICK_HPP
