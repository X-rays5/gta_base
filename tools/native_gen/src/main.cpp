#include <cstdio>
#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "cli/cli.hpp"
#include "gen/generator.hpp"
#include "globals.hpp"
#include "util/log.hpp"

int main(int argc, char** argv) {
  using namespace base::tools::native_gen;

  std::vector<std::string> args;
  args.reserve(static_cast<std::size_t>(argc > 1 ? argc - 1 : 0));
  for (int index = 1; index < argc; ++index) {
    args.emplace_back(argv[index]);
  }

  cli::CliOptions options;
  try {
    options = cli::ParseCli(args);
  } catch (const cli::CliError& error) {
    // The logger does not exist yet, so this goes straight to stderr.
    std::fprintf(stderr, "error: %s\n\n%s", error.what(), cli::UsageText().c_str());
    return 2;
  }

  if (options.show_version) {
    std::fputs("native_gen 1.0\n", stdout);
    return 0;
  }
  if (options.show_help) {
    std::fputs(cli::UsageText().c_str(), stdout);
    return 0;
  }

  // This tool is a console application that already owns a console, so the Manager must not
  // attach one, and must not free it on the way out.
  globals::kLOGGER = std::make_unique<base::common::logging::Manager>(false);
  globals::kVERBOSE = options.verbose;
  util::SetUpConsoleLogging(options.quiet);

  int exit_code = 0;
  try {
    exit_code = gen::Run(options, argc > 0 ? std::filesystem::path(argv[0]) : std::filesystem::path{});
  } catch (const std::exception& error) {
    LOG_ERROR("error: unexpected failure: {}", error.what());
    exit_code = 1;
  }

  // The Manager has to be torn down while spdlog is still alive. Its destructor shuts the
  // registry down, and a namespace scope unique_ptr is destroyed at exit - after the
  // registry's own function-local static, which leaving it to the destructor would outlive.
  // injector does the same with an explicit Shutdown from atexit.
  globals::kLOGGER.reset();

  return exit_code;
}
