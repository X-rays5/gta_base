//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_ENTITY_HPP
#define GTA_BASE_ENTITY_HPP
#include "../types.hpp"

namespace rage {
  namespace util {
    /// @note This should be called in a fiber job
    bool TakeControlOfEntity(Entity ent);
  }
}
#endif //GTA_BASE_ENTITY_HPP
