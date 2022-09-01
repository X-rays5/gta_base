//
// Created by X-ray on 08/22/22.
//

#include "invoker.hpp"
#include "crossmap.hpp"
#include "../memory/pointers.hpp"
#include "../logger/logger.hpp"
#include "spoofer/spoof.hpp"

namespace rage {
  NativeCallContext::NativeCallContext() {
    m_return_value = &return_stack_[0];
    m_args = &arg_stack_[0];
  }

  void ExceptionHandler(PEXCEPTION_POINTERS e, rage::scrNativeHash hash) {
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

  void CallNative(rage::scrNativeHash hash, NativeCallContext* call_context, scrNativeHandler handler, PVOID native_ret, gta_base::memory::Pointers::fix_vectors_t fix_vectors) {
    __try {
      static char og_native_ret[2];
      constexpr static const std::uint8_t patch[2] = { (std::uint8_t)0xFF, (std::uint8_t)0x23 };

      std::copy_n((char*)native_ret, 2, og_native_ret);
      std::copy(std::begin(patch), std::end(patch), (char*)native_ret);

      spoof_call(native_ret, static_cast<void(*)(rage::scrNativeCallContext*)>(handler), static_cast<rage::scrNativeCallContext*>(call_context));
      std::copy(std::begin(og_native_ret), std::end(og_native_ret), (char*)native_ret);

      fix_vectors(call_context);
    } __except(ExceptionHandler(GetExceptionInformation(), hash), EXCEPTION_EXECUTE_HANDLER) {}
  }

  void Invoker::EndCall(rage::scrNativeHash hash) {
    auto entry = handler_cache_.find(hash);

    if (entry != handler_cache_.end()) {
      auto handler = entry->second;

      CallNative(hash, &call_context_, handler, gta_base::memory::kPOINTERS->native_return_, gta_base::memory::kPOINTERS->FixVectors);
    } else {
      LOG_WARN("Failed to find: 0x{:X} native's handler", hash);
    }
  }
}
