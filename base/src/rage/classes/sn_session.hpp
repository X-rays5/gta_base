//
// Created by X-ray on 08/28/22.
//

#pragma once
#ifndef GTA_BASE_SN_SESSION_HPP
#define GTA_BASE_SN_SESSION_HPP
#include <cstdint>
#include "sn_msg_remove_gamers_from_session_cmd.hpp"

namespace rage {
  struct struct_with_snMsgRemoveGamersFromSessionCmd
  {
    char gap0[576];
    snMsgRemoveGamersFromSessionCmd *sn_msg_remove_gamers_from_session_cmd;
  };

  class snSession {
  public:
    uint8_t gap0[72];
    int64_t net_connection_mgr;
    uint8_t gap86[48];
    struct_with_snMsgRemoveGamersFromSessionCmd sn_msg_remove_gamers_from_session_cmd;
    uint8_t gap2C8[1656];
    int64_t host_player;
    uint8_t gap23[144];
    int64_t local_player;
    uint8_t gap3[6312];
    rlGamerHandle *peers[32];
    uint32_t peer_count;
    uint8_t gap238C[5148];
    int64_t player_list[32];
    uint16_t player_count;
    uint8_t gap2[1030];
    uint32_t dword3CB0;
    uint8_t byte3CB4;
    uint8_t gap3CB5[99];
    uint8_t byte3D18;
    uint32_t dword3D1C;
    int32_t connection_id;
    uint8_t byte3D24;
    uint32_t unsigned_int3D28;
  };
  static_assert(sizeof(snSession) == 15664);
}
#endif //GTA_BASE_SN_SESSION_HPP
