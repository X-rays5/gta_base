//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP
#define GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP

#include <filesystem>
#include <memory>
#include <string>
#include <spdlog/pattern_formatter.h>

namespace base::common::logging {
  class Manager {
  public:
    // Pass false from a console application that already owns a console. It skips the
    // stdout sink, and with it EnsureConsole/FreeConsole, which can only fail for a process
    // that is already attached to a console - a failure Init treats as fatal.
    explicit Manager(bool console_sink = true);
    ~Manager();

    static void Shutdown() noexcept;

  private:
    static void Init();
  };

  /**
   * Whether a logger built in this process carries a console sink. The console is the manager's
   * business - it is created by Init, and a process that already owns one is told to skip it - so a
   * logger built outside the manager has to ask rather than decide for itself.
   */
  [[nodiscard]] bool ConsoleSinkEnabled();

  /**
   * A formatter for the pattern given, with this project's own thread flag registered on it.
   *
   * Every logger here formats with the thread column, and the flag it is written by lives in
   * formatter/thread_id.hpp rather than in spdlog, so a pattern given anywhere else would silently
   * print nothing where the column belongs. Anything logging alongside these loggers should build
   * its formatter here.
   */
  [[nodiscard]] std::unique_ptr<spdlog::pattern_formatter> MakeLogFormatter(const std::string& pattern);
}

#endif //GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP
