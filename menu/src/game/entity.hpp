//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include "handles.hpp"
#include <rage/vector.hpp>

namespace base::menu::game {
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

    /// Moves the entity. `clearArea` empties whatever was standing there, which is what a vehicle needs
    /// to land in its place instead of on top of it. A null handle is left alone: there is nothing to
    /// move, and the game treats a move of entity 0 as a request about another script's world.
    void SetCoords(const rage::Vector3& position, bool clearArea = false) const;

    /// Whether the game lets us move this entity. An entity we do not control is one the game will
    /// ignore moves for - in a session that is anything the server has not handed over.
    [[nodiscard]] bool HasControl() const;

    /// Asks for that control. The answer does not arrive with the call: the server gives it on one of
    /// the following frames, so a caller that needs it *now* has to wait for HasControl() to turn true,
    /// which is a job for the game task executor rather than for a single method.
    void RequestControl() const;
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
