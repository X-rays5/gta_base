//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_GLOBAL_IDX_HPP
#define GTA_BASE_GLOBAL_IDX_HPP

namespace rage {
namespace Globals {
namespace SessionSwitcher {
enum Globals : std::size_t {
  kSpectatorMode = 2695915,
  kSessionJoinerType = 1575017,
  kSessionJoinerActive = 1574589,
};
}
namespace Tunables {
enum Globals : std::size_t {
  kTunables = 262145,
  kSnow = 4751,
  kBallisticArmorRequestPrice = 20083,
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
namespace RequestServices {
enum Globals : std::size_t {
  kMain = 2810701,
  kMoc = 913, // bool
  kAvenger = 921, // bool
  kTerrorbyte = 925, // bool
  kKosatka = 933, // bool
  kDingy = 945, // bool
  kBallisticArmor = 884, // bool
};
}
namespace Misc {
enum Globals : std::size_t {
  kTransitionState = 1574993,
};
}
}
}
#endif //GTA_BASE_GLOBAL_IDX_HPP
