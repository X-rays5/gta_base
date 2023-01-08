//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_THREADS_HPP
#define GTABASE_THREADS_HPP
#include <memory>
#include <vector>
#include <mutex>
#include "base_script.hpp"

namespace gta_base {
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

    void Tick(scriptmanager::ScriptType type);
    void AddScript(const std::shared_ptr<scriptmanager::BaseScript>& script);
    void RemoveScript(const std::shared_ptr<scriptmanager::BaseScript>& script);

    std::size_t Count() {
      mtx_.lock();
      auto script_count = scripts_.size();
      mtx_.unlock();
      return script_count;
    }

  private:
    std::mutex mtx_;
    std::vector<std::shared_ptr<scriptmanager::BaseScript>> scripts_;

  private:

  };

  inline ScriptManager* kSCRIPT_MANAGER{};
}

#endif //GTABASE_THREADS_HPP
