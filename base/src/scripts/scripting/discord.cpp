//
// Created by X-ray on 08/02/22.
//

#include "discord.hpp"
#include "../../rpc/discord.hpp"

namespace gta_base {
  namespace scripts {

    void Discord::Init() {
      if (rpc::kDISCORD) {
        rpc::kDISCORD->SetLargeImage("gta-logo");
        initialized_ = true;
      }
    }

    void Discord::RunTick() {
      // TODO: update discord state here

      rpc::kDISCORD->Tick();
    }
  }
}