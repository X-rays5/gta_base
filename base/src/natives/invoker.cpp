//
// Created by X-ray on 08/22/22.
//

#include "invoker.hpp"
#include "crossmap.hpp"
#include "../memory/pointers.hpp"

extern "C" void spoof_call(void* context, void* function, void* ret);

//#define DEBUG_INVOKER

namespace rage {
  NativeCallContext::NativeCallContext() {
    m_return_value = &return_stack_[0];
    m_args = &arg_stack_[0];
  }

  void Invoker::CacheHandlers() {
    #ifdef DISABLE_INVOKER
    return;
    #endif

    if (!handler_cache_.empty())
      return;

    for (const auto& mapping: kCROSSMAP) {
      rage::scrNativeHandler handler = gta_base::memory::kPOINTERS->GetNativeHandler(gta_base::memory::kPOINTERS->native_registration_table_, mapping.second);
      if (!handler) {
        LOG_ERROR("{:X}:{:X} handler invalid: {:X}", mapping.first, mapping.second, reinterpret_cast<std::uintptr_t>(static_cast<void*>(handler)));
        continue;
      }

      handler_cache_[mapping.first] = handler;
    }
  }

  void Invoker::BeginCall() {
    call_context_.reset();
  }

  void CallNative(rage::scrNativeHash hash, NativeCallContext* call_context, scrNativeHandler handler) {
    __try{
      spoof_call(call_context, handler, gta_base::memory::kPOINTERS->native_return_);
      #ifdef DEBUG_INVOKER
      LOG_DEBUG("calling native: {:X} handler: {:X}", hash, reinterpret_cast<std::uintptr_t>(static_cast<void*>(handler)));
      gta_base::kLOGGER->Flush();
      #endif

      gta_base::memory::kPOINTERS->FixVectors(call_context);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
      LOG_WARN("Exception caught while trying to call native. hash: {}, handler: {}", hash, reinterpret_cast<std::uintptr_t>(static_cast<void*>(handler)));
    }
  }

  void Invoker::EndCall(rage::scrNativeHash hash) {
    auto entry = handler_cache_.find(hash);

    if (entry != handler_cache_.end()) {
      auto handler = entry->second;
      if (!handler) {
        handler = gta_base::memory::kPOINTERS->GetNativeHandler(gta_base::memory::kPOINTERS->native_registration_table_, kCROSSMAP.find(hash)->second);
        if (!handler) {
          LOG_CRITICAL("native handler missing for: {:X}", hash);
          return;
        }
      }

      CallNative(hash, &call_context_, handler);
    } else {
      LOG_WARN("Failed to find: 0x{:X} native's handler", hash);
    }
  }
}
