//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <ThreadPool/ThreadPool.h>

int base::menu_main() {
  hmthrp::ThreadPool thread_pool(std::thread::hardware_concurrency());

  auto fut = thread_pool.dispatch(false, [] {
    std::string hello_world = "Hello World!";
    return hello_world;
  });

  LOG_INFO(fut.get());

  return 0;
}