//
// Created by X-ray on 09/17/22.
//

#include "loops.hpp"
#include "../../looped_game/self_globals.hpp"
#include "../../looped_game/self_features.hpp"

namespace gta_base {
  namespace scripts {

    void Loops::Init() {}

    void Loops::RunTick() {
      looped_game::SetSelfGlobals();
      looped_game::SelfFeatures();
    }
  }
}