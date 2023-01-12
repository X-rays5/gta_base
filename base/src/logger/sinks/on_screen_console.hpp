//
// Created by X-ray on 01/12/23.
//

#pragma once
#ifndef GTA_BASE_ON_SCREEN_CONSOLE_B5C45726A59C43C3879EECC113727667_HPP
#define GTA_BASE_ON_SCREEN_CONSOLE_B5C45726A59C43C3879EECC113727667_HPP
#include <mutex>
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include "../../ui/console/on_screen_console.hpp"

namespace gta_base::logger::sinks {
  template<typename mtx_t>
  class OnScreenConsole : public spdlog::sinks::base_sink<mtx_t> {
  protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
      if (!ui::kON_SCREEN_CONSOLE)
        return;

      spdlog::memory_buf_t formatted;
      this->formatter_->format(msg, formatted);

      ui::kON_SCREEN_CONSOLE->Print(fmt::to_string(formatted));
    }

    void flush_() override {}
  };

  using OnScreenConsole_mt = OnScreenConsole<std::mutex>;
  using OnScreenConsole_st = OnScreenConsole<spdlog::details::null_mutex>;
}
#endif //GTA_BASE_ON_SCREEN_CONSOLE_B5C45726A59C43C3879EECC113727667_HPP
