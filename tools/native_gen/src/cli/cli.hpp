#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace base::tools::native_gen::cli {
  enum class Command { All, Generate, Lookup };

  struct CliOptions {
    Command command{Command::All};
    std::optional<std::string> config_path;
    std::optional<std::string> root;
    std::optional<std::string> out_dir;
    std::optional<std::string> crossmap_path;
    bool no_crossmap{false};
    bool refresh{false};
    bool verbose{false};
    bool quiet{false};
    bool show_help{false};
    bool show_version{false};
  };

  class CliError : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
  };

  [[nodiscard]] CliOptions ParseCli(const std::vector<std::string>& args);

  [[nodiscard]] std::string UsageText();
}
