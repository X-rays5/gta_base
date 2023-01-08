//
// Created by X-ray on 01/08/23.
//

#pragma once
#ifndef GTA_BASE_COMMAND_MANAGER_565B5EF63BD84BDF91F1B079B46176EE_HPP
#define GTA_BASE_COMMAND_MANAGER_565B5EF63BD84BDF91F1B079B46176EE_HPP
#include <robin_hood.h>
#include "base_commands/base_command.hpp"
#include "../pch.hpp"

namespace gta_base::commands {
  class CommandManager;

  inline CommandManager* kCOMMAND_MANAGER;

  class CommandManager {
  public:
    FORCE_INLINE CommandManager() {
      kCOMMAND_MANAGER = this;
    }

    FORCE_INLINE ~CommandManager() {
      kCOMMAND_MANAGER = nullptr;
    }

    FORCE_INLINE base_commands::Base* GetCommand(rage::joaat_t id) {
      std::unique_lock lock(mtx_);

      auto it = commands_.find(id);
      if (it != commands_.end()) {
        return it->second;
      }
      return nullptr;
    }

    FORCE_INLINE base_commands::Base* GetCommand(const std::string& id) {
      return GetCommand(rage::joaat(id));
    }

    template<template_str cmd_str>
    FORCE_INLINE base_commands::Base* GetCommand() {
      return GetCommand(RAGE_JOAAT(cmd_str));
    }

    FORCE_INLINE void RegisterCommand(base_commands::Base* command) {
      mtx_.lock();
      commands_.insert({command->GetId(), command});
      mtx_.unlock();
    }

  private:
    std::recursive_mutex mtx_;
    robin_hood::unordered_map<rage::joaat_t, base_commands::Base*> commands_;
  };
}
#endif //GTA_BASE_COMMAND_MANAGER_565B5EF63BD84BDF91F1B079B46176EE_HPP
