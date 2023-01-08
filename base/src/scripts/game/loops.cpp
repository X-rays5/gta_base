//
// Created by X-ray on 09/17/22.
//

#include "loops.hpp"
#include "../../looped_game/self_globals.hpp"
#include "../../looped_game/self_features.hpp"
#include "../../looped_game/weapon.hpp"
#include "../../looped_game/transition_state.hpp"

namespace gta_base::scripts {

  void Loops::Init() {}

  void Loops::RunTick() {
    looped_game::SetSelfGlobals();
    looped_game::SelfFeatures();
    looped_game::Weapons();
    looped_game::TransitionState();
  }
}