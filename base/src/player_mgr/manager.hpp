//
// Created by X-ray on 09/08/22.
//

#pragma once
#ifndef GTA_BASE_PLAYER_MANAGER_HPP
#define GTA_BASE_PLAYER_MANAGER_HPP
#include <memory>
#include <utility>
//#include <robin_hood.h>
#include "player.hpp"
#include "../rage/util/get.hpp"

namespace gta_base {
  namespace player_mgr {
    class Manager {
    public:
      using player_ptr_t = std::shared_ptr<Player>;
      using player_list_t = robin_hood::unordered_map<std::string, player_ptr_t>;

    public:
      Manager();
      ~Manager();

      void SetSelf(CNetGamePlayer* self);
      [[nodiscard]] player_ptr_t GetSelf();

      void AddPlayer(CNetGamePlayer* player);
      void RemovePlayer(CNetGamePlayer* player);
      void RemovePlayer(const std::string& name);

      [[nodiscard]] player_ptr_t GetPlayer(const std::string& name);
      [[nodiscard]] player_ptr_t GetPlayer(const char* name);
      [[nodiscard]] player_ptr_t GetPlayer(std::uint64_t host_token);
      [[nodiscard]] player_ptr_t GetPlayer(std::uint32_t msg_id);
      [[nodiscard]] player_ptr_t GetPlayer(std::uint8_t id);
      [[nodiscard]] player_ptr_t GetPlayer(CPed* ped);
      [[nodiscard]] player_ptr_t GetPlayer(CPlayerInfo* info);
      [[nodiscard]] player_ptr_t GetPlayer(CNonPhysicalPlayerData* data);
      [[nodiscard]] player_ptr_t GetPlayer(rage::rlGamerInfo* data);
      [[nodiscard]] player_ptr_t GetPlayer(CNetGamePlayer* player);

      [[nodiscard]] std::vector<player_ptr_t> GetPlayers(CVehicle* vehicle);
      [[nodiscard]] std::vector<player_ptr_t> GetPlayers(ClanData* data);

      [[nodiscard]] std::size_t Size() const;
      [[nodiscard]] bool Empty() const;

      void SetSelectedPlayer(player_ptr_t player);
      [[nodiscard]] player_ptr_t GetSelectedPlayer();

      void Clear();

      [[nodiscard]] inline player_list_t::iterator begin() noexcept;
      [[nodiscard]] inline player_list_t::const_iterator cbegin() const noexcept;
      [[nodiscard]] inline player_list_t::iterator end() noexcept;
      [[nodiscard]] inline player_list_t::const_iterator cend() const noexcept;

    private:
      player_list_t player_list_;
      player_ptr_t self_;
      player_ptr_t selected_player_ = nullptr;
    };
  }
  inline player_mgr::Manager* kPLAYER_MGR{};
}

#endif //GTA_BASE_PLAYER_MANAGER_HPP
