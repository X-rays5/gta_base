//
// Created by X-ray on 05/11/2023.
//

#include "render_thread.hpp"
#include "renderer.hpp"

namespace base::menu::render {
  RenderThread::RenderThread() {
    kRENDER_THREAD = this;
  }

  RenderThread::~RenderThread() {
    kRENDER_THREAD = nullptr;
  }

  void RenderThread::RenderMain() {
    SetThreadDescription(GetCurrentThread(), L"RenderLoop");
    LOG_INFO("Render thread started.");
    while (globals::kRUNNING) {
      if (kRENDER_THREAD && kRENDERER) [[likely]] {
        const auto buffer = kRENDERER->GetDrawQueueBuffer();
        const auto callbacks = kRENDER_THREAD->GetRenderCallbacks();

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
