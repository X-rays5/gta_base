//
// Created by X-ray on 19/12/2025.
//

#pragma once

namespace base::rage {
  class Entity {
  public:
    Entity(const std::int32_t handle) :
      handle(handle) {}

    [[nodiscard]] std::int32_t GetHandle() const {
      return handle;
    }

    operator std::int32_t() const {
      return handle;
    }

  private:
    std::int32_t handle;
  };
}