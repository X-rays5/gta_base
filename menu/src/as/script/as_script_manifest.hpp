//
// Created by X-ray on 11/09/2026.
//

#pragma once

namespace base::menu::as::script {
  class ScriptManifest {
  public:
    explicit ScriptManifest(const std::filesystem::path& path);

    Status ReadMetaData();

    std::filesystem::path GetMainFile() const;
    std::filesystem::path GetPath() const;
    std::string GetName() const;

    /**
     * The prefix this script's own options are registered and run under.
     *
     * A script's options are its own, so they are kept apart from the menu's and from any other
     * script's by this prefix rather than by asking every author to spell one into every name they
     * write. It is the manifest's `short_name` when it has one, and a word made out of the script's
     * name otherwise - which is why a script that wants to write `optreg.set_level` rather than
     * `example_option_registry.set_level` is a line rather than a longer name everywhere.
     */
    [[nodiscard]] std::string GetOptionPrefix() const;
    std::optional<std::string> GetDescription() const;
    std::vector<std::string> GetAuthor() const;
    std::optional<std::string> GetVersion() const;
    std::optional<std::string> GetRepository() const;
    std::optional<std::filesystem::path> GetLicense() const;

  private:
    struct Data {
      std::string main_file;
      std::string name;
      std::optional<std::string> short_name;
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