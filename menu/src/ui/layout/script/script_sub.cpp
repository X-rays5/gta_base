//
// Created by X-ray on 11/09/2026.
//

#include "script_sub.hpp"

#include "../../menu_renderer.hpp"
#include "../../components/components.hpp"
#include "../../../as/script/as_script_manager.hpp"

#include <base-common/concurrency/spinlock.hpp>

#include <optional>
#include <string>
#include <vector>

namespace base::menu::ui::layout {
  namespace {
    using as::script::ScriptManager;
    using as::script::ScriptManifest;
    using as::script::ScriptState;

    // The scripts on disk and the one the selected submenu is showing. Both are read from the render
    // loop and written from the thread pool, which is where the load, refresh and sub-link handlers
    // run, so both are behind this lock.
    common::concurrency::Spinlock scripts_lock;
    std::vector<ScriptManifest> scripts;
    // Absent until a script has been picked, and when there is nothing on disk to pick.
    std::optional<ScriptManifest> current_script;

    void RefreshScripts() {
      common::concurrency::ScopedSpinlock lock(scripts_lock);
      scripts = ScriptManager::GetScriptsOnDisk();
    }

    std::vector<ScriptManifest> GetScripts() {
      common::concurrency::ScopedSpinlock lock(scripts_lock);
      return scripts;
    }

    void SetCurrentScript(const ScriptManifest& manifest) {
      common::concurrency::ScopedSpinlock lock(scripts_lock);
      current_script = manifest;
    }

    std::optional<ScriptManifest> GetCurrentScript() {
      common::concurrency::ScopedSpinlock lock(scripts_lock);
      return current_script;
    }

    /**
     * What the manager says `name` is doing right now. The state is asked for by name rather than
     * through a handle to the script, so that the menu never becomes the last owner of one.
     */
    ScriptState GetState(const std::string& name) {
      if (!as::script::kAS_SCRIPT_MANAGER) {
        return ScriptState::kNotLoaded;
      }

      return as::script::kAS_SCRIPT_MANAGER->GetScriptState(name);
    }

    /**
     * The localization key for `state`. The descriptions of these keys are the strings the player
     * reads, which is what lets a state be handed to a component as its key.
     */
    const char* GetStateKey(const ScriptState state) {
      switch (state) {
        case ScriptState::kNotLoaded:
          return "label/script_state/not_loaded";
        case ScriptState::kLoaded:
          return "label/script_state/loaded";
        case ScriptState::kRunning:
          return "label/script_state/running";
        case ScriptState::kSuspended:
          return "label/script_state/suspended";
      }

      return "label/script_state/not_loaded";
    }

    void UnloadScript(const std::string& name) {
      if (!as::script::kAS_SCRIPT_MANAGER) {
        LOG_ERROR("Cannot unload script '{}': the script manager is not initialized", name);
        return;
      }

      if (const auto status = as::script::kAS_SCRIPT_MANAGER->UnloadScript(name); status.has_error()) {
        LOG_ERROR("Failed to unload script '{}': {}", name, status.error());
        NOTIFY_ERR("ui/sub/as", "Failed to unload script '{}': {}", name, status.error());
      }
    }

    void LoadScript(const ScriptManifest& manifest) {
      if (!as::script::kAS_SCRIPT_MANAGER) {
        LOG_ERROR("Cannot load script '{}': the script manager is not initialized", manifest.GetName());
        return;
      }

      if (const auto status = as::script::kAS_SCRIPT_MANAGER->LoadScript(manifest); status.has_error()) {
        LOG_ERROR("Failed to load script '{}': {}", manifest.GetName(), status.error());
        NOTIFY_ERR("ui/sub/as", "Failed to load script '{}': {}", manifest.GetName(), status.error());
      }
    }

    void InitSelectedScript() {
      Submenu selected_sub("", [](Submenu* sub) {
        const auto manifest = GetCurrentScript();
        if (!manifest) {
          sub->AddComponent(components::LabelComponent("label/no_scripts_found"));
          return;
        }

        const std::string name = manifest->GetName();
        const ScriptState state = GetState(name);

        sub->SetName(name);

        if (state == ScriptState::kNotLoaded) {
          sub->AddComponent(components::ExecuteComponent("ui/action/load_script", "", [m = *manifest] {
            LoadScript(m);
          }));
        } else {
          sub->AddComponent(components::ExecuteComponent("ui/action/unload_script", "", [name] {
            UnloadScript(name);
          }));
        }

        sub->AddComponent(components::LabelComponent("label/script_info"));
        // Every value is passed as a key for the component to localize, which is what the Execute
        // components holding a plain value here do not need - see BaseComponent::GetDescription.
        sub->AddComponent(components::ExecuteComponent("label/script_state", GetStateKey(state), [] {}));
        sub->AddComponent(components::ExecuteComponent("label/script_name", name, [] {}));
        sub->AddComponent(components::ExecuteComponent("label/script_main_file", manifest->GetMainFile().filename().string(), [] {}));
        sub->AddComponent(components::ExecuteComponent("label/script_desc", manifest->GetDescription().value_or("label/script_no_desc"), [] {}));

        const auto authors = manifest->GetAuthor();
        if (!authors.empty()) {
          std::string authors_str = authors[0];
          for (size_t i = 1; i < authors.size(); ++i) {
            authors_str += ", " + authors[i];
          }
          sub->AddComponent(components::ExecuteComponent("label/script_authors", authors_str, [] {}));
        }

        if (const auto version = manifest->GetVersion()) {
          sub->AddComponent(components::ExecuteComponent("label/script_version", version.value(), [] {}));
        }

        if (const auto repo = manifest->GetRepository()) {
          sub->AddComponent(components::ExecuteComponent("label/script_repository", repo.value(), [] {}));
        }

        if (const auto license = manifest->GetLicense()) {
          sub->AddComponent(components::ExecuteComponent("label/script_license", license.value().filename().string(), [] {}));
        }
      });
      kMENU_RENDERER->AddSubmenu(SubmenuIDs::kAS_SCRIPT, std::move(selected_sub));
    }
  }

  void InitScriptLayout() {
    RefreshScripts();

    Submenu script_submenu("ui/sub/as", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/refresh", "", [] {
        RefreshScripts();
      }));
      sub->AddComponent(components::ExecuteComponent("ui/action/write_as_predefined", "ui/action/write_as_predefined/desc", [] {
        ScriptManager::WriteAsPredefined();
      }));
      sub->AddComponent(components::LabelComponent("label/scripts"));

      auto found = GetScripts();
      if (found.empty()) {
        sub->AddComponent(components::LabelComponent("label/no_scripts_found"));
        return;
      }

      for (auto&& manifest : found) {
        auto comp = components::SubLinkComponent(SubmenuIDs::kAS_SCRIPT, [manifest] {
          SetCurrentScript(manifest);
        });
        comp.SetName(manifest.GetName());
        // Shown when the entry is highlighted, so what a script is doing can be read without opening
        // it - and it is live, because the whole submenu is rebuilt every frame.
        comp.SetDescription(GetStateKey(GetState(manifest.GetName())));

        sub->AddComponent(std::move(comp));
      }
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kAS, std::move(script_submenu));

    InitSelectedScript();
  }
}
