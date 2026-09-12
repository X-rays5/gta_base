//
// Created by X-ray on 11/09/2026.
//

#include "as_script_manifest.hpp"
#include <glaze/toml.hpp>

#include <base-common/fs/vfs.hpp>

namespace base::menu::as::script {
  namespace {
    /// Whether `c` is a letter or a digit, spelled out rather than left to std::isalnum: a script's
    /// name is UTF-8 text of whatever the author typed, and no locale has any business deciding which
    /// of its bytes an option name can be made of.
    bool IsNameChar(const char c) {
      return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    }

    /// `name` as the one word an option name can be built from: its letters and digits, with every run
    /// of anything else - a space, a dash, the bytes of a character outside ASCII - become a single
    /// underscore between them and none at either end.
    ///
    /// A script's name is written to be read rather than to be typed, so "Example Option Registry"
    /// becomes `example_option_registry` here rather than being refused.
    std::string ToPrefix(const std::string_view name) {
      std::string prefix;
      prefix.reserve(name.size());

      for (const char c : name) {
        if (c >= 'a' && c <= 'z') {
          prefix.push_back(c);
        } else if (c >= 'A' && c <= 'Z') {
          prefix.push_back(static_cast<char>(c - 'A' + 'a'));
        } else if (c >= '0' && c <= '9') {
          prefix.push_back(c);
        } else if (!prefix.empty() && prefix.back() != '_') {
          prefix.push_back('_');
        }
      }

      while (!prefix.empty() && prefix.back() == '_') {
        prefix.pop_back();
      }

      return prefix;
    }

    /// Whether `short_name` is something an option name can be made from: one word of letters, digits,
    /// underscores and dashes. A short name that cannot be read back the way it was written is
    /// reported rather than used, since what it would produce is a name no script could run.
    bool IsUsableShortName(const std::string_view short_name) {
      if (short_name.empty()) {
        return false;
      }

      for (const char c : short_name) {
        if (!IsNameChar(c) && c != '_' && c != '-') {
          return false;
        }
      }

      return true;
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
    if (!common::fs::vfs::EnsureIsWithinDirectory(path_, main_file_path)) {
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

  std::string ScriptManifest::GetOptionPrefix() const {
    if (data_.short_name.has_value()) {
      if (IsUsableShortName(*data_.short_name)) {
        return *data_.short_name;
      }

      LOG_ERROR("The short_name '{}' of script '{}' is not one word of letters, digits, underscores and "
                "dashes, so it cannot be what an option name is built from: the script's own name is used "
                "instead", *data_.short_name, data_.name);
    }

    const std::string prefix = ToPrefix(data_.name);
    if (prefix.empty()) {
      // A name with nothing nameable in it. Its options are registered unprefixed, which is what a
      // script registered before prefixes existed - said out loud because two scripts in that state
      // would then collide over an option name rather than being kept apart by it.
      LOG_WARN("The name '{}' of a script has nothing an option prefix can be made from, so its options are "
               "registered unprefixed", data_.name);
    }

    return prefix;
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
      if (!common::fs::vfs::EnsureIsWithinDirectory(path_, license_path)) {
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