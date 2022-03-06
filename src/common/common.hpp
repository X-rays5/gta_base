//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_COMMON_HPP
#define GTABASE_COMMON_HPP
#include <cstdlib>
#include <filesystem>
#include <string>
#include "globals.hpp"

#pragma warning(disable:4996)

namespace gta_base {
  namespace common {
    __forceinline std::filesystem::path GetBaseDir() {
      std::filesystem::path base_path = std::string(std::getenv("APPDATA")) + "/" + globals::name;
      if (!std::filesystem::exists(base_path)) {
        std::filesystem::create_directories(base_path);
      }
      return base_path;
    }

    __forceinline std::filesystem::path GetConfigDir() {
      std::filesystem::path config_path = GetBaseDir() / "config";
      if (!std::filesystem::exists(config_path)) {
        std::filesystem::create_directories(config_path);
      }
      return config_path;
    }

    __forceinline std::filesystem::path GetLogDir() {
      std::filesystem::path log_path = GetBaseDir() / "logs";
      if (!std::filesystem::exists(log_path)) {
        std::filesystem::create_directories(log_path);
      }
      return log_path;
    }

    __forceinline std::filesystem::path GetLogSaveDir() {
      std::filesystem::path log_path = GetLogDir() / "save";
      if (!std::filesystem::exists(log_path)) {
        std::filesystem::create_directories(log_path);
      }
      return log_path;
    }

    __forceinline bool KeyState(int key) {
      return GetAsyncKeyState(key) & 0x8000;
    }
  }
}

#endif //GTABASE_COMMON_HPP
