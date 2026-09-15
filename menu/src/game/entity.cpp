//
// Created by X-ray on 15/09/2026.
//

#include "entity.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::game {
  bool Entity::Exists() const {
    return natives::ENTITY::DOES_ENTITY_EXIST(*this);
  }

  bool Entity::IsVehicle() const {
    return natives::ENTITY::IS_ENTITY_A_VEHICLE(*this);
  }

  rage::Vector3 Entity::GetCoords() const {
    return natives::ENTITY::GET_ENTITY_COORDS(*this, true);
  }

  void Entity::SetCoords(const rage::Vector3& position, const bool clearArea) const {
    if (!IsValid()) {
      return;
    }

    // The flags are the ones the teleports already passed: alive, then the dead and ragdoll ones off.
    natives::ENTITY::SET_ENTITY_COORDS(*this, position.x, position.y, position.z, 1, 0, 0, clearArea);
  }

  bool Entity::HasControl() const {
    return natives::NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(*this);
  }

  void Entity::RequestControl() const {
    natives::NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(*this);
  }
}
