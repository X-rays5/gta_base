//
// Created by X-ray on 20/12/2025.
//

#include "script.hpp"
#include <rage/tls_context.hpp>
#include "../memory/pointers.hpp"

namespace base::menu::game {
  rage::script::Thread* FindScriptThread(common::conversion::joaat_t hash) {
    for (const auto& thread : *memory::kPOINTERS->scriptThreads_)
    {
      if (thread && thread->ctx.thread_id && thread->script_hash == hash)
      {
        return thread;
      }
    }

    return nullptr;
  }

  void RunAsScript(rage::script::Thread* thread, const std::function<void()>& callback) {
    const auto og_thread = rage::TlsContext::Get()->current_script_thread;
    const auto og_running_in_scrthread = rage::TlsContext::Get()->script_thread_active;
    rage::TlsContext::Get()->current_script_thread = thread;
    rage::TlsContext::Get()->script_thread_active = true;
    callback();
    rage::TlsContext::Get()->script_thread_active = og_running_in_scrthread;
    rage::TlsContext::Get()->current_script_thread = og_thread;
  }
}