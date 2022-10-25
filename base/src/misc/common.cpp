//
// Created by X-ray on 08/11/22.
//

#include "common.hpp"
#include "../memory/pointers.hpp"
#include <network/CNetworkPlayerMgr.hpp>

namespace gta_base::common {
  Platform GetCurrentPlatform() {
    static const Platform cur_platform = GetModuleHandleA("steam_api64.dll") ? Platform::kSteam : GetModuleHandleA("EOSSDK-Win64-Shipping.dll") ? Platform::kEpicGames : Platform::kRockstar;

    return cur_platform;
  }

  bool IsSessionStarted() {
    return (*memory::kPOINTERS->network_player_mgr_)->m_local_net_player != nullptr;
  }

  std::string VkToStr(std::uint64_t vk) {
    std::uint32_t scanCode = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);

    char szName[128];
    std::int32_t result{};
    switch (vk) {
      case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
      case VK_RCONTROL: case VK_RMENU:
      case VK_LWIN: case VK_RWIN: case VK_APPS:
      case VK_PRIOR: case VK_NEXT:
      case VK_END: case VK_HOME:
      case VK_INSERT: case VK_DELETE:
      case VK_DIVIDE:
      case VK_NUMLOCK:
        scanCode |= KF_EXTENDED;
      default:
        result = GetKeyNameTextA(scanCode << 16, szName, 128);
    }

    if(result == 0) {
      LOG_ERROR("Failed to convert vk key: {} to char. win32 err code: {}", vk, GetLastError());
      return {};
    }

    return szName;
  }

  std::string FloatPrecision(float num, std::size_t precision) {
    return fmt::format("{:.{}f}", num, precision);
  }

  std::filesystem::path GetBaseDir() {
    std::filesystem::path path = std::filesystem::path(std::getenv("APPDATA")) / globals::name;
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetLogDir() {
    std::filesystem::path path = GetBaseDir() / "logs";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetLogFile() {
    return GetLogDir() / fmt::format("{}.log", globals::name);
  }

  std::filesystem::path GetLogSaveDir() {
    std::filesystem::path path = GetLogDir() / "save";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetDataDir() {
    std::filesystem::path path = GetBaseDir() / "data";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetSettingsDir() {
    std::filesystem::path path = GetBaseDir() / "settings";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetThemesDir() {
    std::filesystem::path path = GetSettingsDir() / "themes";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetHotkeysDir() {
    std::filesystem::path path = GetDataDir() / "hotkeys";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetOptionsStateDir() {
    std::filesystem::path path = GetDataDir() / "options_state";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetTeleportsDir() {
    std::filesystem::path path = GetDataDir() / "teleports";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetTextureDir() {
    std::filesystem::path path = GetDataDir() / "textures";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::uint64_t GetEpoch() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  }

  HWND GetHwnd(const char* class_name, const char* window_name) {
    return FindWindowA(!strcmp(class_name, "") ? nullptr : class_name, !strcmp(window_name, "") ? nullptr : window_name);
  }

  HWND GetGameHwnd() {
    static const auto hwnd = GetHwnd(globals::target_window_class, globals::target_window_name);
    return hwnd;
  }

  bool IsForegroundWindow() {
    auto foreground_window = GetForegroundWindow();
    return foreground_window == GetConsoleWindow() || GetGameHwnd();
  }

  bool IsTargetProcess() {
    return GetGameHwnd() != nullptr;
  }

  robin_hood::unordered_map<std::uint32_t, KeyState> key_state{};
  bool IsKeyDown(std::uint32_t key) {
    auto key_entry = key_state.find(key);

    if (key_entry != key_state.end()) {
      return key_entry->second.down;
    }

    return false;
  }

  bool IsKeyJustReleased(std::uint32_t key, std::uint64_t since_up) {
    auto key_entry = key_state.find(key);

    if (key_entry != key_state.end()) {
      if (!key_entry->second.down) {
        return (GetEpoch() - key_entry->second.released_at) <= since_up;
      }
    }

    return false;
  }

  void SetKeyState(std::uint32_t key, bool down) {
    auto key_entry = key_state.find(key);
    if (key_entry != key_state.end()) {
      key_entry->second.down = down;
      if (!down)
        key_entry->second.released_at = GetEpoch();
    } else {
      key_state[key] = {down, 0};
    }
  }
}