//
// Created by X-ray on 05/11/2023.
//

#include "render_thread.hpp"
#include "renderer.hpp"

namespace base::menu::render {
  RenderThread::RenderThread() {
    render_thread_ = std::make_unique<std::thread>(RenderThread::RenderMain);
    kRENDER_THREAD = this;
  }

  RenderThread::~RenderThread() {
    kRENDER_THREAD = nullptr;

    if (render_thread_ && render_thread_->joinable()) {
      LOG_INFO("Waiting for render thread to finish...");
      render_thread_->join();
      LOG_INFO("Render thread finished.");
    } else {
      LOG_DEBUG("Render thread was not running or already joined.");
    }
  }

  void RenderThread::RenderMain() {
    SetThreadDescription(GetCurrentThread(), L"RenderLoop");
    LOG_INFO("Render thread started.");
    while (globals::kRUNNING) {
      if (kRENDER_THREAD && kRENDERER) {
        const auto buffer = kRENDERER->GetDrawQueueBuffer();

        for (auto& [_, cb_] : kRENDER_THREAD->GetRenderCallbacks()) {
          if (!buffer) {
            if (globals::kRUNNING) {
              LOG_ERROR("DrawQueueBuffer is null. Skipping render tick.");
              break;
            }

            LOG_INFO("Render thread is assuming shutdown. Exiting render thread...");
            break;
          }

          cb_(buffer);
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
