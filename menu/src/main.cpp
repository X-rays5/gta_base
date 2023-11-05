//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <ThreadPool/ThreadPool.h>

std::atomic<bool> base::globals::kRUNNING = false;

int base::menu_main() {
  hmthrp::ThreadPool thread_pool(std::thread::hardware_concurrency());
  LOG_INFO("[INIT] Created thread pool with {} threads", std::thread::hardware_concurrency());

  while (globals::kRUNNING) {
    if (GetAsyncKeyState(VK_END))
      break;
  }

  thread_pool.shutdown();
  LOG_INFO("[SHUTDOWN] Thread pool has been shutdown");

  return 0;
}