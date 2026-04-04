//
// Created by X-ray on 04/04/2026.
//

#include "lua.hpp"
#include "../../menu_renderer.hpp"
#include "../../../lua/lua_util.hpp"
#include "../../../lua/script.hpp"
#include "../../../lua/script_metadata.hpp"
#include "../../components/components.hpp"

namespace base::menu::ui::layout {
  namespace {
    common::concurrency::Spinlock lua_scripts_lock;
    std::vector<std::filesystem::path> lua_scripts = lua::GetLuaScripts();
    lua::ScriptMetaData current_lua_script("");

    void RefreshLuaScripts() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      lua_scripts = lua::GetLuaScripts();
    }

    std::vector<std::filesystem::path> GetLuaScripts() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      return lua_scripts;
    }
  }

  void InitSelectedScript() {
    Submenu selected_sub("", [](Submenu* sub) {
      sub->SetName(current_lua_script.GetName());
      sub->AddComponent(components::ExecuteComponent("ui/action/run_script", "", [] {
        lua::Script script(current_lua_script.GetPath().string());
        script.Start();
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
      auto scripts = GetLuaScripts();
      if (scripts.empty()) {
        sub->AddComponent(components::LabelComponent("label/no_scripts_found"));
        return;
      }

      for (auto&& script : scripts) {
        sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kLUA_SCRIPT, [script] {
          auto tmp = lua::ScriptMetaData(script);
          const auto status = tmp.ReadMetaData();
          if (!status) {
            LOG_ERROR("Failed to read script meta data: {}", status.error());
          }
          current_lua_script = tmp;
        }));
      }
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kLUA, std::move(lua_submenu));

    InitSelectedScript();
  }
}