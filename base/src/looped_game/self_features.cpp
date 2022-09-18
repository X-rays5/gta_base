//
// Created by X-ray on 09/18/22.
//

#include "self_features.hpp"
#include "../misc/settings.hpp"
#include "../fiber/pool.hpp"
#include "../rage/util/get.hpp"
#include "../misc/globals.hpp"

namespace gta_base {
  namespace looped_game {
    inline void GodMode() {

    }

    inline void SemiGod() {
      fiber::kPOOL->AddJob([]{
        auto player = rage::GetLocalPed();
        player->m_health = ENTITY::GET_ENTITY_MAX_HEALTH(globals::local_player.ped_id);
      });
    }

    inline void NeverWanted() {
      if (auto player_info = rage::GetLocalPlayerInfo()) {
        player_info->m_is_wanted = false;
        player_info->m_wanted_level = 0;
        player_info->m_wanted_level_display = 0;
      }
    }

    void SelfFeatures() {
      if (globals::local_player.alive) {
        if (kSETTINGS.player.god_mode)
          GodMode();
        if (kSETTINGS.player.semi_god_mode)
          SemiGod();
      }
      if (kSETTINGS.player.never_wanted)
        NeverWanted();
    }
  }
}