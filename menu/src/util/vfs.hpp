//
// Created by X-ray on 05/09/2023.
//

#ifndef GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#define GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#include <filesystem>

#define GET_PATH(path_name, path_to_dir) FORCE_INLINE std::filesystem::path Get##path_name() { \
  std::filesystem::path dir = xorstr_(path_to_dir); \
  std::filesystem::create_directories(std::filesystem::absolute(dir));                     \
  return dir;                                                                               \
 }

namespace base::util::vfs {
  inline void SetWorkingDir() {
    auto app_path_res = win32::GetKnownFolderPath(win32::KNOWN_FOLDER_ID::kRoamingAppData);
    if (!app_path_res.ok()) {
      LOG_CRITICAL("Failed to get appdata path: {}", app_path_res.status().ToString());
      abort();
    }

    const auto appdata_path = app_path_res.value() / globals::kBASE_NAME;

    std::filesystem::create_directories(appdata_path);
    std::filesystem::current_path(appdata_path);
  }

  GET_PATH(LoggingDir, "logs")
  GET_PATH(LoggingSaveDir, "logs/saved")
  GET_PATH(PatternCacheDir, "cache/patterns")
}

#undef GET_PATH

#endif //GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
