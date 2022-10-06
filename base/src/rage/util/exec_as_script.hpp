//
// Created by X-ray on 08/02/22.
//

#pragma once
#ifndef GTA_BASE_EXEC_AS_SCRIPT_HPP
#define GTA_BASE_EXEC_AS_SCRIPT_HPP
#include "../joaat.hpp"
#include "../../memory/pointers.hpp"

namespace rage::util {
    template <typename F, typename ...Args>
    void ExecuteAsScript(rage::joaat_t script_hash, F &&callback, Args &&...args) {
      auto tls_ctx = rage::tlsContext::get();
      for (auto thread : *gta_base::memory::kPOINTERS->script_threads_)
      {
        if (!thread || !thread->m_context.m_thread_id || thread->m_context.m_script_hash != script_hash)
          continue;

        auto og_thread = tls_ctx->m_script_thread;

        tls_ctx->m_script_thread = thread;
        tls_ctx->m_is_script_thread_active = true;

        std::invoke(std::forward<F>(callback), std::forward<Args>(args)...);

        tls_ctx->m_script_thread = og_thread;
        tls_ctx->m_is_script_thread_active = og_thread != nullptr;

        return;
      }
    }
  }
#endif //GTA_BASE_EXEC_AS_SCRIPT_HPP
