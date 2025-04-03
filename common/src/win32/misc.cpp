//
// Created by xray on 06/09/2023.
//

#include "misc.hpp"
#include <ShlObj.h>
#include <unordered_map>
#include "../globals.hpp"

namespace base::menu::win32 {
  namespace {
    const std::unordered_map<KNOWN_FOLDER_ID, KNOWNFOLDERID> guid_map{
      {KNOWN_FOLDER_ID::kDocuments, FOLDERID_Documents},
      {KNOWN_FOLDER_ID::kFonts, FOLDERID_Fonts},
      {KNOWN_FOLDER_ID::kLocalAppData, FOLDERID_LocalAppData},
      {KNOWN_FOLDER_ID::kRoamingAppData, FOLDERID_RoamingAppData},
    };
  }

  StatusOr<std::filesystem::path> GetKnownFolderPath(const KNOWN_FOLDER_ID folder_id) {
    PWSTR path = nullptr;

    if (const GUID guid = guid_map.at(folder_id); SUCCEEDED(SHGetKnownFolderPath(guid, 0, nullptr, &path))) {
      std::filesystem::path res = path;
      CoTaskMemFree(path);
      return res;
    }

    LOG_ERROR("Failed to get known folder path. win32 err code: {}", GetLastError());
    return MakeFailure<ResultCode::kNOT_FOUND>("Unknown folder requested");
  }

  StatusOr<std::vector<MODULEENTRY32>> GetProcessModules(std::uint32_t pid) {
    std::vector<MODULEENTRY32> modules;
    MODULEENTRY32 mod_entry{};
    mod_entry.dwSize = sizeof(MODULEENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("PID is an invalid handle");
    }

    if (Module32First(hSnapshot, &mod_entry)) {
      do {
        modules.push_back(mod_entry);
      } while (Module32Next(hSnapshot, &mod_entry));
    }

    CloseHandle(hSnapshot);
    return modules;
  }

  StatusOr<HWND> GetHwnd(std::string_view window_class, std::string_view window_name) {
    auto hwnd = FindWindowA(window_class.empty() ? nullptr : window_class.data(), window_name.empty() ? nullptr : window_name.data());
    if (hwnd == INVALID_HANDLE_VALUE) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to retrieve valid hwnd handle for target window");
    }

    return hwnd;
  }

  StatusOr<HWND> GetGameHwnd() {
    static const auto hwnd = GetHwnd(common::globals::target_window_class, common::globals::target_window_name);
    return hwnd;
  }

  StatusOr<DWORD> GetPIDFromHWND(const HWND hwnd) {
    DWORD process_id = 0;
    GetWindowThreadProcessId(hwnd, &process_id);
    if (process_id == 0) {
      return MakeFailure<ResultCode::kINVALID_HANDLE>("Failed to retrieve valid pid from hwnd");
    }

    return process_id;
  }

  StatusOr<HANDLE> GetHProcFromHWND(const HWND hwnd) {
    DWORD process_id = 0;
    GetWindowThreadProcessId(hwnd, &process_id);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (hProc == INVALID_HANDLE_VALUE) {
      return MakeFailure<ResultCode::kINVALID_HANDLE>("Failed to open process handle");
    }

    return hProc;
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

    return cur_proc_name == common::globals::target_process_name;
  }
}
