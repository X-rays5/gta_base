//
// Created by X-ray on 11/27/22.
//

#include "manager.hpp"

namespace gta_base::lua {
  Manager::Manager() {
    kMANAGER = this;
  }

  Manager::~Manager() {
    kMANAGER = nullptr;
  }

  std::optional<sol::state*> Manager::AddScript(const std::filesystem::path& path) {
    auto script_path = common::GetScriptsDir() / path;

    auto script = std::make_unique<Script>(script_path);
    running_scripts_.insert_or_assign(path, std::move(script));

    return running_scripts_[path]->GetState();
  }

  bool Manager::RemoveScript(const std::filesystem::path& path) {
    return running_scripts_.erase(path) > 0;
  }

  void Manager::RunScriptTick() {
    for (auto& [path, script] : running_scripts_)
      script->Tick();
  }
}