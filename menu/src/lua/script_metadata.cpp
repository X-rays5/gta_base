//
// Created by X-ray on 04/04/2026.
//

#include "script_metadata.hpp"

#pragma warning(push)
#pragma warning(disable : 5321)
#include <sol/sol.hpp>
#pragma warning(pop)

namespace base::menu::lua {
  const std::string ScriptMetaData::meta_filename_ = "manifest.lua";

  ScriptMetaData::ScriptMetaData(const std::filesystem::path& path) : path_(path) {}

  Status ScriptMetaData::ReadMetaData() {
    if (!std::filesystem::is_directory(path_)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Path is not a directory: {}", path_);
    }

    const auto manifest_file = path_ / meta_filename_;
    if (!std::filesystem::is_regular_file(manifest_file)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Manifest file not found: {}", manifest_file);
    }

    sol::state L{};
    L.safe_script_file(manifest_file.string());

    const auto name = L["name"];
    if (name.valid() && name.get_type() == sol::type::string) {
      name_ = name.get<std::string>();
    } else {
      return MakeFailure<ResultCode::kNOT_FOUND>("Mandatory field 'name' not found or invalid in manifest: {}", manifest_file);
    }

    const auto main_file = L["main_file"];
    if (main_file.valid() && main_file.get_type() == sol::type::string) {
      main_file_ = path_ / main_file.get<std::string>();
      if (!std::filesystem::is_regular_file(main_file_)) {
        return MakeFailure<ResultCode::kNOT_FOUND>("Main file specified in manifest not found: {}", main_file_);
      }
    } else {
      return MakeFailure<ResultCode::kNOT_FOUND>("Mandatory field 'main_file' not found or invalid in manifest: {}", manifest_file);
    }

    const auto description = L["description"];
    if (description.valid() && description.get_type() == sol::type::string) {
      description_ = description.get<std::string>();
    }

    const auto author = L["author"];
    if (author.valid() && author.get_type() == sol::type::string) {
      authors_.push_back(author.get<std::string>());
    }

    const auto authors = L["authors"];
    if (authors.valid() && authors.get_type() == sol::type::table) {
      authors_ = authors.get<std::vector<std::string>>();
    }

    const auto version = L["version"];
    if (version.valid() && version.get_type() == sol::type::string) {
      version_ = version.get<std::string>();
    }

    const auto repository = L["repository"];
    if (repository.valid() && repository.get_type() == sol::type::string) {
      repository_ = repository.get<std::string>();
    }

    const auto license_file = L["license"];
    if (license_file.valid() && license_file.get_type() == sol::type::string) {
      license_ = license_file.get<std::string>();
      if (!std::filesystem::is_regular_file(license_)) {
        return MakeFailure<ResultCode::kNOT_FOUND>("License file specified in manifest not found: {}", license_);
      }
    }

    LOG_DEBUG("Successfully read script metadata from manifest: {}", manifest_file);
    return {};
  }

  std::filesystem::path ScriptMetaData::GetMainFile() const {
    return main_file_;
  }

  std::filesystem::path ScriptMetaData::GetPath() const {
    return path_;
  }

  std::string ScriptMetaData::GetName() const {
    return name_;
  }

  std::optional<std::string> ScriptMetaData::GetDescription() const {
    return description_.empty() ? std::nullopt : std::make_optional(description_);
  }

  std::vector<std::string> ScriptMetaData::GetAuthor() const {
    return authors_;
  }

  std::optional<std::string> ScriptMetaData::GetVersion() const {
    return version_.empty() ? std::nullopt : std::make_optional(version_);
  }

  std::optional<std::string> ScriptMetaData::GetRepository() const {
    return repository_.empty() ? std::nullopt : std::make_optional(repository_);
  }

  std::optional<std::filesystem::path> ScriptMetaData::GetLicense() const {
    return license_.empty() ? std::nullopt : std::make_optional(license_);
  }
}
