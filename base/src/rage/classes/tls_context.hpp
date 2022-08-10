//
// Created by X-ray on 08/08/22.
//

#pragma once
#ifndef GTA_BASE_TLS_CONTEXT_HPP
#define GTA_BASE_TLS_CONTEXT_HPP
#include <intrin.h>
#include "sys_mem_allocator.hpp"
#include "script_thread.hpp"
#include "fwddec.hpp"

namespace rage
{
  class tlsContext {
  public:
    char m_padding1[0xC8]; // 0x00
    sysMemAllocator* m_allocator; // 0xC8
    char m_padding2[0x770]; // 0xD0
    scrThread* m_script_thread; // 0x840
    bool m_is_script_thread_active; // 0x848

    static tlsContext* get()
    {
      return *reinterpret_cast<tlsContext**>(__readgsqword(0x58));
    }
  };

  static_assert(sizeof(tlsContext) == 0x850);
}

#endif //GTA_BASE_TLS_CONTEXT_HPP
