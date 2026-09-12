//
// Created by X-ray on 11/09/2026.
//

#include "as_script_metadata.hpp"
#include <glaze/toml.hpp>

namespace base::menu::as::script {
  namespace {
    bool EnsureIsWithinDirectory(const std::filesystem::path& base, const std::filesystem::path& target) {
      const auto canonical_base = std::filesystem::canonical(base);
      const auto canonical_target = std::filesystem::canonical(target);

      return std::mismatch(canonical_base.begin(), canonical_base.end(), canonical_target.begin()).first == canonical_base.end();
    }
  }

  const std::string ScriptMetaData::meta_filename_ = "manifest.toml";

  ScriptMetaData::ScriptMetaData(const std::filesystem::path& path) : path_(path) {}

  Status ScriptMetaData::ReadMetaData() {
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

  std::filesystem::path ScriptMetaData::GetMainFile() const {
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

  std::filesystem::path ScriptMetaData::GetPath() const {
    return path_;
  }

  std::string ScriptMetaData::GetName() const {
    return data_.name;
  }

  std::optional<std::string> ScriptMetaData::GetDescription() const {
    return data_.description;
  }

  std::vector<std::string> ScriptMetaData::GetAuthor() const {
    return data_.authors;
  }

  std::optional<std::string> ScriptMetaData::GetVersion() const {
    return data_.version;
  }

  std::optional<std::string> ScriptMetaData::GetRepository() const {
    return data_.repository;
  }

  std::optional<std::filesystem::path> ScriptMetaData::GetLicense() const {
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