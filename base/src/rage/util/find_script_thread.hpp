//
// Created by X-ray on 10/05/22.
//

#pragma once
#ifndef GTA_BASE_FIND_SCRIPT_THREAD_HPP
#define GTA_BASE_FIND_SCRIPT_THREAD_HPP
#include "../../memory/pointers.hpp"
#include "../classes/script_thread.hpp"
#include "../joaat.hpp"

namespace rage {
  inline GtaThread* FindScriptThread(rage::joaat_t script_hash) {
    for (auto thread : *gta_base::memory::kPOINTERS->script_threads_) {
      if (thread->m_context.m_script_hash == script_hash)
        return thread;
    }

    return nullptr;
  }
}
#endif //GTA_BASE_FIND_SCRIPT_THREAD_HPP
