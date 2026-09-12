//
// Created by X-ray on 05/09/2023.
//

#ifndef GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#define GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#include <filesystem>
#include <system_error>
#include <xorstr.hpp>
#include "../globals.hpp"
#include "../logging/logging_macro.hpp"
#include "../win32/misc.hpp"

#define GET_PATH(path_name, path_to_dir) \
inline std::filesystem::path Get##path_name() { \
  std::filesystem::path dir = xorstr_(path_to_dir); \
  std::filesystem::create_directories(std::filesystem::absolute(dir));                     \
  return dir;                                                                               \
 }

namespace base::common::fs::vfs {
  /**
   * \brief Set the working directory to the appdata directory
   */
  inline void SetWorkingDir(const std::string& subcomponent) {
    auto app_path_res = win32::GetKnownFolderPath(win32::KNOWN_FOLDER_ID::kRoamingAppData);
    LOG_CRITICAL_CONDITIONAL(app_path_res.error(), "Failed to get appdata path: {}", app_path_res);

    const auto appdata_path = app_path_res.value() / globals::kBASE_NAME / subcomponent;

    std::filesystem::create_directories(appdata_path);
    std::filesystem::current_path(appdata_path);

    LOG_DEBUG("Set working directory to: {}", appdata_path);
  }

  inline std::filesystem::path GetWorkingDir() {
    return std::filesystem::current_path();
  }

  GET_PATH(LoggingDir, "logs")
  GET_PATH(LoggingSaveDir, "logs/saved")
  GET_PATH(ExceptionReportsDir, "logs/exception_reports")
  GET_PATH(PatternCacheDir, "cache/patterns")
  GET_PATH(TranslationDir, "translations")
  GET_PATH(SettingsDir, "settings")
  GET_PATH(TimeTraceDir, "logs/time_trace")
  GET_PATH(UIThemeDir, "settings/themes")
  GET_PATH(OptionSettingsDir, "settings/options")
  GET_PATH(ScriptsDir, "scripts")
  GET_PATH(ScriptLogsDir, "logs/scripts")
  GET_PATH(ScriptDataDir, "script_data")

  /**
   * \brief The folder one script's own logs go to: logs/scripts/<script name>, created if it is not there.
   */
  inline std::filesystem::path GetScriptLogDir(const std::string& script_name) {
    std::filesystem::path dir = GetScriptLogsDir() / script_name;
    std::filesystem::create_directories(std::filesystem::absolute(dir));
    return dir;
  }

  /**
   * \brief The folder one script's own data files go to: script_data/<script name>, created if it is not there.
   *
   * Every path a script names resolves inside its own folder, so two scripts cannot read or overwrite
   * each other's files, and a script that names a path outside it is refused.
   */
  inline std::filesystem::path GetScriptDataDir(const std::string& script_name) {
    std::filesystem::path dir = GetScriptDataDir() / script_name;
    std::filesystem::create_directories(std::filesystem::absolute(dir));
    return dir;
  }

  /**
   * Whether every element of `base` is the corresponding element of `target`, so that `target` is at
   * or below `base`.
   *
   * Written out rather than handed to std::equal: equal's four-iterator form requires both ranges to
   * be the same length and so would answer no for every file below the directory it was given, while
   * its three-iterator form reads the second path as far as the first goes, off the end of a target
   * that is shorter. Element-wise is also why this is not a string prefix test on the native form,
   * which would call "/tmp/ab" a path below "/tmp/a".
   */
  inline bool IsPathPrefix(const std::filesystem::path& base, const std::filesystem::path& target) {
    auto base_it = base.begin();
    const auto base_end = base.end();
    auto target_it = target.begin();
    const auto target_end = target.end();

    for (; base_it != base_end; ++base_it, ++target_it) {
      if (target_it == target_end || *base_it != *target_it) {
        return false;
      }
    }

    return true;
  }

  /**
   * Whether `target` is inside `base`, with both resolved first so that a ".." cannot be hidden in
   * one of them.
   *
   * `base` must exist - it is the directory being escaped from, and the answer for one that is not
   * there is no rather than a throw, since callers ask this from the render loop. `target` does not
   * have to exist, which is why it is only weakly canonicalised.
   */
  inline bool EnsureIsWithinDirectory(const std::filesystem::path& base, const std::filesystem::path& target) {
    std::error_code ec;
    const auto canonical_base = std::filesystem::canonical(base, ec);
    if (ec) {
      return false;
    }

    // Weakly canonical because the target is not required to exist.
    const auto canonical_target = std::filesystem::weakly_canonical(target, ec);
    if (ec) {
      return false;
    }

    return IsPathPrefix(canonical_base, canonical_target);
  }
}

#undef GET_PATH

#endif //GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
