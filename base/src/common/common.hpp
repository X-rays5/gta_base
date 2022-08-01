//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_COMMON_HPP
#define GTABASE_COMMON_HPP
#include <cstdlib>
#include <filesystem>
#include <string>
#include <fmt/format.h>
#include "globals.hpp"

#pragma warning(disable:4996)

/// NOTE: Everything in this file has to be __forceinline otherwise the linker will get mad.
/// This could be fixed by moving the implementation to a .cpp file, but this also works fine for now.

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

    __forceinline std::filesystem::path GetLogFile() {
      return GetLogDir() / fmt::format("{}.log", globals::name);
    }

    __forceinline std::filesystem::path GetLogSaveDir() {
      std::filesystem::path log_path = GetLogDir() / "save";
      if (!std::filesystem::exists(log_path)) {
        std::filesystem::create_directories(log_path);
      }
      return log_path;
    }

    __forceinline std::filesystem::path GetDataDir() {
      std::filesystem::path data_path = GetBaseDir() / "data";
      if (!std::filesystem::exists(data_path)) {
        std::filesystem::create_directories(data_path);
      }
      return data_path;
    }

    __forceinline std::filesystem::path GetTextureDir() {
      std::filesystem::path texture_path = GetDataDir() / "textures";
      if (!std::filesystem::exists(texture_path)) {
        std::filesystem::create_directories(texture_path);
      }
      return texture_path;
    }

    __forceinline bool KeyState(int key) {
      return GetAsyncKeyState(key) & 0x8000;
    }

    __forceinline HWND GetHwnd(const char* class_name, const char* window_name) {
      return FindWindowA(!strcmp(class_name, "") ? nullptr : class_name, !strcmp(window_name, "") ? nullptr : window_name);
    }

    __forceinline bool IsTargetProcess() {
      return GetHwnd(globals::target_window_class, globals::target_window_name) != nullptr;
    }
  }
}

#endif //GTABASE_COMMON_HPP
