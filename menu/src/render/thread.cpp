//
// Created by X-ray on 05/11/2023.
//

#include "thread.hpp"

namespace base::render {
  Thread::Thread() {
    kTHREAD = this;
  }

  Thread::~Thread() {
    kTHREAD = nullptr;
  }

  void Thread::RenderMain() {
    while (globals::kRUNNING) {
      if (kTHREAD) {
        auto buffer = kMANAGER->GetDrawQueueBuffer();

        for (auto& cb : kTHREAD->GetRenderCallbacks()) {
          cb(buffer);
        }

        buffer->SwapBuffers();
      }
    }
  }
}