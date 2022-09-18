//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_GLOBAL_IDX_HPP
#define GTA_BASE_GLOBAL_IDX_HPP
#include <cstdint>

namespace rage {
  enum class GlobalIdx : std::size_t {
    kSpectatorMode = 2726795,
    kSessionJoinerType = 1575015,
    kSessionJoinerActive = 1574589,
    kTransitionState = 1574991,
    kTunables = 262145,
    kSnow = 4751, // Global_kTunables.f_kSnow bool
    kRequestServices = 2810701,
    kRequestServicesMoc = 913, // bool
    kRequestServicesAvenger = 921, // bool
    kRequestServicesTerrorbyte = 925, // bool
    kRequestServicesKosatka = 933, // bool
    kRequestServicesDingy = 945, // bool
    kRequestServicesBallisticArmor = 884, // bool
    kBallisticArmorRequestPrice = 20083, // Global_kTunables.f_kBallisticArmorRequestPrice
    kIdleKick1 = 87, // set to INT_MAX
    kIdleKick2 = 88, // set to INT_MAX
    kIdleKick3 = 89, // set to INT_MAX
    kIdleKickKick = 90, // set to INT_MAX
    kIdleConstrainedKick1 = 8248, // set to INT_MAX
    kIdleConstrainedKick2 = 8249, // set to INT_MAX
    kIdleConstrainedKick3 = 8250, // set to INT_MAX
    kIdleConstrainedKickKick = 8251, // set to INT_MAX
  };
}
#endif //GTA_BASE_GLOBAL_IDX_HPP
