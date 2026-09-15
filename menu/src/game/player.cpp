//
// Created by X-ray on 15/09/2026.
//

#include "player.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::game {
  Ped Player::GetPed() const {
    return natives::PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(*this);
  }
}
