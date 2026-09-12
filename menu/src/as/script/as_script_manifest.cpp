//
// Created by X-ray on 11/09/2026.
//

#include "as_script_manifest.hpp"
#include <glaze/toml.hpp>

#include <system_error>

namespace base::menu::as::script {
  namespace {
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
    bool IsPathPrefix(const std::filesystem::path& base, const std::filesystem::path& target) {
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
     * Neither path has to exist. A manifest whose main file or license was deleted asks a question
     * about a path that is not there, and the answer is no - `canonical` would throw instead, out of
     * the menu's render loop, which reads these every frame.
     */
    bool EnsureIsWithinDirectory(const std::filesystem::path& base, const std::filesystem::path& target) {
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

  const std::string ScriptManifest::meta_filename_ = "manifest.toml";

  ScriptManifest::ScriptManifest(const std::filesystem::path& path) : path_(path) {}

  Status ScriptManifest::ReadMetaData() {
    if (!std::filesystem::is_directory(path_)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Path is not a directory: {}", path_);
    }

    const auto manifest_file = path_ / meta_filename_;
    if (!std::filesystem::is_regular_file(manifest_file)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Manifest file not found: {}", manifest_file);
    }

    auto ec = glz::read_file_toml(data_, manifest_file.string(), std::string{});
    if (ec) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Failed to read manifest file: {}", ec);
    }

    LOG_DEBUG("Successfully read script metadata from manifest: {}", manifest_file);
    return {};
  }

  std::filesystem::path ScriptManifest::GetMainFile() const {
    const auto main_file_path = path_ / data_.main_file;
    if (!EnsureIsWithinDirectory(path_, main_file_path)) {
      LOG_ERROR("Main file is outside of script directory: {}", main_file_path);
      return {};
    }

    if (!std::filesystem::is_regular_file(main_file_path)) {
      LOG_ERROR("Main file does not exist: {}", main_file_path);
      return {};
    }

    return main_file_path;
  }

  std::filesystem::path ScriptManifest::GetPath() const {
    return path_;
  }

  std::string ScriptManifest::GetName() const {
    return data_.name;
  }

  std::optional<std::string> ScriptManifest::GetDescription() const {
    return data_.description;
  }

  std::vector<std::string> ScriptManifest::GetAuthor() const {
    return data_.authors;
  }

  std::optional<std::string> ScriptManifest::GetVersion() const {
    return data_.version;
  }

  std::optional<std::string> ScriptManifest::GetRepository() const {
    return data_.repository;
  }

  std::optional<std::filesystem::path> ScriptManifest::GetLicense() const {
    if (data_.license.has_value()) {
      const auto license_path = path_ / data_.license.value();
      if (!EnsureIsWithinDirectory(path_, license_path)) {
        LOG_ERROR("License file is outside of script directory: {}", license_path);
        return std::nullopt;
      }

      if (!std::filesystem::is_regular_file(license_path)) {
        LOG_ERROR("License file does not exist: {}", license_path);
        return std::nullopt;
      }

      return license_path;
    }

    return std::nullopt;
  }
}