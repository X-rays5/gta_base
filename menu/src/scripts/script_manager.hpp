//
// Created by X-ray on 31/05/2025.
//

#ifndef SCRIPTMANAGER_HPP_05162518
#define SCRIPTMANAGER_HPP_05162518
#include <memory>
#include <ankerl/unordered_dense.h>
#include <base-common/concurrency/spinlock.hpp>
#include "base_script.hpp"

namespace base::menu::scripts {
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

    std::size_t AddScript(BaseScript* script);
    void RemoveScript(std::size_t id, BaseScript::Type type);

    void InitScripts(BaseScript::Type type);
    void TickScripts(BaseScript::Type type);

  private:
    ankerl::unordered_dense::map<BaseScript::Type, std::unique_ptr<common::concurrency::Spinlock>> script_locks_;
    ankerl::unordered_dense::map<BaseScript::Type, ankerl::unordered_dense::map<std::size_t, BaseScript*>> scripts_;
    std::atomic<std::size_t> next_id_;
  };

  inline ScriptManager* kSCRIPTMANAGER{};
}

#endif //SCRIPTMANAGER_HPP_05162518
