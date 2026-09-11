#pragma once

#include <filesystem>

#include "../cli/cli.hpp"

namespace base::tools::native_gen::gen {
  [[nodiscard]] int Run(const cli::CliOptions& options, const std::filesystem::path& exe_path);
}
