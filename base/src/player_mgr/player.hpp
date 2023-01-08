//
// Created by X-ray on 09/08/22.
//

#pragma once
#ifndef GTA_BASE_PLAYER_HPP
#define GTA_BASE_PLAYER_HPP
#include <algorithm>
#include <cctype>
#include <network/CNetGamePlayer.hpp>
#include <vehicle/CVehicle.hpp>
#include <ped/CPed.hpp>
#include <network/snSession.hpp>

namespace gta_base::player_mgr {
  class Player {
  public:
    explicit Player(CNetGamePlayer* player) : self_(player) {}

    [[nodiscard]] CNetGamePlayer* NetPlayer();
    [[nodiscard]] CNonPhysicalPlayerData* NonPhysicalPlayer();
    [[nodiscard]] rage::rlGamerInfo* NetData();
    [[nodiscard]] CPlayerInfo* Info();
    [[nodiscard]] CPed* Ped();
    [[nodiscard]] CVehicle* Vehicle();
    [[nodiscard]] ClanData* Clan();
    [[nodiscard]] rage::snPlayer* SessionPlayer();
    [[nodiscard]] rage::snPeer* SessionPeer();

    [[nodiscard]] std::string Name();
    [[nodiscard]] std::string NameLowerCase();

    [[nodiscard]] std::uint8_t Id() const;
    [[nodiscard]] bool Host() const;
    [[nodiscard]] bool Valid() const;

    bool operator==(CNetGamePlayer* other) const;
    bool operator==(CNetGamePlayer& other) const;
    bool operator==(const CNetGamePlayer& other) const;
    bool operator!=(CNetGamePlayer* other) const;
    bool operator!=(CNetGamePlayer& other) const;
    bool operator!=(const CNetGamePlayer& other) const;
    bool operator==(Player* other) const;
    bool operator==(Player& other) const;
    bool operator==(const Player& other) const;
    bool operator!=(Player* other) const;
    bool operator!=(Player& other) const;
    bool operator!=(const Player& other) const;

  private:
    CNetGamePlayer* self_;
  };
}

#endif //GTA_BASE_PLAYER_HPP
