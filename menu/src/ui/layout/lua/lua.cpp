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

    std::vector<lua::ScriptMetaData> GetValidScripts() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      return valid_scripts;
    }

    void SetCurrentLuaScript(const lua::ScriptMetaData& meta_data) {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      current_lua_script = meta_data;
    }

    lua::ScriptMetaData GetCurrentLuaScript() {
      common::concurrency::ScopedSpinlock lock(lua_scripts_lock);
      return current_lua_script;
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
      sub->AddComponent(components::LabelComponent("label/script_info"));
      const auto curr = GetCurrentLuaScript();
      sub->AddComponent(components::ExecuteComponent("label/script_name", curr.GetName(), []{}));
      sub->AddComponent(components::ExecuteComponent("label/script_main_file", curr.GetMainFile().filename().string(), []{}));
      sub->AddComponent(components::ExecuteComponent("label/script_desc", curr.GetDescription().value_or("label/script_no_desc"), []{}));

      const auto authors = curr.GetAuthor();
      if (!authors.empty()) {
        std::string authors_str = authors[0];
        for (size_t i = 1; i < authors.size(); ++i) {
          authors_str += ", " + authors[i];
        }
        sub->AddComponent(components::ExecuteComponent("label/script_authors", authors_str, []{}));
      }

      if (const auto version = curr.GetVersion()) {
        sub->AddComponent(components::ExecuteComponent("label/script_version", version.value(), []{}));
      }

      if (const auto repo = curr.GetRepository()) {
        sub->AddComponent(components::ExecuteComponent("label/script_repository", repo.value(), []{}));
      }

      if (const auto license = curr.GetLicense()) {
        sub->AddComponent(components::ExecuteComponent("label/script_license", license.value().filename().string(), []{}));
      }
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