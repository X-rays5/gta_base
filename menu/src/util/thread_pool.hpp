//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_THREAD_POOL_HPP
#define GTA_BASE_THREAD_POOL_HPP
#include <glaze/thread/threadpool.hpp>

namespace base::menu::util {
  using ThreadPool = glz::pool;

  inline ThreadPool* kTHREAD_POOL{};
}
#endif //GTA_BASE_THREAD_POOL_HPP
