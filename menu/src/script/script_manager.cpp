//
// Created by X-ray on 20/12/2025.
//

#include "script_manager.hpp"
#include <magic_enum/magic_enum.hpp>

#include "game_task_executor.hpp"

namespace base::menu::script {
  ScriptManager::ScriptManager() {
    for (const auto type : magic_enum::enum_values<ScriptBase::Type>()) {
      scripts.emplace(type, script_list_t{});
    }

    kSCRIPT_MANAGER = this;

    AddScript(std::make_unique<GameTaskExecutor>());
  }

  ScriptManager::~ScriptManager() {
    kSCRIPT_MANAGER = nullptr;
  }

  void ScriptManager::TickScripts(const ScriptBase::Type type) {
    auto& script_list = scripts[type];
    for (auto&& script : script_list | std::views::values) {
      script->Tick();
    }
  }

  std::uint64_t ScriptManager::AddScript(std::unique_ptr<ScriptBase> script) {
    const auto script_id = script_id_counter_.fetch_add(1);
    const auto script_type = script->GetType();
    const auto script_name = script->GetName();

    scripts[script_type].emplace_back(std::make_pair(script_id, std::move(script)));

    LOG_INFO("Script added: {} (ID: {})", script_name, script_id);
    return script_id;
  }

  void ScriptManager::RemoveScript(std::uint64_t script_id) {
    LOG_INFO("Removing script with ID: {}", script_id);
    for (auto& script_list : scripts | std::views::values) {
      script_list.erase(std::ranges::remove_if(script_list,
                                               [script_id](const auto& pair) {
                                                 return pair.first == script_id;
                                               }).begin(),
                        script_list.end());
    }
  }
}
