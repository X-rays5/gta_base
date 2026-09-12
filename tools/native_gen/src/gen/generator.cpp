#include "generator.hpp"

#include <chrono>
#include <cstdio>
#include <string>
#include <vector>

#include "../config/config.hpp"
#include "../constants.hpp"
#include "../db/fetch.hpp"
#include "../db/model.hpp"
#include "../globals.hpp"
#include "../util/log.hpp"
#include "../util/util.hpp"
#include "emitters.hpp"

namespace base::tools::native_gen::gen {
  namespace {
    std::string Join(const std::vector<std::string>& values, const std::string_view separator) {
      std::string result;
      for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
          result += separator;
        }
        result += values[i];
      }
      return result;
    }

    std::string FormatSeconds(const double seconds) {
      char buffer[32];
      std::snprintf(buffer, sizeof(buffer), "%.2fs", seconds);
      return buffer;
    }
  }

  int Run(const cli::CliOptions& options, const std::filesystem::path& exe_path) {
    const auto started = std::chrono::steady_clock::now();

    auto settings = config::LoadSettings(options, exe_path);
    if (!settings) {
      LOG_ERROR("error: {}", settings.error());
      return 1;
    }
    const config::Config& config = settings->config;

    LOG_DETAIL("repository root: {}", settings->paths.root.generic_string());
    if (settings->paths.config_file.empty()) {
      LOG_DETAIL("config: none found, using built-in defaults");
    } else {
      LOG_DETAIL("config: {}", settings->paths.config_file.generic_string());
    }

    const auto document = db::LoadNativeDbDocument(*settings, options.refresh);
    if (!document) {
      LOG_ERROR("error: {}", document.error());
      return 1;
    }

    const auto database = db::ParseNativeDb(document->body);
    if (!database) {
      LOG_ERROR("error: {}", database.error());
      return 1;
    }

    std::error_code ec;
    db::Crossmap crossmap;
    std::string crossmap_note = "disabled";
    if (config.crossmap.enabled) {
      if (!std::filesystem::exists(settings->paths.crossmap_file, ec)) {
        LOG_WARN("warning: crossmap file not found: {}", settings->paths.crossmap_file.generic_string());
        LOG_WARN("warning: generating without crossmap (using native hashes directly)");
        crossmap_note = "not found";
      } else {
        std::string text;
        if (auto error = util::ReadFileText(settings->paths.crossmap_file, text)) {
          LOG_WARN("warning: cannot read crossmap file: {}", settings->paths.crossmap_file.generic_string());
          LOG_WARN("warning: generating without crossmap (using native hashes directly)");
          crossmap_note = "unreadable";
        } else {
          crossmap = db::ParseCrossmap(text);
          crossmap_note = std::to_string(crossmap.entries.size()) + " entries";
        }
      }
    }

    std::size_t total_natives = 0;
    std::size_t unmatched = 0;
    std::vector<std::string> excluded_groups;
    for (const auto& group : *database) {
      total_natives += group.natives.size();
      if (config::IsGroupExcluded(config.generation, group.name)) {
        excluded_groups.push_back(group.name);
        continue;
      }
      for (const auto& native : group.natives) {
        if (!crossmap.Has(native.hash)) {
          ++unmatched;
        }
      }
    }

    const auto out_dir = settings->paths.output_dir;
    std::filesystem::create_directories(out_dir, ec);
    if (ec) {
      LOG_ERROR("error: cannot create output directory {}: {}", out_dir.generic_string(), ec.message());
      return 1;
    }

    const bool crlf = config::UseCrlf(config.output);
    std::vector<std::filesystem::path> written;
    const auto write = [&](const std::string_view name, const std::string& content) {
      const auto path = out_dir / std::string(name);
      if (auto error = util::WriteTextFile(path, content, crlf, false)) {
        LOG_ERROR("error: cannot write {}: {}", path.generic_string(), *error);
        return false;
      }
      written.push_back(path);
      LOG_DETAIL("wrote {}", path.generic_string());
      return true;
    };

    if (options.command != cli::Command::Lookup) {
      if (!write(kOutputHeader, EmitNativesHeader(config, crossmap, *database))) {
        return 1;
      }
      if (!write(kOutputImpl, EmitNativesImpl(config, crossmap, *database))) {
        return 1;
      }
      if (!crossmap.empty()) {
        if (!write(kOutputCrossmapHeader, EmitCrossmapHeader(config, crossmap))) {
          return 1;
        }
        if (!write(kOutputCrossmapImpl, EmitCrossmapImpl(config, crossmap))) {
          return 1;
        }
      }
      // Built before either is written so an unsupported type leaves no half-written pair behind.
      const auto angelscript_impl = EmitAngelScriptImpl(config, crossmap, *database);
      if (!angelscript_impl) {
        LOG_ERROR("error: {}", angelscript_impl.error());
        return 1;
      }
      if (!write(kOutputAsHeader, EmitAngelScriptHeader(config))) {
        return 1;
      }
      if (!write(kOutputAsImpl, *angelscript_impl)) {
        return 1;
      }
    }

    if (options.command != cli::Command::Generate) {
      if (!write(kOutputLookup, EmitNativeLookup(config, crossmap, *database))) {
        return 1;
      }
    }

    if (globals::kVERBOSE) {
      for (const auto& group : *database) {
        if (!config::IsGroupExcluded(config.generation, group.name)) {
          LOG_DETAIL("  {}: {} natives", group.name, group.natives.size());
        }
      }
    }

    const std::string excluded_note =
        excluded_groups.empty() ? "none" : std::to_string(excluded_groups.size()) + "  [" + Join(excluded_groups, ", ") + "]";

    LOG_INFO("native_gen -> {}", out_dir.generic_string());
    LOG_INFO("  nativedb   {} groups, {} natives  ({})", database->size(), total_natives,
             document->from_cache ? "cache" : "downloaded");
    LOG_INFO("  crossmap   {}", crossmap_note);
    LOG_INFO("  excluded   {}", excluded_note);
    LOG_INFO("  unmatched  {} natives missing in crossmap", unmatched);
    LOG_INFO("  wrote      {} files in {}",
             written.size(), FormatSeconds(std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count()));
    return 0;
  }
}
