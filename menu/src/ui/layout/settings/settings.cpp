//
// Created by X-ray on 26/03/2026.
//

#include "settings.hpp"
#include <algorithm>
#include <array>
#include <base-common/fs/vfs.hpp>
#include "../../menu_renderer.hpp"
#include "../../../hotkey/hotkey_manager.hpp"
#include "../../../options/option_registry.hpp"
#include "../../components/components.hpp"
#include "../../header/header_image.hpp"

namespace {
  base::menu::hotkey::Hotkey cur_hotkey_remove;
  std::string cur_hotkey_remove_name;
}

namespace base::menu::ui::layout {
  namespace {
    /**
     * The header images the theme submenu offers.
     *
     * Two lists for one row: what the row shows (a label for "no image" rather than nothing, so the
     * choice can be read and gone back to) and what each choice means as a setting, which is the
     * file name or no name at all.
     */
    struct HeaderImages {
      std::shared_ptr<const std::vector<std::string>> display = std::make_shared<const std::vector<std::string>>();
      std::shared_ptr<const std::vector<std::string>> values = std::make_shared<const std::vector<std::string>>(1, std::string{});
      std::shared_ptr<std::atomic<std::size_t>> index = std::make_shared<std::atomic<std::size_t>>(0);
      /// What was last written into the theme, so the row writes the theme when the choice changes
      /// and not on every frame - a theme loaded with an image that is not in the directory keeps it.
      std::size_t applied = 0;
    };

    /// The images directory as it is now, with the theme's own choice selected if it is in there.
    void RefreshHeaderImages(HeaderImages& images, const std::shared_ptr<Theme>& theme) {
      auto display = std::make_shared<std::vector<std::string>>();
      auto values = std::make_shared<std::vector<std::string>>();
      display->push_back(localization::kMANAGER->Localize("ui/option/header_image_none"));
      values->emplace_back();

      std::size_t selected = 0;
      for (const auto& name : header_image::ListAvailable(common::fs::vfs::GetImageDir())) {
        display->push_back(name);
        values->push_back(name);
        if (name == theme->header_image) {
          selected = values->size() - 1;
        }
      }

      images.display = std::move(display);
      images.values = std::move(values);
      images.index->store(selected);
      images.applied = selected;
    }

    /// The header types the theme submenu offers, in the order the row shows them, and the label each
    /// one is shown under. A row is an index, so one of these is what turns it back into a type.
    constexpr std::array kHeaderTypes{HeaderType::kText, HeaderType::kImage};
    constexpr std::array<std::string_view, kHeaderTypes.size()> kHeaderTypeLabels{
      "ui/option/header_type_text",
      "ui/option/header_type_image",
    };

    /// The header types the theme submenu offers. Same shape as the image row: the types are fixed,
    /// but their labels are localized, so the row is built with the page like the image list is.
    struct HeaderTypes {
      std::shared_ptr<const std::vector<std::string>> display = std::make_shared<const std::vector<std::string>>();
      std::shared_ptr<const std::vector<HeaderType>> values = std::make_shared<const std::vector<HeaderType>>();
      std::shared_ptr<std::atomic<std::size_t>> index = std::make_shared<std::atomic<std::size_t>>(static_cast<std::uint32_t>(kMENU_RENDERER->GetTheme()->header_type));
      /// What was last written into the theme - see HeaderImages::applied.
      std::size_t applied = 0;
    };

    /// The types MakeHeader takes, with the theme's own choice selected. A theme that never chose one,
    /// or stored a value this build has no type for, is on the default, the first one.
    void RefreshHeaderTypes(HeaderTypes& types, const std::shared_ptr<Theme>& theme) {
      auto display = std::make_shared<std::vector<std::string>>();
      auto values = std::make_shared<std::vector<HeaderType>>();

      for (std::size_t i = 0; i < kHeaderTypes.size(); ++i) {
        display->push_back(localization::kMANAGER->Localize(kHeaderTypeLabels.at(i)));
        values->push_back(kHeaderTypes.at(i));
      }

      std::size_t selected = 0;
      for (std::size_t i = 0; i < values->size(); ++i) {
        if (values->at(i) == theme->header_type) {
          selected = i;
        }
      }

      types.display = std::move(display);
      types.values = std::move(values);
      types.index->store(selected);
      types.applied = selected;
    }

    /**
     * Both header rows, held by one shared_ptr because the submenu's callbacks keep what they capture
     * in a fixed amount of room - two pointers for the theme and this - and the rows are three lots of
     * state between them.
     */
    struct HeaderRows {
      HeaderImages images;
      HeaderTypes types;
    };
  }

  void ThemeSub() {
    // Get theme reference that captures the shared_ptr to keep it alive
    auto menuTheme = kMENU_RENDERER->GetTheme();
    auto header_rows = std::make_shared<HeaderRows>();
    RefreshHeaderImages(header_rows->images, menuTheme);
    RefreshHeaderTypes(header_rows->types, menuTheme);

    Submenu theme_submenu("ui/sub/theme", [menuTheme, header_rows] (Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/save", "", [menuTheme] {
        auto res = menuTheme->Save("default");
      }));
      sub->AddComponent(components::ExecuteComponent("label/load", "", [menuTheme] {
        auto res = menuTheme->Load("default");
      }));
      // Pass references directly to theme member variables so changes affect the actual theme object
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_x_pos", "", menuTheme->x_position, 0.005F));
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_y_pos", "", menuTheme->y_position, 0.005F));
      sub->AddComponent(components::NumberRangeComponent("ui/option/ui_scale", "", menuTheme->menu_ui_scale, 0.1F));
      sub->AddComponent(components::ToggleComponent("ui/option/render_header", "", &menuTheme->render_header));

      const std::size_t selected_type = std::min(header_rows->types.index->load(), header_rows->types.values->size() - 1);
      if (selected_type != header_rows->types.applied) {
        header_rows->types.applied = selected_type;
        menuTheme->header_type = header_rows->types.values->at(selected_type);
      }
      sub->AddComponent(components::ListComponent("ui/option/header_type", "", header_rows->types.display, header_rows->types.index));

      // The image belongs to the image header, so its row is only there while that is the header being
      // drawn: a row the player cannot affect is worse than no row.
      if (header_rows->types.values->at(selected_type) == HeaderType::kImage) {
        const std::size_t selected = std::min(header_rows->images.index->load(), header_rows->images.values->size() - 1);
        if (selected != header_rows->images.applied) {
          header_rows->images.applied = selected;
          menuTheme->header_image = header_rows->images.values->at(selected);
        }
        sub->AddComponent(components::ListComponent("ui/option/header_image", "", header_rows->images.display, header_rows->images.index));
      }

      sub->AddComponent(components::ToggleComponent("ui/option/mouse_input", "", &menuTheme->mouse_input_enabled));
    }, [menuTheme, header_rows](Submenu*) {
      // Rescanned on opening the page, so an image dropped into the directory is offered without a
      // restart.
      RefreshHeaderImages(header_rows->images, menuTheme);
      RefreshHeaderTypes(header_rows->types, menuTheme);
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kTHEME_SETTINGS, std::move(theme_submenu));
  }

  void FeatureSettingsSub() {
    Submenu feature_submenu("ui/sub/feature_settings", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/save", "", [] {
        if (options::kOPTION_REGISTRY) {
          auto stat = options::kOPTION_REGISTRY->SaveOptions("default");
        }
      }));
      sub->AddComponent(components::ExecuteComponent("label/load", "", [] {
        if (options::kOPTION_REGISTRY) {
          auto stat = options::kOPTION_REGISTRY->LoadOptions("default");
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kLOAD_FEATURE_SETTINGS, std::move(feature_submenu));
  }

  void RemoveHotkeyConfirmSub() {
    Submenu remove_hotkey_sub("ui/sub/remove_hotkey_confirm", [](Submenu* sub) {
      const std::string desc = fmt::format(fmt::runtime(localization::kMANAGER->Localize("ui/option/remove_hotkey_confirm_desc")), cur_hotkey_remove_name);
      sub->AddComponent(components::ExecuteComponent("label/confirm", desc, [] {
        if (hotkey::kHOTKEY_MANAGER) {
          LOG_DEBUG("Removing hotkey: {} - {}", cur_hotkey_remove.combined, cur_hotkey_remove.AsString());
          hotkey::kHOTKEY_MANAGER->RemoveHotkey(cur_hotkey_remove);
          kMENU_RENDERER->PopSubmenu();
        }
      }));
      sub->AddComponent(components::ExecuteComponent("label/cancel", "", [] {
        if (kMENU_RENDERER) {
          kMENU_RENDERER->PopSubmenu();
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kHOTKEY_REMOVE_CONFIRM, std::move(remove_hotkey_sub));
  }

  void HotkeysSub() {
    Submenu hotkeys_submenu("ui/sub/hotkeys", [](Submenu* sub) {
      if (hotkey::kHOTKEY_MANAGER) {
        const auto hotkeys = hotkey::kHOTKEY_MANAGER->GetAllHotkeys();
        for (const auto& [hotkey, option] : hotkeys) {
          auto comp = components::SubLinkComponent(SubmenuIDs::kHOTKEY_REMOVE_CONFIRM, [hotkey, option] {
            cur_hotkey_remove = hotkey;
            cur_hotkey_remove_name = option->GetName();
          });
          comp.SetName(option->GetName());
          comp.SetDescription(hotkey.AsString());

          sub->AddComponent(std::move(comp));
        }
      }

      if (sub->IsEmpty()) {
        sub->AddComponent(components::LabelComponent("ui/option/no_hotkeys"));
      }
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kHOTKEYS, std::move(hotkeys_submenu));

    RemoveHotkeyConfirmSub();
  }

  void UnloadConfirmSub() {
    Submenu unload_sub("ui/sub/unload", [](Submenu* sub) {
      sub->AddComponent(components::ExecuteComponent("label/confirm", "", [] {
        globals::kRUNNING = false;
      }));
      sub->AddComponent(components::ExecuteComponent("label/cancel", "", [] {
        if (kMENU_RENDERER) {
          kMENU_RENDERER->PopSubmenu();
        }
      }));
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kUNLOAD_CONFIRM, std::move(unload_sub));
  }

  void InitSettingsLayout() {
    Submenu settings_submenu("ui/sub/settings", [](Submenu* sub) {
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kTHEME_SETTINGS));
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kLOAD_FEATURE_SETTINGS));
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kHOTKEYS));
#ifndef NDEBUG
      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kUNLOAD_CONFIRM));
#endif
    });
    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSETTINGS, std::move(settings_submenu));

    ThemeSub();
    FeatureSettingsSub();
    HotkeysSub();

#ifndef NDEBUG
    UnloadConfirmSub();
#endif
  }
}