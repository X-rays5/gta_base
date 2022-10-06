//
// Created by X-ray on 09/18/22.
//

#include "selected_player.hpp"
#include "tab_includes.hpp"
#include "../../rage/util/teleport.hpp"
#include "../../rage/util/entity.hpp"
#include "../../rage/util/streaming.hpp"
#include "../../rage/enums.hpp"
#include "../../rage/util/breakup_kick.hpp"

namespace gta_base::ui::tabs {
      void SelectedPlayerTab() {
        kMANAGER->AddSubmenu(Submenus::NetworkPlayerList, "tab/title/player_list", [](Submenu* sub){
          sub->AddOption(option::LabelOption(fmt::format("Players: {}", kPLAYER_MGR->Size())));
          kPLAYER_MGR->Iterate(true, [&](const std::string& name, const std::shared_ptr<player_mgr::Player>& player){
            sub->AddOption(option::SubmenuOption(name, "", Submenus::PlayerListSelectedPlayer, [&]{
              kPLAYER_MGR->SetSelectedPlayer(player);
              kMANAGER->GetSubmenu(Submenus::PlayerListSelectedPlayer)->SetNameKey(player->Name());
            }));
          });
        });

        kMANAGER->AddSubmenu(Submenus::PlayerListSelectedPlayer, "", [](Submenu* sub){
          sub->AddOption(option::LabelOption("label/teleport_remote"));
          sub->AddOption(option::ExecuteOption("option/teleport_to_selected", "", []{
            fiber::kPOOL->AddJob([]{
              auto target = rage::util::LoadGroundAtCoord(kPLAYER_MGR->GetSelectedPlayer()->Ped()->m_position);
              rage::util::Teleport((rage::fvector3)target, true);
            });
          }));
          sub->AddOption(option::ExecuteOption("option/teleport_into_selected_vehicle", "", []{
            fiber::kPOOL->AddJob([]{
              Ped target_ped = rage::PtrToHandle(kPLAYER_MGR->GetSelectedPlayer()->Ped());
              if (PED::IS_PED_IN_ANY_VEHICLE(target_ped, false)) {
                rage::util::TeleportIntoVehicle(PED::GET_VEHICLE_PED_IS_IN(target_ped, false));
              } else {
                kNOTIFICATIONS->Create(Notification::Type::kFail, "Teleport into vehicle error", "The selected player is currently not in a vehicle. You might be too far away");
              }
            });
          }));
          sub->AddOption(option::LabelOption("label/toxic_remote"));
          sub->AddOption(option::ExecuteOption("Kick", "", []{
            if (common::IsSessionStarted() && kPLAYER_MGR->GetSelectedPlayer() != kPLAYER_MGR->GetSelf()) {
              rage::BreakupKick(kPLAYER_MGR->GetSelectedPlayer());
            }
          }));
        });
      }
    }