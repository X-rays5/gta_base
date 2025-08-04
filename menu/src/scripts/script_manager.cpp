//
// Created by X-ray on 31/05/2025.
//

#include "script_manager.hpp"

namespace base::menu::scripts {
  ScriptManager::ScriptManager() {
    // Get all script types from BaseScript::Type enum and initialize the scripts_ map
    for (constexpr auto t = magic_enum::enum_values<BaseScript::Type>(); const auto& type : t) {
      script_locks_.emplace(type, std::make_unique<common::concurrency::Spinlock>());
      scripts_[type] = ankerl::unordered_dense::map<std::size_t, BaseScript*>();
    }

    kSCRIPTMANAGER = this;
  }

  ScriptManager::~ScriptManager() {
    kSCRIPTMANAGER = nullptr;
  }


  std::size_t ScriptManager::AddScript(BaseScript* script) {
    const auto id = next_id_.fetch_add(1, std::memory_order_relaxed);
    const auto type = script->GetScriptType();
    common::concurrency::ScopedSpinlock lock(*script_locks_[type]);
    scripts_[script->GetScriptType()][id] = script;

    LOG_DEBUG("Added script of type {} with ID {}", magic_enum::enum_name(type), id);
    return id;
  }

  void ScriptManager::RemoveScript(const std::size_t id, const BaseScript::Type type) {
    common::concurrency::ScopedSpinlock lock(*script_locks_[type]);
    if (scripts_.find(type) != scripts_.end()) {
      scripts_[type].erase(id);
      LOG_INFO("Removed script of type {} with ID {}", magic_enum::enum_name(type), id);
    }
  }

  void ScriptManager::InitScripts(const BaseScript::Type type) {
    common::concurrency::ScopedSpinlock lock(*script_locks_[type]);
    LOG_INFO("Initializing scripts of type {}", magic_enum::enum_name(type));
    if (scripts_.find(type) != scripts_.end()) {
      for (const auto script : scripts_[type] | std::views::values) {
        if (!script) {
          LOG_ERROR("Encountered an invalid script, skipping initialization.");
          continue;
        }

        try {
          script->ScriptInit();
        } catch (const std::exception& e) {
          LOG_ERROR("Failed to initialize a script of type {}: {}", magic_enum::enum_name(type), e.what());
        } catch (...) {
          LOG_ERROR("An unknown error occurred while initializing script of type {}", magic_enum::enum_name(type));
        }
      }
    } else {
      LOG_WARN("No scripts of type {} found to initialize.", magic_enum::enum_name(type));
    }

    LOG_INFO("Initialized scripts of type {}", magic_enum::enum_name(type));
  }

  void ScriptManager::TickScripts(const BaseScript::Type type) {
    common::concurrency::ScopedSpinlock lock(*script_locks_[type]);
    if (scripts_.find(type) != scripts_.end()) {
      for (const auto script : scripts_[type] | std::views::values) {
        if (!script) {
          LOG_ERROR("Encountered an invalid script, skipping tick.");
          continue;
        }

        try {
          script->ScriptTick();
        } catch (const std::exception& e) {
          LOG_ERROR("Failed to tick a script of type {}: {}", static_cast<int>(type), e.what());
        } catch (...) {
          LOG_ERROR("An unknown error occurred while ticking script of type {}", static_cast<int>(type));
        }
      }
    }
  }
}