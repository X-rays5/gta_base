//
// Created by X-ray on 09/08/22.
//

#include "player.hpp"

namespace gta_base {
 namespace player_mgr {

   CNetGamePlayer* Player::NetPlayer() {
     if (self_)
       return self_;

     return nullptr;
   }

   CNonPhysicalPlayerData* Player::NonPhysicalPlayer() {
     if (self_->m_non_physical_player)
       self_->m_non_physical_player;

     return nullptr;
   }

   rage::rlGamerInfo* Player::NetData() {
     return self_->get_net_data();
   }

   CPlayerInfo* Player::Info() {
     if (self_->m_player_info)
       return self_->m_player_info;

     return nullptr;
   }

   CPed* Player::Ped() {
     if (auto info = Info())
       if (info->m_ped)
         return info->m_ped;

     return nullptr;
   }

   CVehicle* Player::Vehicle() {
     if (auto ped = Ped())
       if (ped->m_vehicle)
         return ped->m_vehicle;

     return nullptr;
   }

   ClanData* Player::Clan() {
     return &self_->m_clan_data;
   }

   std::uint8_t Player::Id() const {
     return self_->m_player_id;
   }

   bool Player::Host() const {
     return self_->is_host();
   }

   bool Player::Valid() const {
     return self_->is_valid();
   }

   bool Player::operator==(CNetGamePlayer& other) const {
     return self_->m_player_id == other.m_player_id;
   }
 }
}