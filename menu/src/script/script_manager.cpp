//
// Created by X-ray on 20/12/2025.
//

#include "script_manager.hpp"
#include <enchantum/enchantum.hpp>

#include "game_task_executor.hpp"
#include "general_task_executor.hpp"
#include "../feature/feature_loop.hpp"

namespace base::menu::script {
  ScriptManager::ScriptManager() {
    for (const auto type : enchantum::values_generator<ScriptBase::Type>) {
      scripts.emplace(type, script_list_t{});
    }

    kSCRIPT_MANAGER = this;

    AddScript(std::make_unique<GameTaskExecutor>());
    AddScript(std::make_unique<feature::GameFeatureLoop>());

    // The general hosts go in before the thread starts: it ticks them, and TickScripts reads the list
    // without a lock, so nothing of this type may be added afterwards.
    AddScript(std::make_unique<GeneralTaskExecutor>(&general_signal_));
    general_thread_ = std::thread([this] {
      GeneralLoop();
    });
  }

  ScriptManager::~ScriptManager() {
    // The thread ticks the hosts, so it has to be gone before they are destroyed and before anything
    // else here is torn down. Notifying wakes it out of an indefinite park if it is idle.
    general_stopping_ = true;
    general_signal_.Notify();
    if (general_thread_.joinable()) {
      general_thread_.join();
    }

    kSCRIPT_MANAGER = nullptr;
  }

  void ScriptManager::GeneralLoop() {
    while (!general_stopping_) {
      TickScripts(ScriptBase::Type::General);

      const bool has_queued_tasks = kGENERAL_TASK_EXECUTOR && kGENERAL_TASK_EXECUTOR->HasPendingTasks();
      const bool has_other_hosts = GetScriptCount(ScriptBase::Type::General) > 1;
      if (has_queued_tasks || has_other_hosts) {
        // Work left over - a task that suspended itself is still queued - so come back for it.
        general_signal_.Wait(kGENERAL_TICK_MS);
      } else {
        // Nothing at all to do: park until a task is queued. A task queued between this check and
        // the wait notifies the signal, which is already set by then, so the wait returns at once
        // instead of missing it.
        general_signal_.Wait();
      }
    }
  }

  void ScriptManager::TickScripts(const ScriptBase::Type type) {
    auto& script_list = scripts[type];
    for (auto&& script : script_list | std::views::values) {
      script->Tick();
    }
  }

  std::size_t ScriptManager::GetScriptCount(const ScriptBase::Type type) const {
    const auto it = scripts.find(type);
    return it == scripts.end() ? 0 : it->second.size();
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
