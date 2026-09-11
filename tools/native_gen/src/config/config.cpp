#include "config.hpp"

#include <array>
#include <cstdlib>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include <glaze/glaze.hpp>
#include <glaze/toml.hpp>

#include "../constants.hpp"
#include "../util/log.hpp"
#include "../util/util.hpp"

namespace base::tools::native_gen::config {
  namespace {
    using KeyTable = std::unordered_map<std::string, std::unordered_set<std::string>>;

    const KeyTable kKnownKeys{
        {"natives", {"url", "cache_file", "timeout_seconds"}},
        {"output", {"dir", "line_ending"}},
        {"generation",
         {"base_namespace", "invoker_function", "log_native_error_func", "header_includes", "excluded_groups"}},
        {"crossmap", {"file", "enabled"}},
        {"paths", {"root"}},
    };

    std::filesystem::path ResolveAgainst(const std::filesystem::path& base, const std::string& value) {
      std::filesystem::path path(value);
      if (path.is_absolute() || value.empty()) {
        return path;
      }
      return (base / path).lexically_normal();
    }

    // Walks up looking for a repository marker. `.git` wins over CMakeLists.txt, otherwise a
    // nested CMakeLists.txt (tools/native_gen has one) would be mistaken for the root.
    std::optional<std::filesystem::path> FindRepoRoot(std::filesystem::path start) {
      std::error_code ec;
      for (const auto marker : {".git", "CMakeLists.txt"}) {
        auto candidate = start;
        for (int depth = 0; depth < 12; ++depth) {
          if (std::filesystem::exists(candidate / marker, ec)) {
            return candidate;
          }
          const auto parent = candidate.parent_path();
          if (parent.empty() || parent == candidate) {
            break;
          }
          candidate = parent;
        }
      }
      return std::nullopt;
    }

    struct Assignment {
      std::string table;
      std::string key;
    };

    // glaze's TOML reader neither reports which keys were present nor replaces the members
    // of a vector, so the key layout is recovered with this scan. Every key in this config
    // is a plain assignment on its own line, optionally under a [table] header.
    std::vector<Assignment> ScanAssignments(const std::string& text) {
      std::vector<Assignment> assignments;
      std::string table;
      std::size_t start = 0;
      while (start <= text.size()) {
        const auto newline = text.find('\n', start);
        const auto end = newline == std::string::npos ? text.size() : newline;

        std::string_view line(text.data() + start, end - start);
        while (!line.empty() && (line.front() == ' ' || line.front() == '\t' || line.front() == '\r')) {
          line.remove_prefix(1);
        }
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
          line.remove_suffix(1);
        }

        if (!line.empty() && line.front() != '#') {
          if (line.front() == '[') {
            const auto close = line.find(']');
            if (close != std::string_view::npos) {
              table = std::string(line.substr(1, close - 1));
            }
          } else if (const auto equals = line.find('='); equals != std::string_view::npos) {
            std::string_view key = line.substr(0, equals);
            while (!key.empty() && (key.back() == ' ' || key.back() == '\t')) {
              key.remove_suffix(1);
            }
            assignments.push_back(Assignment{table, std::string(key)});
          }
        }

        if (newline == std::string::npos) {
          break;
        }
        start = newline + 1;
      }
      return assignments;
    }

    void WarnUnknownKeys(const std::vector<Assignment>& assignments) {
      std::unordered_set<std::string> warned_tables;
      for (const auto& [table, key] : assignments) {
        const auto known = kKnownKeys.find(table);
        if (known == kKnownKeys.end()) {
          if (warned_tables.insert(table).second) {
            LOG_WARN("warning: config: unknown table [{}] (ignored)", table);
          }
          continue;
        }
        if (!known->second.contains(key)) {
          LOG_WARN("warning: config: unknown key [{}].{} (ignored)", table, key);
        }
      }
    }

    bool HasKey(const std::vector<Assignment>& assignments, const std::string_view table,
                const std::string_view key) {
      for (const auto& assignment : assignments) {
        if (assignment.table == table && assignment.key == key) {
          return true;
        }
      }
      return false;
    }

    std::optional<std::filesystem::path> DiscoverConfigFile(const cli::CliOptions& options,
                                                            const std::filesystem::path& exe_dir,
                                                            const std::string& cwd,
                                                            std::string& error) {
      std::error_code ec;

      if (options.config_path) {
        const std::filesystem::path explicit_path(*options.config_path);
        if (!std::filesystem::exists(explicit_path, ec)) {
          error = "config file not found: " + explicit_path.string();
          return std::nullopt;
        }
        return explicit_path;
      }

      if (const char* from_env = std::getenv("NATIVE_GEN_CONFIG"); from_env != nullptr && *from_env != '\0') {
        const std::filesystem::path env_path(from_env);
        if (!std::filesystem::exists(env_path, ec)) {
          error = "NATIVE_GEN_CONFIG points at a missing file: " + env_path.string();
          return std::nullopt;
        }
        return env_path;
      }

      const std::array candidates{
          exe_dir / "config.toml",
          std::filesystem::path(cwd) / "config.toml",
          std::filesystem::path(cwd) / "tools" / "native_gen" / "config.toml",
      };
      for (const auto& candidate : candidates) {
        if (!candidate.empty() && std::filesystem::exists(candidate, ec)) {
          return candidate;
        }
      }
      return std::nullopt;
    }
  }

  std::expected<Settings, std::string> LoadSettings(const cli::CliOptions& options,
                                                    const std::filesystem::path& exe_path) {
    Settings settings;
    settings.config.natives.url = std::string(kDefaultUrl);

    std::error_code ec;
    auto cwd = std::filesystem::current_path(ec);
    if (ec) {
      cwd = ".";
    }
    auto exe_dir = exe_path.parent_path();
    if (exe_dir.empty()) {
      exe_dir = cwd;
    }

    std::string error;
    const auto config_file = DiscoverConfigFile(options, exe_dir, cwd.string(), error);
    if (!error.empty()) {
      return std::unexpected(error);
    }

    // Stays empty when there is no config file, which leaves both list defaults in play.
    std::vector<Assignment> assignments;

    if (config_file) {
      std::string text;
      if (auto read_error = util::ReadFileText(*config_file, text)) {
        return std::unexpected(*read_error);
      }
      if (const auto parse_error = glz::read<glz::toml::toml_opts{.error_on_unknown_keys = false}>(
              settings.config, text)) {
        return std::unexpected("cannot parse " + config_file->string() + ": " +
                               glz::format_error(parse_error, text));
      }
      assignments = ScanAssignments(text);
      WarnUnknownKeys(assignments);
      settings.paths.config_file = *config_file;
    }

    if (!HasKey(assignments, "generation", "header_includes")) {
      settings.config.generation.header_includes = kDefaultHeaderIncludes;
    }
    if (!HasKey(assignments, "generation", "excluded_groups")) {
      settings.config.generation.excluded_groups = kDefaultExcludedGroups;
    }

    // Command line wins over the config file.
    if (options.out_dir) {
      settings.config.output.dir = *options.out_dir;
    }
    if (options.crossmap_path) {
      settings.config.crossmap.file = *options.crossmap_path;
    }
    if (options.no_crossmap) {
      settings.config.crossmap.enabled = false;
    }

    std::filesystem::path root;
    if (options.root) {
      root = std::filesystem::absolute(*options.root, ec).lexically_normal();
      ec.clear();
    } else if (!settings.config.paths.root.empty()) {
      root = ResolveAgainst(cwd, settings.config.paths.root);
    } else if (const auto discovered = FindRepoRoot(config_file ? config_file->parent_path() : exe_dir)) {
      root = *discovered;
    } else {
      root = cwd;
    }

    settings.paths.root = root;
    settings.paths.output_dir = ResolveAgainst(root, settings.config.output.dir);
    settings.paths.crossmap_file = ResolveAgainst(root, settings.config.crossmap.file);
    settings.paths.cache_file = ResolveAgainst(root, settings.config.natives.cache_file);
    return settings;
  }

  bool IsGroupExcluded(const GenerationConfig& generation, const std::string& group_name) {
    if (generation.excluded_groups.empty()) {
      return false;
    }
    const std::string lower_name = util::ToLowerAscii(group_name);
    for (const auto& excluded : generation.excluded_groups) {
      if (util::ToLowerAscii(excluded) == lower_name) {
        return true;
      }
    }
    return false;
  }

  bool UseCrlf(const OutputConfig& output) { return util::ToLowerAscii(output.line_ending) != "lf"; }
}
