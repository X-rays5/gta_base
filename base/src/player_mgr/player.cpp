//
// Created by X-ray on 09/08/22.
//

#include "player.hpp"
#include "../memory/pointers.hpp"
#include "../rage/util/get.hpp"

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

   rage::snPlayer* Player::SessionPlayer() {
     auto network = rage::GetNetwork();
     for (auto i = 0; i < network->m_game_session_ptr->m_player_count; i++) {
       if (network->m_game_session_ptr->m_players[i]->m_player_data.m_host_token == NetData()->m_host_token)
         return network->m_game_session_ptr->m_players[i];
     }

     if (network->m_game_session_ptr->m_local_player.m_player_data.m_host_token == NetData()->m_host_token)
       return &network->m_game_session_ptr->m_local_player;

     return nullptr;
   }

   rage::snPeer* Player::SessionPeer() {
     auto network = rage::GetNetwork();
     for (auto i = 0; i < network->m_game_session_ptr->m_peer_count; i++) {
       if (network->m_game_session_ptr->m_peers[i]->m_peer_data.m_gamer_handle_2.m_rockstar_id == NetData()->m_gamer_handle_2.m_rockstar_id) {
         return network->m_game_session_ptr->m_peers[i];
       }
     }

     return nullptr;
   }

   std::string Player::Name() {
     if (!self_ || !self_->m_player_info)
       return {};

     return self_->get_name();
   }

   std::string Player::NameLowerCase() {
     std::string res = Name();
     std::transform(res.begin(), res.end(), res.begin(), ::tolower);

     return res;
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
     if (!self_)
       return false;

     return self_->m_player_id == other.m_player_id;
   }
 }
}