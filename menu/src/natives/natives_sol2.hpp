#pragma once

#pragma warning(push)
#pragma warning(disable: 4100)

// Auto-generated file - DO NOT EDIT
// Sol2 native registration functions

#pragma warning(push)
#pragma warning(disable: 5321)
#include <sol/sol.hpp>
#pragma warning(pop)

namespace base::menu::natives {

	/**
	 * Register all natives to a sol2 lua state.
	 * 
	 * @param lua The sol::state to register natives into
	 * @return A sol::table containing all registered native groups
	 */
	sol::table register_natives(sol::state& lua);

} // namespace base::menu::natives

#pragma warning(pop)
