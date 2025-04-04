//
// Created by X-ray on 05/09/2023.
//

#ifndef GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#define GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#include <filesystem>
#include <xorstr.hpp>
#include "globals.hpp"
#include "logging/logging_macro.hpp"
#include "win32/misc.hpp"

#define GET_PATH(path_name, path_to_dir)                                        \
inline std::filesystem::path Get##path_name() {                                 \
  std::filesystem::path dir = std::filesystem::absolute(xorstr_(path_to_dir));  \
  if (!std::filesystem::is_directory(dir))                                       \
    std::filesystem::create_directories(dir);                                   \
                                                                                \
  return dir;                                                                   \
 }

namespace base::common::vfs {
  /**
   * \brief Set the working directory to the appdata directory
   */
  inline void SetWorkingDir(const std::string& subcomponent) {
    auto app_path_res = win32::GetKnownFolderPath(win32::KNOWN_FOLDER_ID::kRoamingAppData);
    if (app_path_res.error()) {
      MessageBoxA(nullptr, app_path_res.error().GetResultMessage().c_str(), fmt::format("Failed to get appdata path: {}", app_path_res).c_str(), MB_OK | MB_ICONERROR);
      std::exit(EXIT_FAILURE);
    }

    const auto appdata_path = app_path_res.value() / common::globals::kBASE_NAME / subcomponent;

    std::filesystem::create_directories(appdata_path);
    std::filesystem::current_path(appdata_path);
  }

  GET_PATH(LoggingDir, "logs")
  GET_PATH(LoggingSaveDir, "logs/saved")
  GET_PATH(TimeTraceDir, "logs/time_trace")
  GET_PATH(ExceptionReports, "logs/exception_reports")
  GET_PATH(PatternCacheDir, "cache/patterns")
  GET_PATH(TranslationDir, "translations")
  GET_PATH(SettingsDir, "settings")
}

#undef GET_PATH

#endif //GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
