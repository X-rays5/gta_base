//
// Created by X-ray on 08/11/22.
//

#include "common.hpp"

namespace gta_base {
  namespace common {
    std::filesystem::path GetBaseDir() {
      std::filesystem::path base_path = std::filesystem::path(std::getenv("APPDATA")) / globals::name;
      if (!std::filesystem::exists(base_path)) {
        std::filesystem::create_directories(base_path);
      }
      return base_path;
    }

    std::filesystem::path GetConfigDir() {
      std::filesystem::path config_path = GetBaseDir() / "config";
      if (!std::filesystem::exists(config_path)) {
        std::filesystem::create_directories(config_path);
      }
      return config_path;
    }

    std::filesystem::path GetLogDir() {
      std::filesystem::path log_path = GetBaseDir() / "logs";
      if (!std::filesystem::exists(log_path)) {
        std::filesystem::create_directories(log_path);
      }
      return log_path;
    }

    std::filesystem::path GetLogFile() {
      return GetLogDir() / fmt::format("{}.log", globals::name);
    }

    std::filesystem::path GetLogSaveDir() {
      std::filesystem::path log_path = GetLogDir() / "save";
      if (!std::filesystem::exists(log_path)) {
        std::filesystem::create_directories(log_path);
      }
      return log_path;
    }

    std::filesystem::path GetDataDir() {
      std::filesystem::path data_path = GetBaseDir() / "data";
      if (!std::filesystem::exists(data_path)) {
        std::filesystem::create_directories(data_path);
      }
      return data_path;
    }

    std::filesystem::path GetTextureDir() {
      std::filesystem::path texture_path = GetDataDir() / "textures";
      if (!std::filesystem::exists(texture_path)) {
        std::filesystem::create_directories(texture_path);
      }
      return texture_path;
    }

    std::uint64_t GetEpoch() {
      return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    HWND GetHwnd(const char* class_name, const char* window_name) {
      return FindWindowA(!strcmp(class_name, "") ? nullptr : class_name, !strcmp(window_name, "") ? nullptr : window_name);
    }

    bool IsForegroundWindow() {
      auto foreground_window = GetForegroundWindow();
      return foreground_window == GetConsoleWindow() || foreground_window == GetHwnd(common::globals::target_window_class, common::globals::target_window_name);
    }

    bool IsTargetProcess() {
      return GetHwnd(globals::target_window_class, globals::target_window_name) != nullptr;
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
        key_state[key] = {down, GetEpoch()};
      }
    }
  }
}