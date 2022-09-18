//
// Created by X-ray on 09/18/22.
//

#include "selected_player.hpp"
#include "tab_includes.hpp"
#include "../../rage/util/teleport.hpp"

namespace gta_base {
  namespace ui {
    namespace tabs {
      void SelectedPlayerTab() {
        kMANAGER->AddSubmenu(Submenus::PlayerList, "tab/title/player_list", [](Submenu* sub){
          sub->AddOption(option::LabelOption(fmt::format("Players: {}", kPLAYER_MGR->Size())));
          kPLAYER_MGR->Iterate(true, [&](const std::string& name, const std::shared_ptr<player_mgr::Player>& player){
            sub->AddOption(option::SubmenuOption(name, "", Submenus::SelectedPlayer, [&]{
              kPLAYER_MGR->SetSelectedPlayer(player);
              kMANAGER->GetSubmenu(Submenus::SelectedPlayer)->SetNameKey(player->Name());
            }));
          });
        });

        kMANAGER->AddSubmenu(Submenus::SelectedPlayer, "", [](Submenu* sub){
          sub->AddOption(option::LabelOption("label/teleport_remote"));
          sub->AddOption(option::ExecuteOption("option/teleport_to_selected", "", []{
            fiber::kPOOL->AddJob([]{
              rage::util::Teleport(kPLAYER_MGR->GetSelectedPlayer()->Ped()->m_position, true);
            });
          }));
          sub->AddOption(option::ExecuteOption("option/teleport_into_selected_vehicle", "", []{
            fiber::kPOOL->AddJob([]{
              if (kPLAYER_MGR->GetSelectedPlayer()->Vehicle()) {
                rage::util::TeleportIntoVehicle(memory::kPOINTERS->PtrToHandle(kPLAYER_MGR->GetSelectedPlayer()->Vehicle()));
              } else {
                kNOTIFICATIONS->Create(Notification::Type::kFail, "Teleport into vehicle error", "The selected player is currently not in a vehicle. You might be too far away");
              }
            });
          }));
          sub->AddOption(option::LabelOption("label/toxic_remote"));
          sub->AddOption(option::ExecuteOption("Kick", "", []{
            if (common::IsSessionStarted() && kPLAYER_MGR->GetSelectedPlayer() != kPLAYER_MGR->GetSelf()) {
              //rage::BreakupKick(kPLAYER_MGR->GetSelectedPlayer());
            }
          }));
        });
      }
    }
  }
}