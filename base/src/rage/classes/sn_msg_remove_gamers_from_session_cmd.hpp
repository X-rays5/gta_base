//
// Created by X-ray on 08/28/22.
//

#pragma once
#ifndef GTA_BASE_SN_MSG_REMOVE_GAMERS_FROM_SESSION_CMD_HPP
#define GTA_BASE_SN_MSG_REMOVE_GAMERS_FROM_SESSION_CMD_HPP
#include <cstdint>

namespace rage {
  class snMsgRemoveGamersFromSessionCmd {
  public:
      uint64_t session_id;
      __int64 peer_ids[32];
      int unk_0;
      int num_peers;
  };
  static_assert(sizeof(snMsgRemoveGamersFromSessionCmd) == 272);
}
#endif //GTA_BASE_SN_MSG_REMOVE_GAMERS_FROM_SESSION_CMD_HPP
