//
// Created by X-ray on 08/22/22.
//

#include "invoker.hpp"
#include "crossmap.hpp"
#include "../memory/pointers.hpp"
#include "../logger/logger.hpp"

extern "C" void	_call_asm(void* context, void* function, void* ret);

namespace rage {
  NativeCallContext::NativeCallContext() {
    m_return_value = &return_stack_[0];
    m_args = &arg_stack_[0];
  }

  void ExceptionHandler(PEXCEPTION_POINTERS e) {
    LOG_WARN("Exception caught while trying to call a native");
  }

  void Invoker::CacheHandlers() {
    for (auto&& mapping : kCROSSMAP) {
      rage::scrNativeHandler handler = gta_base::memory::kPOINTERS->GetNativeHandler(gta_base::memory::kPOINTERS->native_registration_table_, mapping.second);

      handler_cache_[mapping.first] = handler;
    }
  }

  void Invoker::BeginCall() {
    call_context_.reset();
  }

  void CallNative(rage::scrNativeHash hash, NativeCallContext* call_context, scrNativeHandler handler, PVOID native_ret, gta_base::memory::Pointers::fix_vectors_t fix_vectors) {
    __try {
      _call_asm(call_context, handler, native_ret);
      //handler(call_context);
      fix_vectors(call_context);
    } __except(ExceptionHandler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
  }

  void Invoker::EndCall(rage::scrNativeHash hash) {
    auto entry = handler_cache_.find(hash);

    if (entry != handler_cache_.end()) {
      auto handler = entry->second;

      CallNative(hash, &call_context_, handler, gta_base::memory::kPOINTERS->native_return_, gta_base::memory::kPOINTERS->FixVectors);
    } else {
      LOG_WARN("Failed to find: 0x{} native's handler", hash);
    }
  }
}
