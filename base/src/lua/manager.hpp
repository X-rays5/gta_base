//
// Created by X-ray on 11/27/22.
//

#pragma once
#ifndef GTA_BASE_LUA_MANAGER_HPP
#define GTA_BASE_LUA_MANAGER_HPP
#include <filesystem>
#include <memory>
#include <optional>
#include <mutex>
#include <robin_hood.h>
#include "script.hpp"

namespace gta_base::lua {
  class Manager {
  public:
    Manager();
    ~Manager();

    std::optional<sol::state*> AddScript(const std::filesystem::path& path);
    bool RemoveScript(const std::filesystem::path& path);

    void RunScriptTick();

  private:
    std::recursive_mutex mutex_;
    robin_hood::unordered_map<std::filesystem::path, std::unique_ptr<Script>> running_scripts_;

  };
  inline Manager* kMANAGER{};
}
#endif //GTA_BASE_LUA_MANAGER_HPP
