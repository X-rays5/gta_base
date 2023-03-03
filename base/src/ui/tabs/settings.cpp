//
// Created by X-ray on 09/18/22.
//

#include "settings.hpp"
#include "tab_includes.hpp"
#include "../../lua/manager.hpp"
#include "../../settings/profile.hpp"

/*
namespace gta_base::ui::tabs {
  namespace {
    std::uint64_t hotkey_delete_key_id = 0;

    std::size_t selected_lua_manifest{};
    std::vector<lua::Manifest> lua_manifests{};

    std::string active_translation;
    std::filesystem::path selected_translation;
    std::vector<std::filesystem::path> translation_paths{};

    bool hotkey_save_on_exit = false;
    bool hotkey_save_on_change = false;
    std::string active_hotkey_profile;
    std::vector<std::filesystem::path> hotkey_profile_paths{};
  }

  void SettingsTab() {
    lua_manifests = gta_base::lua::Manager::GetScriptManifests();

    active_translation = settings::profile::GetSelectedTranslation();
    translation_paths = TranslationManager::GetTranslationList();

    hotkey_save_on_exit = misc::kHOTKEY_MANAGER->GetSaveOnExit();
    hotkey_save_on_change = misc::kHOTKEY_MANAGER->GetSaveOnChange();
    active_hotkey_profile = settings::profile::GetSelectedHotkeyProfile();
    hotkey_profile_paths = misc::HotkeyManager::GetHotkeyProfileList();

    kMANAGER->AddSubmenu(Submenus::Settings, "tab/title/setting", [](Submenu* sub) {
      sub->AddOption(option::SubmenuOption("tab/title/lua_settings", "", Submenus::SettingsLua));
      sub->AddOption(option::SubmenuOption("tab/title/translation_settings", "", Submenus::SettingsTranslation));
      sub->AddOption(option::SubmenuOption("tab/title/hotkeys", "", Submenus::SettingsHotkeys));
      sub->AddOption(option::SubmenuOption("tab/title/theme", "", Submenus::SettingsTheme));
      sub->AddOption(option::SubmenuOption("tab/title/unload", "", Submenus::SettingsUnloadConfirm));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsLua, "tab/title/lua_settings", [](Submenu* sub) {
      sub->AddOption(option::SubmenuOption("tab/title/lua_script_list", "", Submenus::LuaScriptList));
    });

    kMANAGER->AddSubmenu(Submenus::LuaScriptList, "tab/title/lua_script_list", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("option/refresh_lua_manifest", "", [] { lua_manifests = gta_base::lua::Manager::GetScriptManifests(); }, false));
      sub->AddOption(option::LabelOption("label/lua_scripts_list"));
      if (lua_manifests.empty()) {
        sub->AddOption(option::ExecuteOption("option/no_lua_scripts", "", nullptr, false));
        return;
      }

      for (std::size_t i = 0; i < lua_manifests.size(); i++) {
        if (lua_manifests[i].GetHiddenLib())
          continue;

        sub->AddOption(option::SubmenuOption(lua_manifests[i].GetName(), "", Submenus::LuaSelectedScript, [=] {
          selected_lua_manifest = i;
          kMANAGER->GetSubmenu(Submenus::LuaSelectedScript)->SetSelectedOption(0);
        }));
      }
    });

    kMANAGER->AddSubmenu(Submenus::LuaSelectedScript, lua_manifests[selected_lua_manifest].GetName(), [](Submenu* sub) {
      lua::Manifest& manifest = lua_manifests[selected_lua_manifest];

      sub->AddOption(option::ExecuteOption("option/load_lua_script", "", [&] {
        misc::kTHREAD_POOL->AddJob([=] {
          lua::kMANAGER->AddScript(manifest.GetScriptDir());
        });
      }, false));
      sub->AddOption(option::ExecuteOption("option/unload_lua_script", "", [&] {
        misc::kTHREAD_POOL->AddJob([=] {
          lua::kMANAGER->RemoveScript(manifest.GetScriptDir());
        });
      }, false));
      sub->AddOption(option::ExecuteOption("option/reload_lua_script", "", [&] {
        misc::kTHREAD_POOL->AddJob([=] {
          lua::kMANAGER->RemoveScript(manifest.GetScriptDir());
          lua::kMANAGER->AddScript(manifest.GetScriptDir());
        });
      }, false));
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

    kMANAGER->AddSubmenu(Submenus::SettingsTranslation, "tab/title/translation_settings", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("option/create_new_translation", "", [] {
        ui::Translation(ui::translation::default_translation).SaveToFile(common::GetTranslationDir() / fmt::format("default_{}.json", common::GetEpoch()));
        translation_paths = TranslationManager::GetTranslationList();
      }));
      sub->AddOption(option::ExecuteOption("option/active_translation", "", nullptr))->SetRightTextKey(active_translation);
      sub->AddOption(option::ExecuteOption("option/refresh_translation_list", "", [] { translation_paths = TranslationManager::GetTranslationList(); }));
      sub->AddOption(option::LabelOption("label/translation_list"));
      if (translation_paths.empty()) {
        [[unlikely]]
        sub->AddOption(option::ExecuteOption("option/no_translations", "", nullptr, false));
        TranslationManager::CreateDefaultProfile();
        translation_paths = TranslationManager::GetTranslationList();
        return;
      } else {
        [[likely]]
        for (auto&& path : translation_paths) {
          sub->AddOption(option::SubmenuOption(path.stem().string(), "", Submenus::TranslationSelected, [&] {
            selected_translation = path;
            kMANAGER->GetSubmenu(Submenus::TranslationSelected)->SetNameKey(path.stem().string());
          }));
        }
      }
    });

    kMANAGER->AddSubmenu(Submenus::TranslationSelected, "", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("option/translation_set_active", "", [&] {
        active_translation = selected_translation.stem().string();
        misc::kTHREAD_POOL->AddJob([=] {
          kTRANSLATION_MANAGER->SetActiveTranslation(std::move(std::make_shared<Translation>(selected_translation)));
          settings::profile::SetSelectedTranslation(active_translation);
        });
      }, false));
      sub->AddOption(option::SubmenuOption("tab/title/delete_translation", "", Submenus::TranslationConfirmDelete));
    });

    kMANAGER->AddSubmenu(Submenus::TranslationConfirmDelete, "tab/title/delete_translation", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("option/translation_delete", "", [&] {
        std::filesystem::remove(selected_translation);
        translation_paths = TranslationManager::GetTranslationList();
        active_translation = "txt/deleted_translation"_T;
        settings::profile::SetSelectedTranslation("default");
        if (translation_paths.empty()) {
          TranslationManager::CreateDefaultProfile();
          active_translation = "default";
        }
        translation_paths = TranslationManager::GetTranslationList();
        kMANAGER->PopSubmenu();
        kMANAGER->PopSubmenu();
      }, false));
      sub->AddOption(option::ExecuteOption("option/translation_cancel_delete", "", [&] {
        kMANAGER->PopSubmenu();
      }, false));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsHotkeys, "tab/title/hotkeys", [](Submenu* sub) {
      sub->AddOption(option::SubmenuOption("tab/title/hotkey_profiles", "", Submenus::HotkeyProfiles));
      sub->AddOption(option::ToggleOption("option/hotkey_save_on_exit", "", &hotkey_save_on_exit, false, false))->OnEvent([](Event event) {
        if (event == Event::kChange) {
          if (misc::kHOTKEY_MANAGER->GetSaveOnExit() == hotkey_save_on_exit) {
            LOG_WARN("Hotkey manager and ui save_on_exit out of sync");
          }
          misc::kHOTKEY_MANAGER->SetSaveOnExit(hotkey_save_on_exit);
          hotkey_save_on_change = misc::kHOTKEY_MANAGER->GetSaveOnChange();
        }
      });
      sub->AddOption(option::ToggleOption("option/hotkey_save_on_change", "", &hotkey_save_on_change, false, false))->OnEvent([](Event event) {
        if (event == Event::kChange) {
          if (misc::kHOTKEY_MANAGER->GetSaveOnChange() == hotkey_save_on_change) {
            LOG_WARN("Hotkey manager and ui save_on_change out of sync");
          }
          misc::kHOTKEY_MANAGER->SetSaveOnChange(hotkey_save_on_change);
          hotkey_save_on_exit = misc::kHOTKEY_MANAGER->GetSaveOnExit();
        }
      });
      sub->AddOption(option::LabelOption("label/hotkey_list"));
      const auto hotkeys = misc::kHOTKEY_MANAGER->GetAllHotkeys();
      if (hotkeys.empty()) {
        sub->AddOption(option::ExecuteOption("option/no_hotkeys", "", nullptr, false));
        return;
      } else {
        for (auto&& hotkey : hotkeys) {
          sub->AddOption(option::SubmenuOption(hotkey.second, "", Submenus::HotkeysConfirmDelete, [=] {
            hotkey_delete_key_id = hotkey.first;
          }))->SetRightTextKey(common::VkToStr(hotkey.first));
        }
      }
    });

    kMANAGER->AddSubmenu(Submenus::HotkeyProfiles, "tab/title/hotkey_profiles", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("option/active_hotkey_profile", "", nullptr))->SetRightTextKey(active_hotkey_profile);
      sub->AddOption(option::ExecuteOption("option/refresh_hotkey_profile_list", "", [] { hotkey_profile_paths = misc::HotkeyManager::GetHotkeyProfileList(); }, false));
      sub->AddOption(option::ExecuteOption("option/hotkey_save_current_as", "", [] {
        keyboard::kMANAGER->ShowKeyboard("hotkey_profile_name", [](const std::string& name, keyboard::Result res_state) {
          if (res_state == keyboard::Result::kDone && !name.empty()) {
            misc::kTHREAD_POOL->AddJob([=] {
              misc::kHOTKEY_MANAGER->Save(name);
              settings::profile::SetSelectedHotkeyProfile(name);
              hotkey_profile_paths = misc::HotkeyManager::GetHotkeyProfileList();
            });
          }
        });
      }, false));
      sub->AddOption(option::LabelOption("label/hotkey_profile_list"));
      if (hotkey_profile_paths.empty()) {
        [[unlikely]]
        sub->AddOption(option::ExecuteOption("option/no_hotkey_profiles", "", nullptr, false));
        return;
      } else {
        [[likely]]
        for (auto&& path : hotkey_profile_paths) {
          sub->AddOption(option::ExecuteOption(path.stem().string(), "", [&] {
            misc::kHOTKEY_MANAGER->Load(path.stem().string());

            active_hotkey_profile = path.stem().string();
            settings::profile::SetSelectedHotkeyProfile(active_hotkey_profile);
          }, false));
        }
      }
    });

    kMANAGER->AddSubmenu(Submenus::HotkeysConfirmDelete, "tab/title/delete_hotkey", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("confirm/yes", "", [] {
        misc::kHOTKEY_MANAGER->RemoveHotkey(hotkey_delete_key_id);
        kMANAGER->PopSubmenu();
      }, false));
      sub->AddOption(option::ExecuteOption("confirm/no", "", [] {
        kMANAGER->PopSubmenu();
      }, false));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsTheme, "tab/title/theme", [](Submenu* sub) {
      sub->AddOption(option::NumberOption<float>("option/xpos", "option/xpos/desc", &kMANAGER->x_base, 0.005f, 0.f, 1.f - kMANAGER->x_size, false));
      sub->AddOption(option::NumberOption<float>("option/ypos", "option/ypos/desc", &kMANAGER->y_base, 0.005f, 0.f, 1.f, false));
      sub->AddOption(option::NumberOption<int>("option/max_options", "option/max_options/desc", &kMANAGER->max_drawn_options, 1, 1, 30, false));
    });

    kMANAGER->AddSubmenu(Submenus::SettingsUnloadConfirm, "tab/title/unload", [](Submenu* sub) {
      sub->AddOption(option::ExecuteOption("confirm/yes", "", [] {
        globals::running = false;
      }));
      sub->AddOption(option::ExecuteOption("confirm/no", "", [] {
        kMANAGER->PopSubmenu();
      }, false));
    });
  }
}
 */