//
// Created by X-ray on 04/04/2026.
//

#pragma once

namespace base::menu::lua {
  class ScriptMetaData {
    public:
    explicit ScriptMetaData(const std::filesystem::path& path);

    Status ReadMetaData();

    std::filesystem::path GetMainFile() const;
    std::filesystem::path GetPath() const;
    std::string GetName() const;
    std::optional<std::string> GetDescription() const;
    std::vector<std::string> GetAuthor() const;
    std::optional<std::string> GetVersion() const;
    std::optional<std::string> GetRepository() const;
    std::optional<std::filesystem::path> GetLicense() const;

  private:
    static const std::string meta_filename_;

    std::filesystem::path main_file_;
    std::filesystem::path path_;
    std::string name_;
    std::string description_;
    std::vector<std::string> authors_;
    std::string version_;
    std::string repository_;
    std::filesystem::path license_;
  };
}