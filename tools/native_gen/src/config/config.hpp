#pragma once

#include <filesystem>
#include <expected>
#include <string>
#include <vector>

#include "../cli/cli.hpp"

namespace base::tools::native_gen::config {
  // The glaze TOML reader appends to a vector that already holds values rather than
  // replacing it, so the list members start empty and these defaults are applied afterwards
  // whenever the config file omits the key.
  inline const std::vector<std::string> kDefaultHeaderIncludes{
      "#include \"../game/native_types.hpp\"",
      "#include \"invoker.hpp\"",
  };
  inline const std::vector<std::string> kDefaultExcludedGroups{"BUILTIN"};
  inline const std::vector<std::string> kDefaultAngelScriptIncludes{
      "#include \"../as/as_bind.hpp\"",
  };

  // Mirrors config.toml one-to-one. Every key has a built-in default, so the tool runs
  // with no config file at all.
  struct NativesConfig {
    std::string url;
    std::string cache_file{"tools/native_gen/natives_gen9.cache.json"};
    int timeout_seconds{10};
  };

  struct OutputConfig {
    std::string dir{"menu/src/natives"};
    std::string line_ending{"crlf"};
  };

  struct GenerationConfig {
    std::string base_namespace{"base::menu::natives"};
    std::string invoker_function{"Invoker::Invoke"};
    std::string log_native_error_func{"LOG_ERROR"};
    std::vector<std::string> header_includes;
    std::vector<std::string> excluded_groups;
  };

  // The AngelScript surface. base_namespace is deliberately separate from
  // GenerationConfig::base_namespace, which is the C++ namespace the typed natives already live in.
  struct AngelScriptConfig {
    std::string base_namespace{"natives"};
    bool docs{true};
    std::vector<std::string> includes;
  };

  struct CrossmapConfig {
    std::string file{"tools/native_gen/crossmap.txt"};
    bool enabled{true};
  };

  struct PathsConfig {
    std::string root;
  };

  struct Config {
    NativesConfig natives{};
    OutputConfig output{};
    GenerationConfig generation{};
    AngelScriptConfig angelscript{};
    CrossmapConfig crossmap{};
    PathsConfig paths{};
  };

  // Everything the run needs, with relative paths resolved against the repository root.
  struct ResolvedPaths {
    std::filesystem::path root;
    std::filesystem::path output_dir;
    std::filesystem::path crossmap_file;
    std::filesystem::path cache_file;
    std::filesystem::path config_file;  // empty when running on built-in defaults
  };

  struct Settings {
    Config config{};
    ResolvedPaths paths{};
  };

  [[nodiscard]] std::expected<Settings, std::string> LoadSettings(const cli::CliOptions& options,
                                                                  const std::filesystem::path& exe_path);

  // True when the group name matches the configured exclusion list, case-insensitively.
  [[nodiscard]] bool IsGroupExcluded(const GenerationConfig& generation, const std::string& group_name);

  [[nodiscard]] bool UseCrlf(const OutputConfig& output);
}
