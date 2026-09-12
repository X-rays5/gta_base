#include "cli.hpp"

#include <string_view>

namespace base::tools::native_gen::cli {
  namespace {
    std::string RequireValue(const std::vector<std::string>& args, std::size_t& index, const std::string& flag) {
      if (index + 1 >= args.size()) {
        throw CliError(flag + " needs a value");
      }
      return args[++index];
    }

    // Supports both "--flag value" and "--flag=value".
    std::optional<std::string> SplitInlineValue(const std::string& arg, const std::string_view name) {
      if (!arg.starts_with(name) || arg.size() == name.size() || arg[name.size()] != '=') {
        return std::nullopt;
      }
      return arg.substr(name.size() + 1);
    }
  }

  CliOptions ParseCli(const std::vector<std::string>& args) {
    CliOptions options;
    bool command_seen = false;

    for (std::size_t index = 0; index < args.size(); ++index) {
      const std::string& arg = args[index];

      if (arg == "-h" || arg == "--help") {
        options.show_help = true;
      } else if (arg == "--version") {
        options.show_version = true;
      } else if (arg == "-v" || arg == "--verbose") {
        options.verbose = true;
      } else if (arg == "-q" || arg == "--quiet") {
        options.quiet = true;
      } else if (arg == "--refresh") {
        options.refresh = true;
      } else if (arg == "--no-crossmap") {
        options.no_crossmap = true;
      } else if (arg == "-c" || arg == "--config") {
        options.config_path = RequireValue(args, index, arg);
      } else if (arg == "--root") {
        options.root = RequireValue(args, index, arg);
      } else if (arg == "-o" || arg == "--out-dir") {
        options.out_dir = RequireValue(args, index, arg);
      } else if (arg == "--crossmap") {
        options.crossmap_path = RequireValue(args, index, arg);
      } else if (const auto config_value = SplitInlineValue(arg, "--config")) {
        options.config_path = *config_value;
      } else if (const auto root_value = SplitInlineValue(arg, "--root")) {
        options.root = *root_value;
      } else if (const auto out_dir_value = SplitInlineValue(arg, "--out-dir")) {
        options.out_dir = *out_dir_value;
      } else if (const auto crossmap_value = SplitInlineValue(arg, "--crossmap")) {
        options.crossmap_path = *crossmap_value;
      } else if (!arg.empty() && arg[0] == '-') {
        throw CliError("unknown option " + arg);
      } else if (!command_seen) {
        command_seen = true;
        if (arg == "all") {
          options.command = Command::All;
        } else if (arg == "generate") {
          options.command = Command::Generate;
        } else if (arg == "lookup") {
          options.command = Command::Lookup;
        } else {
          throw CliError("unknown command " + arg);
        }
      } else {
        throw CliError("unexpected argument " + arg);
      }
    }

    if (options.verbose && options.quiet) {
      throw CliError("--verbose and --quiet cannot be combined");
    }

    return options;
  }

  std::string UsageText() {
    return "native_gen - generate GTA5 native bindings from the NativeDB\n"
           "\n"
           "Usage:\n"
           "  native_gen [all]     [options]   generate everything (default)\n"
           "  native_gen generate  [options]   natives_gen9.hpp/.cpp, crossmap.hpp/.cpp, natives_as.hpp/.cpp\n"
           "  native_gen lookup    [options]   native_lookup.hpp\n"
           "\n"
           "Options:\n"
           "  -c, --config <path>   configuration file to load\n"
           "      --root <path>     repository root used to resolve relative paths\n"
           "  -o, --out-dir <dir>   where generated files are written\n"
           "      --crossmap <path> crossmap file to use\n"
           "      --no-crossmap     generate as if no crossmap existed\n"
           "      --refresh         ignore the cached native database and re-download it\n"
           "  -v, --verbose         report per-group detail\n"
           "  -q, --quiet           only report warnings and errors\n"
           "      --version         print the version and exit\n"
           "  -h, --help            print this message and exit\n";
  }
}
