//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
#define GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
#include <functional>
#include <vector>
#include <algorithm>
#include "../util/spinlock.hpp"
#include "draw.hpp"

namespace base::menu::render {
  class Thread {
  public:
    struct RenderCB {
      using render_cb_t = std::function<void(DrawQueueBuffer*)>;

      std::size_t z_idx_;
      render_cb_t cb_;
    };

  public:
    Thread();
    ~Thread();

    static void RenderMain();

    void AddRenderCallback(const std::size_t z_idx, const RenderCB::render_cb_t& cb) {
      const auto lock = base::util::ScopedSpinlock(callback_lock_);
      render_callbacks_.push_back({z_idx, cb});

      // While this really isn't optimal, render callbacks aren't constantly added.
      std::sort(render_callbacks_.begin(), render_callbacks_.end(), [](const RenderCB& a, const RenderCB& b) {
        return a.z_idx_ < b.z_idx_;
      });
    }

    std::vector<RenderCB>& GetRenderCallbacks() {
      const auto lock = base::util::ScopedSpinlock(callback_lock_);
      return render_callbacks_;
    }

  private:
    base::util::Spinlock callback_lock_;
    std::vector<RenderCB> render_callbacks_;
  };

  // skipcq: CXX-W2009
  inline Thread* kTHREAD{};
}
#endif //GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
