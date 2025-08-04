//
// Created by X-ray on 05/11/2023.
//

#include "render_thread.hpp"
#include "renderer.hpp"
#include "../scripts/script_manager.hpp"

namespace base::menu::render {
  RenderThread::RenderThread() {
    render_thread_ = std::make_unique<std::thread>(RenderMain);
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
    SetThreadDescription(GetCurrentThread(), L"MenuRenderThread");

    LOG_INFO("Render thread started.");
    while (globals::kRUNNING) {
      if (kRENDER_THREAD && kRENDERER) {
        if (!scripts::kSCRIPTMANAGER) {
          std::this_thread::yield();
          continue;
        }

        if (!kRENDER_THREAD->HasInitRan()) {
          kRENDER_THREAD->SetInitRan(true);
          scripts::kSCRIPTMANAGER->InitScripts(scripts::BaseScript::Type::MenuRenderThread);
        }

        scripts::kSCRIPTMANAGER->TickScripts(scripts::BaseScript::Type::MenuRenderThread);

        if (const auto buffer = kRENDERER->GetDrawQueueBuffer())
          buffer->SwapBuffers();
      } else {
        [[unlikely]]
        LOG_DEBUG("nullptr");
      }
    }

    LOG_INFO("Exiting render thread.");
  }
}
