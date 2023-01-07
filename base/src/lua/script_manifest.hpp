//
// Created by X-ray on 12/12/22.
//

#pragma once
#ifndef GTA_BASE_SCRIPT_MANIFEST_HPP
#define GTA_BASE_SCRIPT_MANIFEST_HPP
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <sol/sol.hpp>

namespace gta_base::lua {
class Manifest {
 public:
  Manifest() = default;
  /**
   * @param script_dir Path to the dir the target script is in
   */
  explicit Manifest(const std::filesystem::path &script_dir);

  bool Valid() {
    return !main_file_.empty() && !name_.empty() && !version_.empty();
  }

  [[nodiscard]] std::filesystem::path GetScriptDir() const {
    return root_path_;
  }

  [[nodiscard]] std::string GetMainFile() const {
    return main_file_;
  }

  [[nodiscard]] std::string GetName() const {
    return name_;
  }

  [[nodiscard]] std::optional<std::string> GetDescription() const {
    return description_.empty() ? std::nullopt : std::optional(description_);
  }

  [[nodiscard]] std::string GetVersion() const {
    return version_;
  }

  [[nodiscard]] std::optional<std::vector<std::string>> GetAuthors() const {
    return authors_.empty() ? std::nullopt : std::optional(authors_);
  }

  [[nodiscard]] std::optional<std::string> GetRepository() const {
    return repository_.empty() ? std::nullopt : std::optional(repository_);
  }

  [[nodiscard]] std::optional<std::string> GetLicense() const {
    return license_.empty() ? std::nullopt : std::optional(license_);
  }

  [[nodiscard]] std::optional<std::string> GetLicenseFile() const {
    return license_file_.empty() ? std::nullopt : std::optional(license_file_);
  }

  [[nodiscard]] bool GetHiddenLib() const {
    return hidden_lib_;
  }

 private:
  /// Path to directory script is in
  std::filesystem::path root_path_;
  /// File where the Init, Tick, Shutdown functions are located
  std::string main_file_;
  /// The name of the package.
  std::string name_;
  /// A description of the package.
  std::string description_;
  /// The version of the package.
  std::string version_;
  /// The authors of the package.
  std::vector<std::string> authors_;
  /// URL of the package source repository.
  std::string repository_;
  /// The package license.
  std::string license_;
  /// Path to the text of the license.
  std::string license_file_;
  /// Hidden lib don't show this anywhere in the UI
  bool hidden_lib_ = false;
};
}
#endif //GTA_BASE_SCRIPT_MANIFEST_HPP
