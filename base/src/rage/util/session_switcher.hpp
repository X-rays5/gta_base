//
// Created by X-ray on 09/18/22.
//

#pragma once
#ifndef GTA_BASE_SESSION_SWITCHER_HPP
#define GTA_BASE_SESSION_SWITCHER_HPP
#include <vector>
#include "../enums.hpp"

namespace rage::util {
struct SessionType {
  eSessionType id;
  const char name[22];
};

static std::vector<SessionType> session_types_list = {
    {eSessionType::JOIN_PUBLIC, "Public Session"},
    {eSessionType::NEW_PUBLIC, "New Public Session"},
    {eSessionType::CLOSED_CREW, "Closed Crew Session"},
    {eSessionType::CREW, "Crew Session"},
    {eSessionType::CLOSED_FRIENDS, "Closed Friend Session"},
    {eSessionType::FIND_FRIEND, "Find Friend Session"},
    {eSessionType::SOLO, "Solo Session"},
    {eSessionType::INVITE_ONLY, "Invite Only Session"},
    {eSessionType::JOIN_CREW, "Join Crew Session"},
    {eSessionType::SC_TV, "SC TV"},
};

/// @note This should be called in a fiber job
void SessionSwitcher(eSessionType session_type);
}

template<>
struct fmt::formatter<rage::util::SessionType> : formatter<std::string> {
  template<typename FormatContext>
  auto format(rage::util::SessionType session_type, FormatContext &ctx) const {
    string_view name = session_type.name;
    return formatter<string_view>::format(name, ctx);
  }
};
#endif //GTA_BASE_SESSION_SWITCHER_HPP
