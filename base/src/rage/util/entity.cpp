//
// Created by X-ray on 09/18/22.
//

#include "entity.hpp"
#include "../../fiber/script.hpp"
#include "../../natives/natives.hpp"

namespace rage {
  namespace util {
    bool TakeControlOfEntity(Entity ent) {
      if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent)) return true;
      for (uint8_t i = 0; !NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent) && i < 10; i++)
      {
        NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(ent);

        gta_base::fiber::Script::GetCurr()->Yield(5);
      }
      if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent)) return false;

      int netHandle = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(ent);
      NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netHandle, true);

      return true;
    }
  }
}