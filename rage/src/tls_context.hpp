//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <intrin.h>

#include "script/thread.hpp"

namespace rage {
  class TlsContext {
  public:
    char pad1[0x7A0];
    script::Thread* current_script_thread;
    bool script_thread_active;

    static TlsContext* Get()
    {
      constexpr std::uint32_t TlsIndex = 0x0;
      return *reinterpret_cast<TlsContext**>(__readgsqword(0x58) + TlsIndex);
    }
  };
  static_assert(sizeof(TlsContext) == 0x7B0);
}