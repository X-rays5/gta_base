//
// Created by X-ray on 05/11/2023.
//

#pragma once
#ifndef GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
#define GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
#include <functional>
#include <vector>
#include "draw.hpp"

namespace base::menu::render {
  class Thread {
  public:
    using render_cb_t = std::function<void(DrawQueueBuffer*)>;

  public:
    Thread();
    ~Thread();

    static void RenderMain();

    inline void AddRenderCallback(const render_cb_t& cb) {
      render_callbacks_.push_back(cb);
    }

    inline std::vector<render_cb_t>& GetRenderCallbacks() {
      return render_callbacks_;
    }

  private:
    std::vector<render_cb_t> render_callbacks_;
  };

  inline Thread* kTHREAD{};
}
#endif //GTA_BASE_THREAD_E1B5110495AF4A36A7A7D235FBB8EC7D_HPP
