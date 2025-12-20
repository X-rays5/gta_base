//
// Created by X-ray on 19/12/2025.
//

#include "invoker.hpp"
#include "../memory/pointers.hpp"

namespace base::menu::natives {
  Invoker::Invoker() {
    memcpy(native_handlers_.data(), NATIVE_CROSSMAP, NATIVE_CROSSMAP_SIZE);
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
    if (native_idx >= NATIVE_CROSSMAP_SIZE) {
      context_.SetReturnValue(0);
      return;
    }

    const auto handler = kINVOKER->native_handlers_[native_idx];
    handler(&context_);
    context_.FixVectors();
  }
}
