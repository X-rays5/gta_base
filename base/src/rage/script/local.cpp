//
// Created by X-ray on 09/18/22.
//

#include "local.hpp"

namespace rage {
  namespace script {
    Local::Local(rage::scrThread* thread, std::size_t idx) : idx_(idx), stack_(thread->m_stack) {}
    Local::Local(PVOID stack, std::size_t idx) : idx_(idx), stack_(stack) {}

    Local Local::At(std::ptrdiff_t idx) {
      return Local(stack_, idx_ + idx);
    }

    Local Local::At(std::ptrdiff_t idx, std::size_t arr_size) {
      return Local(stack_, idx_ + 1 + (idx * arr_size));
    }

    void* Local::Get() {
      return reinterpret_cast<uintptr_t*>((uintptr_t)stack_ + (idx_ * sizeof(uintptr_t)));
    }
  }
}