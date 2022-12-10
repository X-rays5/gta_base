//
// Created by X-ray on 08/22/22.
//

#include "invoker.hpp"
#include "crossmap.hpp"
#include "../memory/pointers.hpp"

extern "C" void	spoof_call(void* context, void* function, void* ret);

namespace rage {
  NativeCallContext::NativeCallContext() {
    m_return_value = &return_stack_[0];
    m_args = &arg_stack_[0];
  }

  void ExceptionHandler([[maybe_unused]] PEXCEPTION_POINTERS e, rage::scrNativeHash hash) {
    LOG_WARN("Exception caught while trying to call native with hash: 0x{:X}", hash);
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

  void CallNative(rage::scrNativeHash hash, NativeCallContext* call_context, scrNativeHandler handler) {
    __try {

      spoof_call(call_context, handler, gta_base::memory::kPOINTERS->native_return_);
      //handler(call_context);

      gta_base::memory::kPOINTERS->FixVectors(call_context);
    } __except(ExceptionHandler(GetExceptionInformation(), hash), EXCEPTION_EXECUTE_HANDLER) {}
  }

  void Invoker::EndCall(rage::scrNativeHash hash) {
    auto entry = handler_cache_.find(hash);

    if (entry != handler_cache_.end()) {
      auto handler = entry->second;

      CallNative(hash, &call_context_, handler);
    } else {
      LOG_WARN("Failed to find: 0x{:X} native's handler", hash);
    }
  }
}
