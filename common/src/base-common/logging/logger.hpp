//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP
#define GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP

#include <filesystem>

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
}

#endif //GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP
