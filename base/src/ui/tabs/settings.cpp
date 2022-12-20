//
// Created by X-ray on 09/18/22.
//

#include "settings.hpp"
#include "tab_includes.hpp"
#include "../../lua/manager.hpp"
#include "../../settings/profile.hpp"

namespace gta_base::ui::tabs {
  namespace {
    std::uint64_t hotkey_delete_key_id = 0;

    std::size_t selected_lua_manifest{};
    std::vector<lua::Manifest> lua_manifests{};

    std::string active_translation;
    std::vector<std::filesystem::path> translation_paths{};
  }

  void SettingsTab() {
    lua_manifests = gta_base::lua::Manager::GetScriptManifests();

    active_translation = settings::profile::GetSelectedTranslation();
    translation_paths = TranslationManager::GetTranslationList();

    kMANAGER->AddSubmenu(Submenus::Settings, "tab/title/setting", [](Submenu* sub) {
      sub->AddOption(option::SubmenuOption("tab/title/lua_settings", "", Submenus::SettingsLua));
      sub->AddOption(option::SubmenuOption("tab/title/translation_settings", "", Submenus::SettingsTranslation));
      sub->AddOption(option::SubmenuOption("tab/title/hotkeys", "", Submenus::SettingsHotkeys));
      sub->AddOption(option::SubmenuOption("tab/title/theme", "", Submenus::SettingsTheme));
      sub->AddOption(option::SubmenuOption("tab/title/unload", "", Submenus::SettingsUnloadConfirm));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsLua, "tab/title/lua_settings", [](Submenu* sub){
      sub->AddOption(option::SubmenuOption("tab/title/lua_script_list", "", Submenus::LuaScriptList));
    });

    kMANAGER->AddSubmenu(Submenus::LuaScriptList, "tab/title/lua_script_list", [](Submenu* sub){
      sub->AddOption(option::ExecuteOption("option/refresh_lua_manifest", "", []{lua_manifests = gta_base::lua::Manager::GetScriptManifests();}));
      sub->AddOption(option::LabelOption("label/lua_scripts_list"));
      if (lua_manifests.empty()) {
        sub->AddOption(option::ExecuteOption("option/no_lua_scripts", "", nullptr, false));
        return;
      }

      for (std::size_t i = 0; i < lua_manifests.size(); i++) {
        if (lua_manifests[i].GetHiddenLib())
          continue;

        sub->AddOption(option::SubmenuOption(lua_manifests[i].GetName(), "", Submenus::LuaSelectedScript, [=]{selected_lua_manifest = i; kMANAGER->GetSubmenu(Submenus::LuaSelectedScript)->SetSelectedOption(0);}));
      }
    });

    kMANAGER->AddSubmenu(Submenus::LuaSelectedScript, lua_manifests[selected_lua_manifest].GetName(), [](Submenu* sub) {
      lua::Manifest& manifest = lua_manifests[selected_lua_manifest];

      sub->AddOption(option::ExecuteOption("option/load_lua_script", "", [&]{lua::kMANAGER->AddScript(manifest.GetScriptDir());}));
      sub->AddOption(option::ExecuteOption("option/unload_lua_script", "", [&]{lua::kMANAGER->RemoveScript(manifest.GetScriptDir());}));
      sub->AddOption(option::ExecuteOption("option/reload_lua_script", "", [&]{
        lua::kMANAGER->RemoveScript(manifest.GetScriptDir());
        lua::kMANAGER->AddScript(manifest.GetScriptDir());
      }));
      sub->AddOption(option::LabelOption("label/lua_script_info"));
      sub->AddOption(option::ExecuteOption("option/lua_script_name", "", nullptr, false))->SetRightTextKey(manifest.GetName());
      sub->AddOption(option::ExecuteOption("option/lua_script_version", "", nullptr, false))->SetRightTextKey(manifest.GetVersion());
      if (auto description = manifest.GetDescription(); description.has_value())
        sub->AddOption(option::ExecuteOption("option/lua_script_description", *description, nullptr, false));
      if (auto repo = manifest.GetRepository(); repo.has_value())
        sub->AddOption(option::ExecuteOption("option/lua_script_repository", *repo, nullptr, false));
      if (auto license = manifest.GetLicense(); license.has_value()) {
        auto license_file = manifest.GetLicenseFile();
        sub->AddOption(option::ExecuteOption("option/lua_script_license", license_file.has_value() ? *license_file : "", nullptr, false))->SetRightTextKey(*license);
      }
      if (auto authors = manifest.GetAuthors(); authors.has_value()) {
        sub->AddOption(option::LabelOption("label/lua_script_authors"));
        for (auto& author : *authors)
          sub->AddOption(option::ExecuteOption("option/lua_script_author", "", nullptr, false))->SetRightTextKey(author);
      }
    });

    kMANAGER->AddSubmenu(Submenus::SettingsTranslation, "tab/title/translation_settings", [](Submenu* sub){
      sub->AddOption(option::ExecuteOption("option/active_translation", "", nullptr))->SetRightTextKey(active_translation);
      sub->AddOption(option::ExecuteOption("option/refresh_translation_list", "", []{translation_paths = TranslationManager::GetTranslationList();}));
      sub->AddOption(option::LabelOption("label/translation_list"));
      if (translation_paths.empty()) {
        [[unlikely]]
        sub->AddOption(option::ExecuteOption("option/no_translations", "", nullptr, false));
        return;
      } else {
        [[likely]]
        for (auto&& path : translation_paths) {
          sub->AddOption(option::ExecuteOption(path.stem().string(), "", [&] {
            kTRANSLATION_MANAGER->SetActiveTranslation(std::move(std::make_shared<Translation>(path)));

            active_translation = path.stem().string();
            settings::profile::SetSelectedTranslation(active_translation);
          }, false));
        }
      }
    });

    kMANAGER->AddSubmenu(Submenus::SettingsHotkeys, "tab/title/hotkeys", [](Submenu* sub){
      for (auto&& hotkey : misc::kHOTKEY_MANAGER->GetAllHotkeys()) {
        sub->AddOption(option::SubmenuOption(hotkey.second, "", Submenus::HotkeysConfirmDelete, [=]{
          hotkey_delete_key_id = hotkey.first;
        }))->SetRightTextKey(common::VkToStr(hotkey.first));
      }
    });

    kMANAGER->AddSubmenu(Submenus::HotkeysConfirmDelete, ("tab/title/delete_hotkey"), [](Submenu* sub){
      sub->AddOption(option::ExecuteOption("confirm/yes", "", []{
        misc::kHOTKEY_MANAGER->RemoveHotkey(hotkey_delete_key_id);
        kMANAGER->PopSubmenu();
      }));
      sub->AddOption(option::ExecuteOption("confirm/no", "", []{
        kMANAGER->PopSubmenu();
      }));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsTheme, "tab/title/theme", [](Submenu* sub){
      sub->AddOption(option::NumberOption<float>("option/xpos", "option/xpos/desc", &kMANAGER->x_base, 0.005f, 0.f, 1.f - kMANAGER->x_size));
      sub->AddOption(option::NumberOption<float>("option/ypos", "option/ypos/desc", &kMANAGER->y_base, 0.005f, 0.f, 1.f));
      sub->AddOption(option::NumberOption<int>("option/max_options", "option/max_options/desc", &kMANAGER->max_drawn_options, 1, 1, 30));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsUnloadConfirm, "tab/title/unload", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("confirm/yes", "", [] {
        globals::running = false;
      }));
      sub->AddOption(option::ExecuteOption("confirm/no", "", [] {
        kMANAGER->PopSubmenu();
      }));
    });
  }
}