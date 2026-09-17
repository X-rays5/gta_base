//
// Created by X-ray on 15/09/2026.
//

#include "entity.hpp"

#include <minicoropp/coroutine.hpp>
#include "../../natives/natives_gen9.hpp"

namespace base::menu::game {
  bool Entity::Exists() const {
    return IsValid() && natives::ENTITY::DOES_ENTITY_EXIST(*this);
  }

  bool Entity::IsVehicle() const {
    return natives::ENTITY::IS_ENTITY_A_VEHICLE(*this);
  }

  rage::Vector3 Entity::GetCoords() const {
    return natives::ENTITY::GET_ENTITY_COORDS(*this, true);
  }

  rage::Vector3 Entity::GetOffsetCoords(const float offsetX, const float offsetY, const float offsetZ) const {
    return natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(*this, offsetX, offsetY, offsetZ);
  }

  ModelExtents Entity::GetModelExtents() const {
    // The out-parameters are safe to point at a Vector3: a vector out-parameter is the script's own
    // vector and not this object, so the invoker keeps the storage the game writes into and copies the
    // components back out of it (natives/invoker.hpp).
    ModelExtents extents{};
    natives::MISC::GET_MODEL_DIMENSIONS(natives::ENTITY::GET_ENTITY_MODEL(*this), &extents.minimum, &extents.maximum);
    return extents;
  }

  bool Entity::SetCoords(const rage::Vector3& position, const bool clearArea) const {
    if (!Exists()) {
      NOTIFY_ERR("label/teleport", "label/teleport/invalid_entity");
      return false;
    }

    if (IsVehicle() && !RequestControl()) {
      NOTIFY_WARN("label/teleport", "label/teleport/no_control");
    }

    natives::ENTITY::SET_ENTITY_COORDS(Get(), position.x, position.y, position.z, true, false, false, clearArea);
    if (IsVehicle())
      natives::VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Get(), 5.f);

    return true;
  }

  bool Entity::HasControl() const {
    return natives::NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(*this);
  }

  bool Entity::RequestControl() const {
    for (int i = 0; i < 10 && !HasControl(); ++i) {
      natives::NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(*this);
      minicoropp::this_coro::sleep_for(std::chrono::milliseconds(10));
    }

    return HasControl();
  }
}
