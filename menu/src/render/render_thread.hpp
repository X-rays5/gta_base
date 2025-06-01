//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
#define GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
#include <base-common/concurrency/spinlock.hpp>
#include "draw/draw_queue.hpp"

namespace base::menu::render {
  class RenderThread  {
  public:
    RenderThread();
    ~RenderThread();

    bool HasInitRan() const {
      return is_initialized_;
    }

    void SetInitRan(const bool init_ran) {
      is_initialized_ = init_ran;
    }

    static void RenderMain();

  private:
    bool is_initialized_ = false;
    std::unique_ptr<std::thread> render_thread_;
  };

  inline RenderThread* kRENDER_THREAD{};
}
#endif //GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
