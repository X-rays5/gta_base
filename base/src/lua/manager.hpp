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
#include "script_info.hpp"

namespace gta_base::lua {
  class Manager {
  public:
    Manager();
    ~Manager();

    std::optional<sol::state*> AddScriptRaw(const std::filesystem::path& path);
    std::optional<sol::state*> AddScript(const std::filesystem::path& path);
    bool RemoveScript(const std::filesystem::path& path);

    [[nodiscard]] bool IsScriptRunning(const std::filesystem::path& path) {
      std::unique_lock lock(mutex_);
      return running_scripts_.contains(path);
    }

    void RunScriptTick();

    /// Get all scripts in the scripts directory
    static std::vector<std::filesystem::path> GetScriptPaths();
    /// Get the manifests for all scripts in the scripts dir
    static std::vector<Manifest> GetScriptManifests();

  private:
    std::recursive_mutex mutex_;

    robin_hood::unordered_map<std::filesystem::path, ScriptInfo> running_scripts_;
  private:
  };

  inline Manager* kMANAGER{};
}
#endif //GTA_BASE_LUA_MANAGER_HPP
