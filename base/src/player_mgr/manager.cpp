//
// Created by X-ray on 09/08/22.
//

#include "manager.hpp"

#include <utility>
#include "../ui/manager.hpp"

namespace gta_base::player_mgr {
    Manager::Manager() {
      kPLAYER_MGR = this;

      auto network_player_mgr = rage::GetNetworkPlayerMgr();
      if (!network_player_mgr)
        return;

      self_ = std::make_shared<Player>(reinterpret_cast<::CNetGamePlayer*>(network_player_mgr->m_local_net_player));

      for (std::uint32_t i = 0; i < network_player_mgr->m_player_limit; i++) {
        auto player = reinterpret_cast<::CNetGamePlayer*>(network_player_mgr->m_player_list[i]);
        if (!player)
          continue;

        AddPlayer(player);
      }
    }

    Manager::~Manager() {
      kPLAYER_MGR = nullptr;
    }

    void Manager::SetSelf(CNetGamePlayer* self) {
      if (!self) {
        LOG_DEBUG("Manager::SetSelf called with a nullptr");
        return;
      }

      self_ = std::make_shared<Player>(self);
    }

    Manager::player_ptr_t Manager::GetSelf() {
      return self_;
    }

    void Manager::AddPlayer(CNetGamePlayer* player) {
      if (!player) {
        LOG_DEBUG("Manager::AddPlayer called with a nullptr");
        return;
      }

      auto player_ptr = std::make_shared<Player>(player);
      player_list_.emplace(player_ptr->NameLowerCase(), std::move(player_ptr));

      LOG_DEBUG("Added player: {}", player->get_name());
    }

    void Manager::RemovePlayer(CNetGamePlayer* player) {
      if (!player) {
        LOG_DEBUG("Manager::RemovePlayer called with a nullptr");
        return;
      }

      if (selected_player_ && *selected_player_ == *player)
        selected_player_ = nullptr;

      RemovePlayer(std::make_unique<Player>(player)->NameLowerCase());
    }

    void Manager::RemovePlayer(const std::string& name) {
      auto itr = player_list_.find(name);
      if (itr == player_list_.end()) {
        LOG_WARN("Tried to remove player with name {} from player manager but doesn't exist", name);
        return;
      }

      player_list_.erase(itr);

      LOG_DEBUG("Removed player: {}", name);
    }

    Manager::player_ptr_t Manager::GetPlayer(const std::string& name) {
      return GetPlayer(name.c_str());
    }

    Manager::player_ptr_t Manager::GetPlayer(const char* name) {
      auto entry = player_list_.find(name);
      if (entry != player_list_.end()) {
        return entry->second;
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(std::uint64_t host_token) {
      for (auto& [name, player] : player_list_) {
        if (player->NetData()->m_host_token == host_token) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(std::uint32_t msg_id) {
      for (auto& [name, player] : player_list_) {
        if (player->NetPlayer()->m_msg_id == msg_id) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(std::uint8_t id) {
      for (auto& [name, player] : player_list_) {
        if (player->Id() == id) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(CPed* ped) {
      for (auto& [name, player] : player_list_) {
        if (player->NetData()->m_peer_id == ped->m_player_info->m_net_player_data.m_peer_id) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(CPlayerInfo* info) {
      for (auto& [name, player] : player_list_) {
        if (player->NetData()->m_peer_id == info->m_net_player_data.m_peer_id) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(CNonPhysicalPlayerData* data) {
      for (auto& [name, player] : player_list_) {
        if (player->NonPhysicalPlayer()->m_player_id == data->m_player_id) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(rage::rlGamerInfo* data) {
      for (auto& [name, player] : player_list_) {
        if (player->NetData()->m_peer_id == data->m_peer_id) {
          return player;
        }
      }
      return nullptr;
    }

    Manager::player_ptr_t Manager::GetPlayer(CNetGamePlayer* net_player) {
      for (auto& [name, player] : player_list_) {
        if (*player == *net_player) {
          return player;
        }
      }
      return nullptr;
    }

    std::vector<Manager::player_ptr_t> Manager::GetPlayers(CVehicle* vehicle) {
      std::vector<player_ptr_t> players;
      for (auto& [name, player] : player_list_) {
        if (player->Vehicle()->m_driver == vehicle->m_driver) {
          players.push_back(player);
        }
      }
      return players;
    }

    std::vector<Manager::player_ptr_t> Manager::GetPlayers(ClanData* data) {
      std::vector<player_ptr_t> players;
      for (auto& [name, player] : player_list_) {
        if (player->Clan()->m_clan_id == data->m_clan_id) {
          players.push_back(player);
        }
      }
      return players;
    }

    std::size_t Manager::Size() const {
      return player_list_.size();
    }

    bool Manager::Empty() const {
      return player_list_.empty();
    }

    void Manager::SetSelectedPlayer(player_ptr_t player) {
      if (!player) {
        LOG_DEBUG("Manager::SetSelectedPlayer called with a nullptr");
        return;
      }

      selected_player_ = std::move(player);
    }

    Manager::player_ptr_t Manager::GetSelectedPlayer() {
      return selected_player_;
    }

    void Manager::Clear() {
      selected_player_ = std::make_shared<Player>(nullptr);
      player_list_.clear();
    }

    void Manager::Iterate(bool include_self, const std::function<void(const std::string&, const std::shared_ptr<Player>&)>& fn) {
      for (auto&& player_entry : player_list_) {
        if (!include_self && player_entry.second->Id() == self_->Id())
          continue;

        std::invoke(fn, player_entry.first, player_entry.second);
      }
    }

    Manager::player_list_t::iterator Manager::begin() noexcept {
      return player_list_.begin();
    }

    Manager::player_list_t::const_iterator Manager::cbegin() const noexcept {
      return player_list_.cbegin();
    }

    Manager::player_list_t::iterator Manager::end() noexcept {
      return player_list_.end();
    }

    Manager::player_list_t::const_iterator Manager::cend() const noexcept {
      return player_list_.cend();
    }

    Manager::player_list_t Manager::GetPlayerList() {
      return player_list_;
    }
  }