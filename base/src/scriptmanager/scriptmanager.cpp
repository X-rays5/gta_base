//
// Created by X-ray on 3/7/2022.
//

#include "scriptmanager.hpp"

namespace gta_base {
  ScriptManager::ScriptManager() {
    kSCRIPT_MANAGER = this;
  }

  ScriptManager::~ScriptManager() {
    kSCRIPT_MANAGER = nullptr;
  }

  void ScriptManager::Tick(scriptmanager::ScriptType type) {
    mtx_.lock();
    for (auto&& script : scripts_) {
      mtx_.unlock();
      if (script->GetType() == type) {
        if (!script->IsInitialized()) {
          script->Init();
        } else {
          script->RunTick();
        }
      }
      mtx_.lock();
    }
    mtx_.unlock();
  }

  void ScriptManager::AddScript(const std::shared_ptr<scriptmanager::BaseScript>& script) {
    std::lock_guard mtx_lock(mtx_);
    scripts_.emplace_back(script);
  }

  void ScriptManager::RemoveScript(const std::shared_ptr<scriptmanager::BaseScript>& script) {
    std::lock_guard mtx_lock(mtx_);
    for (int i = 0; i < scripts_.size(); i++) {
      if (scripts_[i] == script) {
        scripts_.erase(scripts_.begin() + (i - 1));
      }
    }
  }
}