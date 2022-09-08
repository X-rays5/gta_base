//
// Created by X-ray on 08/28/22.
//

#pragma once
#ifndef GTA_BASE_GET_HPP
#define GTA_BASE_GET_HPP
#include <ped/CPedFactory.hpp>
#include <network/CNetGamePlayer.hpp>
#include <network/CNetworkPlayerMgr.hpp>
#include <network/Network.hpp>
#include "../../memory/pointers.hpp"
#include "../../misc/common.hpp"

namespace rage {
  inline CPed* GetLocalPed() {
    if (auto ped_factory = *gta_base::memory::kPOINTERS->ped_factory_) {
      return ped_factory->m_local_ped;
    }

    return nullptr;
  }

  inline CPlayerInfo* GetLocalPlayerInfo() {
    if (auto ped = GetLocalPed()) {
      return ped->m_player_info;
    }

    return nullptr;
  }

  inline rlGamerInfo* GetLocalGamerInfo() {
    if (auto info = GetLocalPlayerInfo()) {
      return &info->m_net_player_data;
    }

    return nullptr;
  }

  inline CNetworkPlayerMgr* GetNetworkPlayerMgr() {
    return *gta_base::memory::kPOINTERS->network_player_mgr_;
  }

  inline ::CNetGamePlayer* GetLocalNetGamePlayer() {
    if (auto player_mgr = GetNetworkPlayerMgr()) {
      return reinterpret_cast<::CNetGamePlayer*>(player_mgr->m_local_net_player);
    }

    return nullptr;
  }

  inline Network* GetNetwork() {
    auto network = (std::int64_t)*gta_base::memory::kPOINTERS->network_;
    if (gta_base::common::GetCurrentPlatform() == gta_base::common::Platform::kSteam)
      network += sizeof(rage::rlSessionInfo);

    return (Network*)network;
  }
}
#endif //GTA_BASE_GET_HPP
