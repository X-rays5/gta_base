//
// Created by xray on 06/09/2023.
//

#include "misc.hpp"
#include <ShlObj.h>
#include <robin_hood.h>

namespace base::win32 {
  namespace {
    const robin_hood::unordered_map <KNOWN_FOLDER_ID, KNOWNFOLDERID> guid_map{
    {KNOWN_FOLDER_ID::kDocuments,      FOLDERID_Documents},
    {KNOWN_FOLDER_ID::kFonts,          FOLDERID_Fonts},
    {KNOWN_FOLDER_ID::kLocalAppData,   FOLDERID_LocalAppData},
    {KNOWN_FOLDER_ID::kRoamingAppData, FOLDERID_RoamingAppData},
    };
  }

  absl::StatusOr<std::filesystem::path> GetKnownFolderPath(KNOWN_FOLDER_ID folder_id) {
    PWSTR path = nullptr;
    const GUID guid = guid_map.at(folder_id);

    if (SUCCEEDED(SHGetKnownFolderPath(guid, 0, nullptr, &path))) {
      std::filesystem::path res = path;
      CoTaskMemFree(path);
      return res;
    } else {
      LOG_ERROR("Failed to get known folder path. win32 err code: {}", GetLastError());
      return absl::NotFoundError("Unknown folder requested");
    }
  }

  absl::StatusOr<std::vector<MODULEENTRY32>> GetProcessModules(std::uint32_t pid) {
    std::vector<MODULEENTRY32> modules;
    MODULEENTRY32 mod_entry{};
    mod_entry.dwSize = sizeof(MODULEENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
      return absl::InvalidArgumentError("PID is an invalid handle");
    }

    if (Module32First(hSnapshot, &mod_entry)) {
      do {
        modules.push_back(mod_entry);
      } while (Module32Next(hSnapshot, &mod_entry));
    }

    CloseHandle(hSnapshot);
    return modules;
  }

  absl::StatusOr<HWND> GetHwnd(std::string_view window_class, std::string_view window_name) {
    return FindWindowA(window_class == "" ? nullptr : window_class.data(), window_name == "" ? nullptr : window_name.data());
  }

  absl::StatusOr<HWND> GetGameHwnd() {
    static const auto hwnd = GetHwnd(globals::target_window_class, globals::target_window_name);
    return hwnd;
  }

  bool IsForegroundWindow() {
    auto foreground_window = GetForegroundWindow();
    return foreground_window == GetConsoleWindow() || GetGameHwnd().value();
  }

  bool IsTargetProcess() {
    auto exe_name = std::make_unique<CHAR[]>(MAX_PATH);
    std::uint32_t str_len = GetModuleFileNameA(nullptr, exe_name.get(), MAX_PATH);

    if (str_len == 0) {
      LOG_ERROR("Failed to get module file name. win32 err code: {}", GetLastError());
      return false;
    }

    std::string cur_proc_name = std::filesystem::path(std::string(exe_name.get(), str_len)).filename().string();

    return cur_proc_name == globals::target_process_name;
  }
}