//
// Created by X-ray on 09/08/22.
//

#pragma once
#ifndef GTA_BASE_PLAYER_HPP
#define GTA_BASE_PLAYER_HPP
#include <algorithm>
#include <ctype.h>
#include <network/CNetGamePlayer.hpp>
#include <vehicle/CVehicle.hpp>
#include <ped/CPed.hpp>

namespace gta_base {
  namespace player_mgr {
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

      template<typename str_t = const char*>
      [[nodiscard]] const char* Name() {
        return (str_t)self_->get_name();
      }

      std::string NameLowerCase() {
        std::string res = Name();
        std::transform(res.begin(), res.end(), res.begin(), ::tolower);

        return res;
      }

      [[nodiscard]] std::uint8_t Id() const;
      [[nodiscard]] bool Host() const;
      [[nodiscard]] bool Valid() const;

      bool operator==(CNetGamePlayer& other) const;

    private:
      CNetGamePlayer* self_;
    };
  }
}

#endif //GTA_BASE_PLAYER_HPP
