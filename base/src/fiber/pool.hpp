//
// Created by X-ray on 08/12/22.
//

#pragma once
#ifndef GTA_BASE_POOL_HPP
#define GTA_BASE_POOL_HPP
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include "../natives/natives.hpp"
#undef AddJob

namespace gta_base {
  namespace fiber {
    class Pool {
    public:
      using job_t = std::function<void()>;

    public:
      explicit Pool(std::size_t num_per_tick);
      ~Pool();

      void AddJob(const job_t& job);

      void Tick();
      [[noreturn]] static void FiberFunc();

    private:
      std::mutex mtx_;
      std::queue<job_t> jobs_;
    };
    inline Pool* kPOOL{};
  }
}
#endif //GTA_BASE_POOL_HPP
