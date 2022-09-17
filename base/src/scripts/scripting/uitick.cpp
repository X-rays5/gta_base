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
#include "../../rage/util/get.hpp"
#include "../../rage/util/breakup_kick.hpp"
#include "../../player_mgr/manager.hpp"
#include "../../misc/settings.hpp"

namespace gta_base {
  namespace scripts {
    static bool never_wanted = false;

    void UiTick::Init() {
      if (ui::kMANAGER) {
        initialized_ = true;

        using namespace ui;
        using namespace ui::components;

        kMANAGER->AddSubmenu(Submenus::Home, "tab/title/home", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("tab/title/self", "", Submenus::Player));
          if (common::IsSessionStarted()) {
            sub->AddOption(option::SubmenuOption("tab/title/player_list", "", Submenus::PlayerList));
          }
#ifndef NDEBUG
          sub->AddOption(option::SubmenuOption("Debug", "", Submenus::Debug));
#endif
          sub->AddOption(option::SubmenuOption("tab/title/misc", "", Submenus::Misc));
          sub->AddOption(option::SubmenuOption("tab/title/setting", "", Submenus::Settings));
        });

        kMANAGER->AddSubmenu(Submenus::Player, "tab/title/self", [](Submenu* sub) {
          auto player = rage::GetLocalPed();
          sub->AddOption(option::SubmenuOption("tab/title/player_health", "", Submenus::PlayerHealth));
          sub->AddOption(option::NumberOption<std::uint32_t>("option/wanted_level", "option/wanted_level/desc", &player->m_player_info->m_wanted_level, 1.f, 0.f, 5.f));
        });

        kMANAGER->AddSubmenu(Submenus::PlayerHealth, "tab/title/player_health", [](Submenu* sub){
          auto player = rage::GetLocalPed();
          sub->AddOption(option::ToggleOption("option/god_mode", "option/unimplemented_option/desc", &kSETTINGS.player.god_mode));
          sub->AddOption(option::ToggleOption("option/semi_god_mode", "option/semi_god_mode/desc", &kSETTINGS.player.semi_god_mode));
          sub->AddOption(option::ExecuteOption("option/suicide", "", [player]{
            player->m_health = 0;
          }));
          sub->AddOption(option::ExecuteOption("option/heal", "", [player]{
            fiber::kPOOL->AddJob([player]{
              player->m_health = ENTITY::GET_ENTITY_MAX_HEALTH(globals::local_player.ped_id);
            });
          }));
          sub->AddOption(option::ExecuteOption("option/max_armor", "", [player]{
            fiber::kPOOL->AddJob([player]{
              player->m_armor= PLAYER::GET_PLAYER_MAX_ARMOUR(globals::local_player.id);
            });
          }));
          sub->AddOption(option::ExecuteOption("option/remove_armor", "", [player]{
            player->m_armor = 0;
          }));
        });

        kMANAGER->AddSubmenu(components::Submenus::PlayerList, "tab/title/player_list", [](Submenu* sub){
          sub->AddOption(option::LabelOption(fmt::format("Players: {}", kPLAYER_MGR->Size())));
          kPLAYER_MGR->Iterate(true, [&](const std::string& name, const std::shared_ptr<player_mgr::Player>& player){
            sub->AddOption(option::SubmenuOption(name, "", Submenus::SelectedPlayer, [&]{
              kPLAYER_MGR->SetSelectedPlayer(player);
              kMANAGER->GetSubmenu(Submenus::SelectedPlayer)->SetNameKey(player->Name());
            }));
          });
        });

        kMANAGER->AddSubmenu(components::Submenus::SelectedPlayer, "", [](Submenu* sub){
          sub->AddOption(option::ExecuteOption("Kick", "", []{
            if (common::IsSessionStarted() && kPLAYER_MGR->GetSelectedPlayer() != kPLAYER_MGR->GetSelf()) {
              rage::BreakupKick(kPLAYER_MGR->GetSelectedPlayer());
            }
          }));
        });

        kMANAGER->AddSubmenu(components::Submenus::Debug, "Debug", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("Test Components", "", Submenus::TestComponents));
          sub->AddOption(option::ExecuteOption("Test notify", "", []{
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

        kMANAGER->AddSubmenu(Submenus::TestComponents, "Test Components", [this](Submenu* sub){
          sub->AddOption(option::SubmenuOption("Test SubmenuOption", "", Submenus::Home));
          sub->AddOption(option::LabelOption("Test LabelOption"));
          sub->AddOption(option::ExecuteOption("Test ExecuteOption", "", []{
            LOG_DEBUG("Test ExecuteOption");
          }));
          sub->AddOption(option::ToggleOption("Test ToggleOption", "", &test_value_b));
          sub->AddOption(option::NumberOption<float>("Test NumberOption", "", &test_value_f, 0.5, 0, 10))->OnEvent([this](Event event){
            if (event == Event::kChange)
              LOG_DEBUG("Test NumberOption {}", test_value_f);
          });
          sub->AddOption(option::ToggleNumberOption<float>("Test ToggleNumberOption", "", test_value_b_toggle_number, test_value_f_toggle_number, 0.5, 0, 10))->OnEvent([this](Event event){
            if (event == Event::kChange)
              LOG_DEBUG("Test ToggleNumberOption {} {}", test_value_b_toggle_number, test_value_f_toggle_number);
          });
          sub->AddOption(option::ListOption("Test ListOption", "", test_value_v_idx, test_value_v));
          sub->AddOption(option::ToggleListOption("Test ToggleListOption", "", test_value_b_toggle_list, test_value_v_idx_toggle_list, test_value_v));
        });

        kMANAGER->AddSubmenu(Submenus::Misc, "tab/title/misc", [](Submenu* sub){
          sub->AddOption(option::ExecuteOption("option/skip_cutscene", "", []{
            fiber::kPOOL->AddJob([]{
              CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
            });
          }));
        });

        kMANAGER->AddSubmenu(Submenus::Settings, "tab/title/setting", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("tab/title/theme", "", Submenus::Theme));
          sub->AddOption(option::SubmenuOption("tab/title/unload", "", Submenus::UnloadConfirm));
        });

        kMANAGER->AddSubmenu(Submenus::Theme, "tab/title/theme", [](Submenu* sub){
          sub->AddOption(option::NumberOption<float>("option/xpos", "option/xpos/desc", &kMANAGER->x_base, 0.005f, 0.f, 1.f - kMANAGER->x_size));
          sub->AddOption(option::NumberOption<float>("option/ypos", "option/ypos/desc", &kMANAGER->y_base, 0.005f, 0.f, 1.f));
          sub->AddOption(option::NumberOption<int>("option/max_options", "option/max_options/desc", &kMANAGER->max_drawn_options, 1, 1, 30));
        });

        kMANAGER->AddSubmenu(Submenus::UnloadConfirm, "tab/title/unload", [](Submenu* sub) {
          sub->AddOption(option::ExecuteOption("confirm/yes", "", [] {
            globals::running = false;
          }));
          sub->AddOption(option::ExecuteOption("confirm/no", "", [] {
            kMANAGER->PopSubmenu();
          }));
        });

        const std::string build_time = std::string("Compilation time: ") + globals::compile_date + " " + globals::compile_time;
        kNOTIFICATIONS->Create(Notification::Type::kInfo, "UI Loaded", build_time);
      }
    }

    void UiTick::RunTick() {
      ui::kMANAGER->Draw();
    }
  }
}