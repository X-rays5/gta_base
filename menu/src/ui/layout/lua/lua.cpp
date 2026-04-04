//
// Created by X-ray on 04/04/2026.
//

#include "lua.hpp"
#include "../../menu_renderer.hpp"
#include "../../../lua/lua_util.hpp"
#include "../../../lua/lua_script.hpp"
#include "../../../lua/lua_script_metadata.hpp"
#include "../../components/components.hpp"

namespace base::menu::ui::layout {
  namespace {
    common::concurrency::Spinlock lua_scripts_lock;
    std::vector<std::filesystem::path> lua_scripts = lua::GetLuaScripts();
    std::vector<lua::ScriptMetaData> valid_scripts;
    lua::ScriptMetaData current_lua_script("");

    void RefreshLuaScripts() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      lua_scripts = lua::GetLuaScripts();

      // Read metadata for all scripts upfront and filter valid ones
      valid_scripts.clear();
      for (const auto& script : lua_scripts) {
        auto metadata = lua::ScriptMetaData(script);
        const auto status = metadata.ReadMetaData();
        if (status) {
          valid_scripts.push_back(metadata);
        } else {
          LOG_ERROR("Failed to read script meta data: {}", status.error());
        }
      }
    }

    std::vector<std::filesystem::path> GetLuaScripts() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      return lua_scripts;
    }

    std::vector<lua::ScriptMetaData> GetValidScripts() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      return valid_scripts;
    }
  }

  void InitSelectedScript() {
    Submenu selected_sub("", [](Submenu* sub) {
      sub->SetName(current_lua_script.GetName());
      sub->AddComponent(components::ExecuteComponent("ui/action/run_script", "", [] {
        lua::Script script(current_lua_script.GetPath().string());
        script.Start().wait();
        LOG_INFO("Finished run");
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kLUA_SCRIPT, std::move(selected_sub));
  }

  void InitLuaLayout() {
    RefreshLuaScripts();

    Submenu lua_submenu("ui/sub/lua", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/refresh", "", [] {
        RefreshLuaScripts();
      }));
      sub->AddComponent(components::LabelComponent("label/scripts"));
      auto scripts = GetValidScripts();
      if (scripts.empty()) {
        sub->AddComponent(components::LabelComponent("label/no_scripts_found"));
        return;
      }

      for (auto&& metadata : scripts) {
        auto comp = components::SubLinkComponent(SubmenuIDs::kLUA_SCRIPT, [metadata] {
          current_lua_script = metadata;
        });
        comp.SetName(metadata.GetName());

        sub->AddComponent(std::move(comp));
      }
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kLUA, std::move(lua_submenu));

    InitSelectedScript();
  }
}