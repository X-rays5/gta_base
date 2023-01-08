//
// Created by X-ray on 11/27/22.
//

#include "manager.hpp"

namespace gta_base::lua {
  namespace {
    void LogScriptLoad(const Manifest& manifest) {
      std::string print = fmt::format("Loading script: {} ({})", manifest.GetName(), manifest.GetVersion());
      if (auto authors_opt = manifest.GetAuthors(); authors_opt.has_value()) {
        auto authors = std::move(*authors_opt);
        print += " by ";
        for (auto&& author: authors) {
          print = fmt::format("{}{}, ", print, author);
        }

        print.pop_back();
        print.pop_back();
      }

      LOG_INFO(print);
    }
  }

  Manager::Manager() {
    kMANAGER = this;
  }

  Manager::~Manager() {
    kMANAGER = nullptr;
  }

  std::optional<sol::state*> Manager::AddScriptRaw(const std::filesystem::path& path) {
    std::unique_lock lock(mutex_);

    if (running_scripts_.contains(path)) {
      LOG_ERROR("Script {} is already running", path.string());
      return std::nullopt;
    }

    ScriptInfo info;
    info.script_dir = path.parent_path();
    info.lua_script_inst = std::make_unique<Script>(info.script_dir, path);

    running_scripts_[path] = std::move(info);

    return running_scripts_[path].lua_script_inst->GetState();
  }

  std::optional<sol::state*> Manager::AddScript(const std::filesystem::path& path) {
    std::unique_lock lock(mutex_);

    std::filesystem::path script_dir;
    if (std::filesystem::is_directory(common::GetScriptsDir() / path)) {
      script_dir = common::GetScriptsDir() / path;
    } else {
      LOG_WARN("Loading script from outside of scripts directory: {}", path.string());
      script_dir = path;
    }

    if (!std::filesystem::is_directory(script_dir)) {
      LOG_ERROR("Script directory {} does not exist", script_dir.string());
      return std::nullopt;
    }

    if (running_scripts_.contains(script_dir)) {
      LOG_ERROR("Script {} is already running", script_dir.string());
      return std::nullopt;
    }

    Manifest manifest(script_dir);
    if (!manifest.Valid()) {
      LOG_ERROR("Failed to load manifest file: {}", (script_dir / globals::lua_manifest_file_name).string());
      return std::nullopt;
    }

    LogScriptLoad(manifest);

    ScriptInfo info;
    info.manifest = manifest;
    info.script_dir = script_dir;
    info.lua_script_inst = std::make_unique<Script>(info.script_dir, info.manifest.GetMainFile());

    running_scripts_[script_dir] = std::move(info);

    return running_scripts_[script_dir].lua_script_inst->GetState();
  }

  bool Manager::RemoveScript(const std::filesystem::path& path) {
    std::unique_lock lock(mutex_);

    return running_scripts_.erase(path) > 0;
  }

  void Manager::RunScriptTick() {
    std::unique_lock lock(mutex_);
    for (auto& [path, script]: running_scripts_)
      script.lua_script_inst->Tick();
  }

  std::vector<std::filesystem::path> Manager::GetScriptPaths() {
    std::filesystem::directory_iterator dir_iter(common::GetScriptsDir());
    std::vector<std::filesystem::path> paths;

    for (auto& entry: dir_iter) {
      if (entry.is_directory()) {
        if (std::filesystem::exists(entry.path() / globals::lua_manifest_file_name)) {
          paths.push_back(entry.path());
        } else {
          LOG_WARN("Found directory without manifest file: {}", entry.path().string());
        }
      }
    }

    return paths;
  }

  std::vector<Manifest> Manager::GetScriptManifests() {
    std::vector<Manifest> res;

    for (auto&& path: GetScriptPaths()) {
      Manifest manifest(path);
      if (!manifest.Valid()) {
        LOG_WARN("Failed to load manifest for: {}", path.string());
        continue;
      }

      res.emplace_back(std::move(manifest));
    }

    return res;
  }
}