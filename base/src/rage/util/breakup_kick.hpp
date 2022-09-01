//
// Created by X-ray on 08/28/22.
//

#pragma once
#ifndef GTA_BASE_BREAKUP_KICK_HPP
#define GTA_BASE_BREAKUP_KICK_HPP
#include "../../memory/pointers.hpp"

namespace rage {
  void BreakupKick(std::uint32_t player_idx) {
    snMsgRemoveGamersFromSessionCmd cmd{};
    cmd.session_id = 0;
    cmd.num_peers = 1;
    cmd.peer_ids[0] =
  }
}
#endif //GTA_BASE_BREAKUP_KICK_HPP
