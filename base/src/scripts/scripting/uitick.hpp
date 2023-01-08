//
// Created by X-ray on 08/02/22.
//

#pragma once
#ifndef GTA_BASE_UITICK_HPP
#define GTA_BASE_UITICK_HPP
#include "../../scriptmanager/base_script.hpp"

namespace gta_base::scripts {
  class UiTick : public scriptmanager::BaseScript {
  public:
    scriptmanager::ScriptType GetType() final {
      return scriptmanager::ScriptType::kRenderer;
    }

    void Init() final;
    void RunTick() final;
  };
}
#endif //GTA_BASE_UITICK_HPP
