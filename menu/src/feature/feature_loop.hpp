//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "../script/script_base.hpp"

namespace base::menu::feature {
  class GameFeatureLoop : public script::ScriptBase {
  public:
    GameFeatureLoop();
    ~GameFeatureLoop() override = default;

    Type GetType() override {
      return Type::GameScript;
    }

    void OnInit() override;
    void OnTick() override;
  };
}