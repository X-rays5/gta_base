//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#define GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#include <filesystem>

#define GET_PATH_FN(path_name, path_to_dir) FORCE_INLINE std::filesystem::path Get##path_name() { \
  std::filesystem::path dir = #path_to_dir; \
  std::filesystem::create_directories(std::filesystem::absolute(dir));                     \
  return dir;                                                                               \
 }

namespace base::util::vfs {
  void SetWorkingDir() {
    // TODO: use the win32 api to get directories like this
    auto appdata_path = std::filesystem::path(std::getenv("APPDATA")) / "base";
    std::filesystem::create_directories(appdata_path);
    std::filesystem::current_path(appdata_path);
  }
  
  GET_PATH_FN(LoggingDir, logs)
  GET_PATH_FN(LoggingSaveDir, logs / saved)
}

#undef GET_PATH_FN

#endif //GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
