//
// Created by X-ray on 09/08/22.
//

#include "../../hooking.hpp"
#include "../../../player_mgr/manager.hpp"
#include "../../../ui/manager.hpp"
#include "../../../logger/logger.hpp"
#include "../../../misc/globals.hpp"

namespace gta_base {
  void* Hooks::assign_physical_index(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, uint8_t new_index) {
    auto slot = player->m_player_id;

    auto res = kHOOKING->assign_physical_index_hook_.GetOriginal<decltype(&Hooks::assign_physical_index)>()(netPlayerMgr, player, new_index);

    // leaving
    if (new_index == 255) {
      kPLAYER_MGR->RemovePlayer(player);

      auto msg = fmt::format("name: {}\nslot: {}\nRID: {}", player->get_name(), slot, player->get_net_data()->m_gamer_handle.m_rockstar_id);
      ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Player leaving.", msg);
      LOG_INFO("{} is leaving. slot: {} RID: {}", player->get_name(), slot, player->get_net_data()->m_gamer_handle.m_rockstar_id);
    } else {
      kPLAYER_MGR->AddPlayer(player);

      // Prevent spam on new session join
      if (common::GetEpoch() - globals::session_join_time > 30000) {
        auto msg = fmt::format("name: {}\nslot: {}\nRID: {}", player->get_name(), new_index, player->get_net_data()->m_gamer_handle.m_rockstar_id);
        ui::kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Player Joining.", msg);
        LOG_INFO("{} is joining. slot: {} RID: {}", player->get_name(), new_index, player->get_net_data()->m_gamer_handle.m_rockstar_id);
      }
    }

    return res;
  }
}