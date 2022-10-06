
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
#include <algorithm>
#include <limits>
#include <fmt/format.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include "../logger/logger.hpp"
#include "globals.hpp"

#define GTA_BASE_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#pragma warning(disable:4996)

namespace gta_base::common {
    enum class Platform {
      kRockstar,
      kEpicGames,
      kSteam
    };

    Platform GetCurrentPlatform();

    bool IsSessionStarted();

    template<typename T>
    inline T LexicalCast(const std::string& str) {
      if (str.empty())
        return {};

      T res;
      std::istringstream iss;
      iss.str(str);
      iss >> res;
      return res;
    }

    template<typename in_t, typename ret_t>
    inline ret_t WithinLimits(in_t convert) {
      constexpr static const in_t min = std::numeric_limits<ret_t>::min();
      constexpr static const in_t max = std::numeric_limits<ret_t>::max();

      return static_cast<ret_t>(std::clamp(convert, min, max));
    }

    std::string FloatPrecision(float num, std::size_t precision = 3);

    std::filesystem::path GetBaseDir();
    std::filesystem::path GetConfigDir();
    std::filesystem::path GetLogDir();
    std::filesystem::path GetLogFile();
    std::filesystem::path GetLogSaveDir();
    std::filesystem::path GetDataDir();
    std::filesystem::path GetSettingsDir();
    std::filesystem::path GetTeleportsDir();
    std::filesystem::path GetTextureDir();

    std::uint64_t GetEpoch();

    HWND GetHwnd(const char* class_name, const char* window_name);
    HWND GetGameHwnd();
    bool IsForegroundWindow();
    bool IsTargetProcess();

    bool IsKeyDown(std::uint32_t key);
    bool IsKeyJustReleased(std::uint32_t key, std::uint64_t since_up = 100);
    void SetKeyState(std::uint32_t key, bool down);
  }

#endif //GTABASE_COMMON_HPP
