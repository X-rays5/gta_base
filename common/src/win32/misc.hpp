//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
#define GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
#include <filesystem>
#include <TlHelp32.h>

namespace base::menu::win32 {
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

 /**
  * \brief Get a path from a KNOWN_FOLDER_ID enum value
  * \param folder_id A KNOWN_FOLDER_ID enum value
  * \return std::filesystem::path to the folder
  */
 StatusOr<std::filesystem::path> GetKnownFolderPath(KNOWN_FOLDER_ID folder_id);

 /**
  * \brief Get a vector of MODULEENTRY32 structs for a process
  * \param pid The pid of the process
  * \return A vector of MODULEENTRY32 structs
  */
 StatusOr<std::vector<MODULEENTRY32>> GetProcessModules(std::uint32_t pid);

 /**
  * \brief Get a HWND from a window class and name
  * \param window_class window_class
  * \param window_name window_name
  * \return HWND
  */
 StatusOr<HWND> GetHwnd(std::string_view window_class, std::string_view window_name);
 /**
  * \brief Get the HWND of the game window
  * \return HWND
  */
 StatusOr<HWND> GetGameHwnd();

 /**
  * \brief Check if the game window is the foreground window
  * \return bool
  */
 bool IsForegroundWindow();
 /**
  * \brief Check if the current process is the target process
  * \return bool
  */
 bool IsTargetProcess();
}
#endif //GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
