
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
#include <d3d11.h>
#include <TlHelp32.h>
#include <Shlobj.h>
#include <Zydis/Zydis.h>

#ifdef LoadImage
#undef LoadImage
#endif
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

  std::string AddrToHex(uint64_t addr);

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
    constexpr static const ret_t min = std::numeric_limits<ret_t>::min();
    constexpr static const ret_t max = std::numeric_limits<ret_t>::max();

    if (convert < min)
      return min;
    else if (convert > max)
      return max;
    else
      return static_cast<ret_t>(convert);
  }

  std::vector<std::string> SplitStr(const std::string& str, const std::string& delim);

  std::int64_t HexToIntFast(char number);
  inline std::optional<std::int64_t> HexToInt(char number) {
    if (number == '0')
      return 0;

    auto res = HexToIntFast(number);
    if (res == 0)
      return std::nullopt;

    return res;
  }

  struct ZydisInstruction {
    ZydisDecodedInstruction instruction;
    ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
  };

  ZydisInstruction GetInstructionAtAddr(std::uintptr_t addr);
  std::string GetInstructionStr(std::uintptr_t addr, const ZydisInstruction& instruction);
  std::string GetInstructionStr(std::uintptr_t addr);

  std::string GetFileMd5Hash(const std::filesystem::path& file_path);

  std::string RemoveForbiddenFilenameChars(const std::filesystem::path& path);

  std::filesystem::path PidToPath(std::size_t pid);

  std::string StripVarName(const std::string& str);

  std::string RemoveNonNumerical(std::string str);

  std::string VkToStr(std::uint64_t vk);

  std::string FloatPrecision(float num, std::size_t precision = 3);

  std::filesystem::path GetKnownFolderPath(const KNOWNFOLDERID& folder_id);

  std::string ProperCapitalization(const std::string& str);

  std::filesystem::path GetBaseDir();
  std::filesystem::path GetTranslationDir();
  std::filesystem::path GetLogDir();
  std::filesystem::path GetLogFile();
  std::filesystem::path GetLogSaveDir();
  std::filesystem::path GetDataDir();
  std::filesystem::path GetCachedDir();
  std::filesystem::path GetGtaDataCacheDir();
  std::filesystem::path GetCachedPatternsDir();
  std::filesystem::path GetSettingsDir();
  std::filesystem::path GetThemesDir();
  std::filesystem::path GetHotkeysDir();
  std::filesystem::path GetOptionsStateDir();
  std::filesystem::path GetTeleportsDir();
  std::filesystem::path GetTextureDir();
  std::filesystem::path GetScriptsDir();
  std::filesystem::path GetScriptsDataDir();

  std::uint64_t GetEpoch();

  HWND GetHwnd(const char* class_name, const char* window_name);
  HWND GetGameHwnd();
  bool IsForegroundWindow();
  bool IsTargetProcess();

  MODULEENTRY32 GetModuleFromHModule(HMODULE mod);
  std::uint64_t GetModuleBaseAddress(std::uint32_t pid, const std::string& mod_name);
  MODULEENTRY32 GetModuleFromAddress(std::uint32_t pid, std::uint64_t addr);
  inline std::string GetModuleNameFromAddress(std::uint32_t pid, std::uint64_t addr) {
    return GetModuleFromAddress(pid, addr).szModule;
  }

  bool IsKeyDown(std::uint32_t key);
  bool IsKeyJustReleased(std::uint32_t key, std::uint64_t since_up = 100);
  void SetKeyState(std::uint32_t key, bool down);

  struct ImageTexture {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    ID3D11ShaderResourceView* texture = nullptr;

    operator ID3D11ShaderResourceView*() const {
      return texture;
    }

    operator bool() {
      return texture != nullptr;
    }

    operator void*() = delete;
    operator void*() const {
      return texture;
    }
  };

  bool LoadImage(const std::filesystem::path& file, ImageTexture* out_texture);
}

#endif //GTABASE_COMMON_HPP
