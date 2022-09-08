//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_COMMON_HPP
#define GTABASE_COMMON_HPP
#include <cstdlib>
#include <filesystem>
#include <string>
#include <chrono>
#include <fmt/format.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../logger/logger.hpp"
#include "globals.hpp"

#pragma warning(disable:4996)

namespace gta_base {
  namespace common {
    enum class Platform {
      kRockstar,
      kEpicGames,
      kSteam
    };

    Platform GetCurrentPlatform();

    bool IsSessionStarted();

    std::filesystem::path GetBaseDir();
    std::filesystem::path GetConfigDir();
    std::filesystem::path GetLogDir();
    std::filesystem::path GetLogFile();
    std::filesystem::path GetLogSaveDir();
    std::filesystem::path GetDataDir();
    std::filesystem::path GetTextureDir();

    std::uint64_t GetEpoch();

    HWND GetHwnd(const char* class_name, const char* window_name);
    bool IsForegroundWindow();
    bool IsTargetProcess();

    bool IsKeyDown(std::uint32_t key);
    bool IsKeyJustReleased(std::uint32_t key, std::uint64_t since_up = 100);
    void SetKeyState(std::uint32_t key, bool down);
  }
}

#endif //GTABASE_COMMON_HPP
