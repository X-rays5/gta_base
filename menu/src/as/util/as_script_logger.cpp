//
// Created by X-ray on 12/09/2026.
//

#include "as_script_logger.hpp"

#include <deque>
#include <mutex>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <angelscript.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logger.hpp>
#include <base-common/logging/logging_macro.hpp>

#include "../script/as_script_manager.hpp"

namespace base::menu::as::util {
  namespace {
    /// The script's name, the .as file the line was written in, and the line.
    ///
    /// The location is in the pattern rather than in the message so that it lands in spdlog's source
    /// column, where a tool reading the log looks for it. The file stays in this pattern in a release
    /// build too, where the menu's own logger drops it: for a script it is the point of the feature,
    /// and it is always a short .as name rather than a path.
    constexpr const char* kScriptLogPattern = "[%Y-%m-%dT%T%z] [%^%L%$] [%N] [%n:%s:%#] %v";

    /**
     * What this file keeps between calls: the loggers it has built, and a copy of every section name
     * a line has been logged from.
     *
     * Both have to outlive the teardown of what they describe. A log message is formatted by one of
     * the pool's threads, and the queue copies the payload and the logger's name but not the source
     * location - so the .as name a line carries has to still be there when the pool gets to it, and the
     * module it came from does not: a script's destructor discards it, usually on the tick after the
     * line was logged. The copies below are the fix; the module names the closest thing to a stable
     * string it has, which is why they are held for the life of the process rather than the script's.
     *
     * The object is deliberately never destroyed. The last thing a script does on its way out is hand
     * its logger back, and spdlog's pool is drained by its own static destructor - so anything here
     * could be asked for after this file's statics would have gone, which is exactly the case a leaked
     * object makes impossible.
     */
    struct ScriptLoggers {
      std::mutex mutex;
      /// By the name the script is known by, which is also the name its logger is built with and the
      /// name the manager keeps it under.
      std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> by_name;
      /// Every .as section name a line has been logged from, copied out of its module. A deque, so a
      /// string already handed to a formatter keeps its address as more are added.
      std::deque<std::string> sections;
    };

    ScriptLoggers& GetScriptLoggers() {
      static auto* loggers = new ScriptLoggers();
      return *loggers;
    }

    /// A copy of `section` that outlives the module it came from - see ScriptLoggers. Returns the copy
    /// already held when there is one, since a script logs from the same few sections over and over.
    const char* InternSection(const char* section) {
      if (section == nullptr) {
        return nullptr;
      }

      const std::string_view wanted{section};
      auto& loggers = GetScriptLoggers();
      const std::scoped_lock lock(loggers.mutex);

      for (const auto& known : loggers.sections) {
        if (known == wanted) {
          return known.c_str();
        }
      }

      return loggers.sections.emplace_back(wanted).c_str();
    }

    /// The pool an async logger is queued on. The menu builds it with its logging manager; a process
    /// without one - the test binary - gets it here, in the same way and under the same lock spdlog's
    /// own async factory creates it, so that a logger is buildable wherever a script is loaded.
    void EnsureThreadPool() {
      auto& registry = spdlog::details::registry::instance();
      const std::lock_guard<std::recursive_mutex> lock(registry.tp_mutex());
      if (registry.get_tp() == nullptr) {
        registry.set_tp(std::make_shared<spdlog::details::thread_pool>(spdlog::details::default_async_q_size, 1U));
      }
    }

    /// The logger the script this thread is running logs through: the menu's, with no location, when
    /// there is no such script. That is also what a line logged from outside a script gets.
    void LogToMenuLogger(const spdlog::level::level_enum level, const std::string_view message) {
      // The location is left empty rather than taken from a module the menu's logger has no hold on:
      // it is one of the loggers that carries no copy of a section name, and the string handed to it
      // would be read by the pool after the module it points into is gone.
      if (const auto& logger = spdlog::default_logger()) {
        logger->log(spdlog::source_loc{}, level, spdlog::string_view_t{message});
      }
    }
  }

  std::shared_ptr<spdlog::logger> CreateScriptLogger(const std::string& script_name) {
    EnsureThreadPool();

    const auto log_file = common::fs::vfs::GetScriptLogDir(script_name) / (script_name + ".log");

    std::vector<spdlog::sink_ptr> sinks;
    if (common::logging::ConsoleSinkEnabled()) {
      sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    }
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file.string()));

    // Named after the script: the name is what the pattern renders as the script's, so a line says
    // which script wrote it without the message having to.
    auto logger = std::make_shared<spdlog::async_logger>(script_name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    // Everything a script logs is kept, and every line is flushed as it is written: a script shares
    // this process with the game, and the last thing it logs before taking the game down with it is
    // the line worth having on disk. The flush is the pool's, on the back of the line it belongs to.
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);

    logger->set_formatter(common::logging::MakeLogFormatter(kScriptLogPattern));

    logger->set_error_handler([](const std::string& err) {
      // The menu's logger rather than this one: a logger that reports its own failure through itself
      // is the one way to make a failing sink fail twice.
      LOG_ERROR("[AS] script logger: {}", err);
    });

    auto& loggers = GetScriptLoggers();
    const std::scoped_lock lock(loggers.mutex);

    // An entry this file already held under the name is the previous instance of the same script,
    // which the game thread has not released yet - a script can be loaded again before the tick that
    // destroys the last one, which is what makes loading over an instance that is still running
    // work. Nothing else may be displaced: the name is the script's own, but spdlog's registry is
    // keyed by name and the menu's loggers are in it too, and a manifest is not allowed to take one
    // of those over.
    if (loggers.by_name.contains(script_name)) {
      spdlog::register_or_replace(logger);
    } else if (spdlog::get(script_name) == nullptr) {
      spdlog::register_logger(logger);
    } else {
      LOG_WARN("[AS] a logger is already registered as '{}', so the script of that name logs outside spdlog's registry", script_name);
    }

    loggers.by_name.insert_or_assign(script_name, logger);
    return logger;
  }

  void DestroyScriptLogger(const std::shared_ptr<spdlog::logger>& logger) {
    if (!logger) {
      return;
    }

    const std::string script_name = logger->name();

    auto& loggers = GetScriptLoggers();
    const std::scoped_lock lock(loggers.mutex);

    // Only an entry that is still this logger's own is dropped. A script loaded again while its
    // previous instance was still being torn down has taken the name over, and this instance coming
    // out must not take the replacement's - or the menu's - entry with it.
    if (const auto it = loggers.by_name.find(script_name); it != loggers.by_name.end() && it->second == logger) {
      loggers.by_name.erase(it);
    }

    if (spdlog::get(script_name) == logger) {
      spdlog::drop(script_name);
    }
  }

  void LogFromScript(const spdlog::level::level_enum level, const std::string& message) {
    AngelScript::asIScriptContext* ctx = AngelScript::asGetActiveContext();
    if (ctx == nullptr) {
      LogToMenuLogger(level, message);
      return;
    }

    // Read on this thread and no other, which is the only reason the section name is still there to
    // read: it belongs to the module the call is running inside of, and the copy taken below is what
    // the pool formats the line against once that module is gone.
    const char* section = nullptr;
    const int line = ctx->GetLineNumber(0, nullptr, &section);

    const std::string script_name = script::ScriptManager::RunningScriptName();
    std::shared_ptr<spdlog::logger> logger;

    if (!script_name.empty()) {
      auto& loggers = GetScriptLoggers();
      const std::scoped_lock lock(loggers.mutex);
      if (const auto it = loggers.by_name.find(script_name); it != loggers.by_name.end()) {
        logger = it->second;
      }
    }

    if (!logger) {
      LogToMenuLogger(level, message);
      return;
    }

    logger->log(spdlog::source_loc{InternSection(section), line, nullptr}, level, spdlog::string_view_t{message});
  }
}
