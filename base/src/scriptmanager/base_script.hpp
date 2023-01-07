//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_BASE_SCRIPT_HPP
#define GTABASE_BASE_SCRIPT_HPP
#include <queue>
#include "enums.hpp"

namespace gta_base::scriptmanager {
class BaseScript {
 public:
  virtual ~BaseScript() noexcept = default;
  BaseScript(BaseScript const &) = delete;
  BaseScript(BaseScript &&) = delete;
  BaseScript &operator=(BaseScript const &) = delete;
  BaseScript &operator=(BaseScript &&) = delete;

  virtual ScriptType GetType() = 0;
  virtual void RunTick() = 0;
  [[nodiscard]] bool IsInitialized() const {
    return initialized_;
  };
  virtual void Init() = 0;
 protected:
  bool initialized_ = false;

 protected:
  BaseScript() = default;
};
}
#endif //GTABASE_BASE_SCRIPT_HPP
