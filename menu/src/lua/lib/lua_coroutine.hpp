//
// Created by X-ray on 04/04/2026.
//

#pragma once
#pragma warning(push)
#pragma warning(disable : 5321)
#include <sol/sol.hpp>
#pragma warning(pop)

namespace base::menu::lua {
  sol::table SetupCoroutine(sol::state& lua);
}
