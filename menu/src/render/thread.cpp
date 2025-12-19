//
// Created by X-ray on 05/11/2023.
//

#include "thread.hpp"
#include "renderer.hpp"

namespace base::menu::render {
  Thread::Thread() {
    kTHREAD = this;
  }

  Thread::~Thread() {
    kTHREAD = nullptr;
  }

  void Thread::RenderMain() {
    SetThreadDescription(GetCurrentThread(), L"RenderLoop");
    LOG_INFO("Render thread started.");
    while (globals::kRUNNING) {
      if (kTHREAD && kRENDERER) [[likely]] {
        const auto buffer = kRENDERER->GetDrawQueueBuffer();
        const auto callbacks = kTHREAD->GetRenderCallbacks();

        for (const auto& [z_idx, cb] : callbacks) {
          if (!buffer) [[unlikely]] {
            if (globals::kRUNNING) {
              LOG_ERROR("DrawQueueBuffer is null. Skipping render tick.");
              break;
            }

            LOG_INFO("Render thread is assuming shutdown. Exiting render thread...");
            break;
          }

          cb(buffer);
        }

        if (buffer)
          buffer->SwapBuffers();
      } else {
        [[unlikely]]
        LOG_DEBUG("nullptr");
      }
    }
    LOG_INFO("Exiting render thread.");
  }
}
