//
// Created by X-ray on 31/05/2025.
//

#include "script_manager.hpp"

namespace base::menu::scripts {
  std::size_t ScriptManager::AddScript(BaseScript* script) {
    const auto id = next_id_.fetch_add(1, std::memory_order_relaxed);
    const auto type = script->GetScriptType();
    if (scripts_.find(type) == scripts_.end()) {
      scripts_[type] = ankerl::unordered_dense::map<std::size_t, BaseScript*>();
    }
    scripts_[type][id] = script;

    return id;
  }

  void ScriptManager::RemoveScript(const std::size_t id, const BaseScript::Type type) {
    if (scripts_.find(type) != scripts_.end()) {
      scripts_[type].erase(id);
    }
  }

  void ScriptManager::InitScripts(BaseScript::Type type) {
    if (scripts_.find(type) != scripts_.end()) {
      for (auto&& script : scripts_[type] | std::views::values) {
        script->Init();
      }
    } else {
      LOG_WARN("No scripts of type {} found to initialize.", static_cast<int>(type));
    }
  }

  void ScriptManager::TickScripts(BaseScript::Type type) {
    if (scripts_.find(type) != scripts_.end()) {
      for (auto&& script : scripts_[type] | std::views::values) {
        script->Tick();
      }
    }
  }
}