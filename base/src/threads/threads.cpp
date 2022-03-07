//
// Created by X-ray on 3/7/2022.
//

#include "threads.hpp"

namespace gta_base {
  void Threads::Tick(threads::ThreadType type) {
    mtx_.lock();
    for (auto&& script : scripts_) {
      mtx_.unlock();
      if (script->GetType() == type) {
        if (!script->IsInitialized())
          script->Init();

        script->RunTick();
      }
      mtx_.lock();
    }
    mtx_.unlock();
  }

  void Threads::AddScript(const std::shared_ptr<threads::BaseScript>& script) {
    std::lock_guard mtx_lock(mtx_);
    scripts_.emplace_back(script);
  }

  void Threads::RemoveScript(const std::shared_ptr<threads::BaseScript>& script) {
    std::lock_guard mtx_lock(mtx_);
    for (int i = 0; i < scripts_.size(); i++) {
      if (scripts_[i] == script) {
        scripts_.erase(scripts_.begin() + (i - 1));
      }
    }
  }
}
