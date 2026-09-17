//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include "handles.hpp"
#include <rage/vector.hpp>

namespace base::menu::game {
  /// A model's bounding box, in the frame of an entity built from it and measured from that entity's
  /// origin. See Entity::GetModelExtents.
  struct ModelExtents {
    rage::Vector3 minimum{};
    rage::Vector3 maximum{};
  };

  /**
   * An entity: the part of the game world that has a place in it.
   *
   * Every method here is a native call, so it belongs on the game thread - a caller anywhere else hands
   * the work to the game task executor rather than calling an entity itself.
   */
  class Entity : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;

    /// Whether the game still knows this handle. A handle that is valid but stale answers false, and
    /// every other call on one is a call about nothing.
    [[nodiscard]] bool Exists() const;
    [[nodiscard]] bool IsVehicle() const;
    [[nodiscard]] rage::Vector3 GetCoords() const;

    /// A point `offset` away from this entity, in the entity's own frame: x is left/right, y
    /// forward/backward, z up/down. This is how a caller arrives *near* an entity rather than on it,
    /// which is what a vehicle needs: a ped put down inside one is left standing in it, and the vehicle
    /// does not survive that.
    [[nodiscard]] rage::Vector3 GetOffsetCoords(float offsetX, float offsetY, float offsetZ) const;

    /// The bounding box of the model this entity is built from, in the entity's own frame.
    ///
    /// This is what a caller measures when it has to arrive *past* an entity rather than near it: the
    /// model decides how much room that takes, and a model's length is not known ahead of time. A
    /// vehicle's origin sits at the middle of it, so the distance from that origin back to the tail is
    /// one of the box's two Y corners.
    [[nodiscard]] ModelExtents GetModelExtents() const;

    /// Moves the entity. `clearArea` empties whatever was standing there, which is what a vehicle needs
    /// to land in its place instead of on top of it. A null handle is left alone: there is nothing to
    /// move, and the game treats a move of entity 0 as a request about another script's world.
    bool SetCoords(const rage::Vector3& position, bool clearArea = false) const;

    /// Whether the game lets us move this entity. An entity we do not control is one the game will
    /// ignore moves for - in a session that is anything the server has not handed over.
    [[nodiscard]] bool HasControl() const;

    /// Asks for that control. The answer does not arrive with the call: the server gives it on one of
    /// the following frames, so a caller that needs it *now* has to wait for HasControl() to turn true,
    /// which is a job for the game task executor rather than for a single method.
    bool RequestControl() const;
  };
  static_assert(sizeof(Entity) == sizeof(ScrHandle));

  /// An entity the game treats as an object. Nothing of its own yet - it is here because Ped and Vehicle
  /// are not the only things the database calls an entity.
  class Object : public Entity {
  public:
    using Entity::Entity;
  };
  static_assert(sizeof(Object) == sizeof(ScrHandle));
}
