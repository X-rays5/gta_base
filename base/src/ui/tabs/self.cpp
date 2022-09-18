//
// Created by X-ray on 09/18/22.
//

#include "self.hpp"
#include "tab_includes.hpp"
#include "../../rage/util/teleport.hpp"

namespace gta_base {
  namespace ui {
    namespace tabs {
      void SelfTab() {
        kMANAGER->AddSubmenu(Submenus::Player, "tab/title/self", [](Submenu* sub) {
          auto player = rage::GetLocalPed();
          sub->AddOption(option::SubmenuOption("tab/title/player_health", "", Submenus::PlayerHealth));
          sub->AddOption(option::ExecuteOption("option/teleport_waypoint", "", []{
            fiber::kPOOL->AddJob([]{
              rage::util::TeleportToWayPoint(true);
            });
          }));
          sub->AddOption(option::ExecuteOption("option/teleport_objective", "", []{
            fiber::kPOOL->AddJob([]{
              rage::util::TeleportToObjective(true);
            });
          }));
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
      }
    }
  }
}