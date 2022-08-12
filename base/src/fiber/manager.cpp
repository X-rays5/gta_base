//
// Created by X-ray on 08/12/22.
//

#include "manager.hpp"
#include "../rage/util/execasscript.hpp"

namespace gta_base {
  namespace fiber {
    Manager::Manager() {
      initialized_ = true;

      kMANAGER = this;
    }

    Manager::~Manager() {
      kMANAGER = nullptr;
    }

    void Manager::AddScript(script_t script) {
      std::unique_lock lock(mtx_);
      scripts_.emplace_back(std::move(script));
    }

    void Manager::RemoveAllScripts() {
      std::unique_lock lock(mtx_);
      scripts_.clear();
    }

    void Manager::RunTick() {
      rage::util::ExecuteAsScript(RAGE_JOAAT("main_persistent"), [this]{
        static bool ensure_main_fiber = (ConvertThreadToFiber(nullptr), true);

        std::lock_guard lock(mtx_);
        for (auto const& script : scripts_)
          script->Tick();
      });
    }

    scriptmanager::ScriptType Manager::GetType() {
      return scriptmanager::ScriptType::kGame;
    }

    void Manager::Init() {}
  }
}