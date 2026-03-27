//
// Created by X-ray on 27/03/2026.
//

#pragma once

namespace base::menu::feature {
  using BoolSetting = std::atomic_bool;
  using Int8Setting = std::atomic_int8_t;
  using Int16Setting = std::atomic_int16_t;
  using Int32Setting = std::atomic_int32_t;
  using Int64Setting = std::atomic_int64_t;
  using UInt8Setting = std::atomic_uint8_t;
  using UInt16Setting = std::atomic_uint16_t;
  using UInt32Setting = std::atomic_uint32_t;
  using UInt64Setting = std::atomic_uint64_t;

  struct Health {
    BoolSetting god_mode = false;
    BoolSetting semi_god_mode = false;
  };

  struct Self {
    Health health;
    UInt8Setting wanted_level_force = 0;
    BoolSetting force_wanted_level = false;
    BoolSetting never_wanted = false;
  };

  struct SettingsData {
    Self self;
  };

}
