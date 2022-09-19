//
// Created by X-ray on 09/18/22.
//

#include "session_switcher.hpp"
#include "../script/global.hpp"
#include "../../fiber/script.hpp"

namespace rage {
  namespace util {
    void SessionSwitcher(eSessionType session_type) {
      script::Global(Globals::SessionSwitcher::kSpectatorMode).As<int>() = (session_type == eSessionType::SC_TV ? 1 : 0); // If SC TV Then Enable Spectator Mode

      if (session_type == eSessionType::LEAVE_ONLINE)
        script::Global(Globals::SessionSwitcher::kSessionJoinerActive).At(2).As<int>() = -1;
      else
        script::Global(Globals::SessionSwitcher::kSessionJoinerType).As<int>() = (int)session_type;

      script::Global(Globals::SessionSwitcher::kSessionJoinerActive).As<int>() = 1;
      gta_base::fiber::Script::GetCurr()->Yield(200);
      script::Global(Globals::SessionSwitcher::kSessionJoinerActive).As<int>() = 0;
    }
  }
}