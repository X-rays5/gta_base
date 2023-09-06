//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
#define GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
#include <filesystem>

namespace base::win32 {
  enum class KNOWN_FOLDER_ID : uint32_t {
    kDocuments, /// %USERPROFILE%/Documents
    kFonts, /// %windir%/Fonts
    kLocalAppData, /// %LOCALAPPDATA%
    kRoamingAppData, /// %APPDATA%
  };

  std::filesystem::path GetKnownFolderPath(KNOWN_FOLDER_ID folder_id);

  std::vector<MODULEENTRY32> GetProcessModules(std::uint32_t pid);
}
#endif //GTA_BASE_MISC_B86A3FB180F14F93A1ACF015A6B7E4C6_HPP
