//
// Created by X-ray on 19/12/2025.
//

#include "invoker.hpp"
#include "../memory/pointers.hpp"

namespace base::menu::natives {
  Invoker::Invoker() {
    memcpy(native_handlers_.data(), NATIVE_CROSSMAP, sizeof(NATIVE_CROSSMAP));
    const auto program = static_cast<rage::script::Program*>(calloc(1, sizeof(rage::script::Program)));
    program->m_NativeCount = NATIVE_CROSSMAP_SIZE;
    program->m_NativeEntrypoints = native_handlers_.data();
    memory::kPOINTERS->init_native_tables_(program);
    free(program);

    kINVOKER = this;
  }

  Invoker::~Invoker() {
    kINVOKER = nullptr;
  }

  void Invoker::NativeCall::EndCall(const std::uint64_t native_idx) {
    if (kINVOKER == nullptr || native_idx >= NATIVE_CROSSMAP_SIZE) {
      LOG_WARN("Invalid native index: {}", native_idx);
      context_.SetReturnValue(std::uint64_t{0});
      return;
    }

    const auto handler = kINVOKER->native_handlers_[native_idx];
    if (!handler) {
        LOG_WARN("No handler for native index: {}", native_idx);
      context_.SetReturnValue(std::uint64_t{0});
      return;
    }

    handler(&context_);
    context_.FixVectors();
  }
}
