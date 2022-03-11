//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_ENUMS_HPP
#define GTA_BASE_ENUMS_HPP
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define _ENUMS_HAS_FLAG(flag, bit) ((flag & bit) == flag)

namespace ui {
  namespace components {
    enum class Submenus {
      Home,
      Player,
      Settings,
      UnloadConfirm,
    };

    enum class OptionFlag {
      kNone = 0x00,
      kDisabled = 0x01,
      kRightIcon = 0x02,
      kRightText = 0x04,
    };
    DEFINE_ENUM_FLAG_OPERATORS(OptionFlag)

    enum class KeyInput {
      kUp,
      kDown,
      kLeft,
      kRight,
      kReturn,
      kBackspace,
    };
  }// namespace components
}// namespace ui
#endif //GTA_BASE_ENUMS_HPP
