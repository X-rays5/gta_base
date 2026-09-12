//
// Created by X-ray on 11/09/2026.
//

#pragma once

namespace base::menu::as::script {
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
    struct Data {
      std::string main_file;
      std::string name;
      std::optional<std::string> description;
      std::vector<std::string> authors;
      std::optional<std::string> version;
      std::optional<std::string> repository;
      std::optional<std::string> license;
    };

    static const std::string meta_filename_;

    std::filesystem::path path_;
    Data data_;
  };
}