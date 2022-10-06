//
// Created by X-ray on 09/18/22.
//

#include "network.hpp"
#include "tab_includes.hpp"
#include "all_players.hpp"
#include "selected_player.hpp"
#include "../../rage/util/session_switcher.hpp"

namespace gta_base::ui::tabs {
      static std::size_t session_switcher_idx = 0;

      void NetworkTab() {
        kMANAGER->AddSubmenu(Submenus::Network, "tab/title/network", [](Submenu* sub){
          if (common::IsSessionStarted()) {
            sub->AddOption(option::SubmenuOption("tab/title/player_list", "", Submenus::NetworkPlayerList));
            sub->AddOption(option::SubmenuOption("tab/title/all_players", "", Submenus::NetworkAllPlayers));
          }
          sub->AddOption(option::SubmenuOption("tab/title/session_switcher", "", Submenus::NetworkSessionSwitcher));
          sub->AddOption(option::SubmenuOption("tab/title/request_services", "", Submenus::NetworkRequestServices));
        });

        kMANAGER->AddSubmenu(Submenus::NetworkSessionSwitcher, "tab/title/session_switcher", [](Submenu* sub){
          sub->AddOption(option::ExecuteOption("option/leave_session", "", []{
            fiber::kPOOL->AddJob([]{
              rage::util::SessionSwitcher(rage::eSessionType::LEAVE_ONLINE);
            });
          }));
          sub->AddOption(option::ExecuteOption("option/bail_from_session", "", []{
            fiber::kPOOL->AddJob([]{
              NETWORK::NETWORK_SESSION_LEAVE_SINGLE_PLAYER();
            });
          }));
          sub->AddOption(option::LabelOption("label/session_switcher"));
          sub->AddOption(option::ListOption<rage::util::SessionType>("option/session_switcher_select", "", session_switcher_idx, rage::util::session_types_list));
          sub->AddOption(option::ExecuteOption("option/session_switcher_switch", "", []{
            fiber::kPOOL->AddJob([]{
              rage::util::SessionSwitcher(rage::util::session_types_list[session_switcher_idx].id);
            });
          }));
        });

        kMANAGER->AddSubmenu(Submenus::NetworkRequestServices, "tab/title/request_services", [](Submenu* sub){
          sub->AddOption(option::ExecuteOption("option/request_avenger", "", []{
            rage::script::Global(rage::Globals::RequestServices::kMain).At(rage::Globals::RequestServices::kAvenger).As<bool>() = true;
          }));
          sub->AddOption(option::ExecuteOption("option/request_moc", "", []{
            rage::script::Global(rage::Globals::RequestServices::kMain).At(rage::Globals::RequestServices::kMoc).As<bool>() = true;
          }));
          sub->AddOption(option::ExecuteOption("option/request_terrorbyte", "", []{
            rage::script::Global(rage::Globals::RequestServices::kMain).At(rage::Globals::RequestServices::kTerrorbyte).As<bool>() = true;
          }));
          sub->AddOption(option::ExecuteOption("option/request_kosatka", "", []{
            rage::script::Global(rage::Globals::RequestServices::kMain).At(rage::Globals::RequestServices::kKosatka).As<bool>() = true;
          }));
          sub->AddOption(option::ExecuteOption("option/request_ballistic_armor", "", []{
            rage::script::Global(rage::Globals::RequestServices::kMain).At(rage::Globals::RequestServices::kBallisticArmor).As<bool>() = true;
          }));
          sub->AddOption(option::ExecuteOption("option/request_dinghy", "", []{
            rage::script::Global(rage::Globals::RequestServices::kMain).At(rage::Globals::RequestServices::kDingy).As<bool>() = true;
          }));
        });

        AllPlayersTab();
        SelectedPlayerTab();
      }
    }