//
// Created by X-ray on 08/02/22.
//

#include "uitick.hpp"
#include "../../memory/pointers.hpp"
#include "../../logger/logger.hpp"
#include "../../misc/common.hpp"
#include "../../ui/manager.hpp"
#include "../../ui/notification/notification.hpp"
#include "../../ui/components/options/executeoption.hpp"
#include "../../ui/components/options/submenuoption.hpp"
#include "../../ui/components/options/numberoption.hpp"
#include "../../ui/components/options/togglenumberoption.hpp"
#include "../../ui/components/options/labeloption.hpp"
#include "../../ui/components/options/toggleoption.hpp"
#include "../../ui/components/options/listoption.hpp"
#include "../../ui/components/options/togglelistoption.hpp"
#include "../../natives/natives.hpp"
#include "../../fiber/pool.hpp"

namespace gta_base {
  namespace scripts {
    static bool never_wanted = false;

    void UiTick::Init() {
      if (ui::kMANAGER) {
        initialized_ = true;

        using namespace ui;

        kMANAGER->AddSubmenu("Home", components::Submenus::Home, [](components::Submenu* sub){
          auto player = *memory::kPOINTERS->ped_factory_;
          sub->AddOption(components::option::NumberOption<std::uint32_t>("Wanted Level", "Press enter to set to current wanted level", player->m_local_ped->m_player_info->m_wanted_level, 1.f, 0.f, 5.f));
          sub->AddOption(components::option::ExecuteOption("fake wanted lvl 0", "", [] {
            fiber::kPOOL->AddJob([]{
              MISC::SET_FAKE_WANTED_LEVEL(0);
            });
          }));
          sub->AddOption(components::option::ExecuteOption("fake wanted lvl 6", "", [] {
            fiber::kPOOL->AddJob([]{
              MISC::SET_FAKE_WANTED_LEVEL(6);
            });
          }));

          sub->AddOption(components::option::SubmenuOption("Player", "", components::Submenus::Player));
          sub->AddOption(components::option::SubmenuOption("Debug", "a short description a short description just a bit too long", components::Submenus::Debug));
          sub->AddOption(components::option::SubmenuOption("Settings", "this is a really long description as you can see weep woop", components::Submenus::Settings));
        });

        kMANAGER->AddSubmenu("Player", components::Submenus::Player, [this](components::Submenu* sub){
          auto player = *memory::kPOINTERS->ped_factory_;
          sub->AddOption(components::option::NumberOption<std::uint32_t>("wanted_level", "wanted_level_desc", player->m_local_ped->m_player_info->m_wanted_level, 1.f, 0.f, 5.f));
        });

        kMANAGER->AddSubmenu("Debug", components::Submenus::Debug, [this](components::Submenu* sub){
          sub->AddOption(components::option::SubmenuOption("Test Components", "", components::Submenus::TestComponents));
          sub->AddOption(components::option::ExecuteOption("Test notify", "", []{
            kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Test notify", "This is a test of the info notification. Seems to look pretty good");
            kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Test notify", "This is a test of the success notification. Seems to look pretty good");
            kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Test notify", "This is a test of the fail notification. Seems to look pretty good");
          }));
          sub->AddOption(components::option::ExecuteOption("Test notify 2", "", []{
            kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "1 line");
            kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2");
            kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line ");
          }));
        });

        kMANAGER->AddSubmenu("Test Components", components::Submenus::TestComponents, [this](components::Submenu* sub){
          sub->AddOption(components::option::SubmenuOption("Test SubmenuOption", "", components::Submenus::Home));
          sub->AddOption(components::option::LabelOption("Test LabelOption"));
          sub->AddOption(components::option::ExecuteOption("Test ExecuteOption", "", []{
            LOG_DEBUG("Test ExecuteOption");
          }));
          sub->AddOption(components::option::ToggleOption("Test ToggleOption", "", &test_value_b));
          sub->AddOption(components::option::NumberOption<float>("Test NumberOption", "", test_value_f, 0.5, 0, 10))->OnEvent([this](components::Event event){
            if (event == components::Event::kChange)
              LOG_DEBUG("Test NumberOption {}", test_value_f);
          });
          sub->AddOption(components::option::ToggleNumberOption<float>("Test ToggleNumberOption", "", test_value_b_toggle_number, test_value_f_toggle_number, 0.5, 0, 10))->OnEvent([this](components::Event event){
            if (event == components::Event::kChange)
              LOG_DEBUG("Test ToggleNumberOption {} {}", test_value_b_toggle_number, test_value_f_toggle_number);
          });
          sub->AddOption(components::option::ListOption("Test ListOption", "", test_value_v_idx, test_value_v));
          sub->AddOption(components::option::ToggleListOption("Test ToggleListOption", "", test_value_b_toggle_list, test_value_v_idx_toggle_list, test_value_v));
        });

        kMANAGER->AddSubmenu("Settings", components::Submenus::Settings, [](components::Submenu* sub){
          sub->AddOption(components::option::SubmenuOption("Unload", "", components::Submenus::UnloadConfirm));
        });

        kMANAGER->AddSubmenu("Unload", components::Submenus::UnloadConfirm, [](components::Submenu* sub) {
          sub->AddOption(components::option::ExecuteOption("Yes", "", [] {
            common::globals::running = false;
          }));
          sub->AddOption(components::option::ExecuteOption("No", "", [] {
            kMANAGER->PopSubmenu();
          }));
        });
      }
    }

    void UiTick::RunTick() {
      ui::kMANAGER->Draw();
    }
  }
}