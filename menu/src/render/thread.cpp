//
// Created by X-ray on 05/11/2023.
//

#include "thread.hpp"
#include "manager.hpp"

namespace base::render {
  Thread::Thread() {
    kTHREAD = this;
  }

  Thread::~Thread() {
    kTHREAD = nullptr;
  }

  void Thread::RenderMain() {
    while (globals::kRUNNING) {
      if (kTHREAD && kMANAGER) {
        auto buffer = kMANAGER->GetDrawQueueBuffer();

        for (auto& cb : kTHREAD->GetRenderCallbacks()) {
          if (!buffer) {
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
      }
    }
  }
}