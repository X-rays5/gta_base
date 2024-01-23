//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
#define GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
#include <filesystem>

namespace base::win32 {
  enum class KNOWN_FOLDER_ID : uint32_t {
    /// @note %USERPROFILE%/Documents
    kDocuments,
    /// @note %windir%/Fonts
    kFonts,
    /// @note %LOCALAPPDATA%
    kLocalAppData,
    /// @note %APPDATA%
    kRoamingAppData,
  };

  StatusOr<std::filesystem::path> GetKnownFolderPath(KNOWN_FOLDER_ID folder_id);

  StatusOr<std::vector<MODULEENTRY32>> GetProcessModules(std::uint32_t pid);

  /// @note Leave window_class or window_name empty to ignore them
  StatusOr<HWND> GetHwnd(std::string_view window_class, std::string_view window_name);
  StatusOr<HWND> GetGameHwnd();

  bool IsForegroundWindow();
  bool IsTargetProcess();
}
#endif //GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
