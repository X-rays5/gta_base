//
// Created by X-ray on 02/10/23.
//

#pragma once
#ifndef GTA_BASE_FLAGS_0C9D077A3E2F4D4A83504D92A46F7FC5_HPP
#define GTA_BASE_FLAGS_0C9D077A3E2F4D4A83504D92A46F7FC5_HPP

namespace gta_base::ui::draw::flags {
  enum class OptionFlags {
    kNone,
    kCenterText,
    kSaveAble,
    kHotkeyAble,
    kSubmenuLink,
    kCantSelect,
    kDarkText,
  };
  GTA_BASE_ENUM_OPERATORS(OptionFlags);
}

#endif //GTA_BASE_FLAGS_0C9D077A3E2F4D4A83504D92A46F7FC5_HPP
