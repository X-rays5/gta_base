//
// Created by X-ray on 20/12/2025.
//

#pragma once

#include <ankerl/unordered_dense.h>
#include "script_base.hpp"

namespace base::menu::script {
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

    std::uint64_t AddScript(std::unique_ptr<ScriptBase> script);
    void RemoveScript(std::uint64_t script_id);

    void TickScripts(ScriptBase::Type type);

  private:
    using script_entry_t = std::pair<std::uint64_t, std::unique_ptr<ScriptBase>>;
    using script_list_t = std::vector<script_entry_t>;

    std::atomic_uint64_t script_id_counter_{0};
    ankerl::unordered_dense::map<ScriptBase::Type, script_list_t> scripts;
  };
  inline ScriptManager* kSCRIPT_MANAGER{};
}