//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_THREAD_POOL_HPP
#define GTA_BASE_THREAD_POOL_HPP
#include <glaze/thread/threadpool.hpp>
#include "startup_shutdown_handler.hpp"

namespace base::menu::util {
  using ThreadPool = glz::pool;
  inline ThreadPool* kTHREAD_POOL{};

  inline void RegisterThreadPoolStartupShutdown(std::unique_ptr<ThreadPool>& thread_inst, StartupShutdownHandler* startup_shutdown_handler) {
    startup_shutdown_handler->AddCallback("ThreadPool", [&](const StartupShutdownHandler::Action action) {
      if (action == StartupShutdownHandler::Action::Init) {
        thread_inst = std::make_unique<std::remove_reference_t<decltype(*thread_inst)>>(std::thread::hardware_concurrency() / 2);
        kTHREAD_POOL = thread_inst.get();
      } else {
        kTHREAD_POOL = nullptr;
        thread_inst.reset();
      }
    });
  }
}
#endif //GTA_BASE_THREAD_POOL_HPP
