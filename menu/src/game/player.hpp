//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include "handles.hpp"
#include "ped.hpp"

namespace base::menu::game {
  /// A player - the slot, not the body in it. Everything about the player that is about their ped is on
  /// the ped, and a player who is not in the session has no ped to hand out.
  class Player : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;

    [[nodiscard]] Ped GetPed() const;
  };
  static_assert(sizeof(Player) == sizeof(ScrHandle));
}
