//
// Created by X-ray on 09/18/22.
//

#include "self_features.hpp"
#include "../fiber/pool.hpp"
#include "../rage/util/get.hpp"

namespace gta_base::looped_game {
  inline void GodMode() {
    ENTITY::SET_ENTITY_CAN_BE_DAMAGED(globals::local_player.ped_id, !kSETTINGS.player.god_mode);
  }

  inline void SemiGod() {
    auto player = rage::GetLocalPed();
    player->m_health = ENTITY::GET_ENTITY_MAX_HEALTH(globals::local_player.ped_id);
  }

  inline void NeverWanted() {
    if (auto player_info = rage::GetLocalPlayerInfo()) {
      PLAYER::SET_MAX_WANTED_LEVEL(kSETTINGS.player.never_wanted ? 0 : 5);
      if (kSETTINGS.player.never_wanted)
        player_info->m_wanted_level = 0;
    }
  }

  void SelfFeatures() {
    if (globals::local_player.alive) {
      GodMode();
      if (kSETTINGS.player.semi_god_mode)
        SemiGod();
    }
    NeverWanted();
  }
}