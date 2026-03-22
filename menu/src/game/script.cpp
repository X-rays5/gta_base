//
// Created by X-ray on 20/12/2025.
//

#include "script.hpp"
#include <rage/tls_context.hpp>
#include "../memory/pointers.hpp"

namespace base::menu::game {
  namespace {
    class ScriptTlsGuard {
    public:
      explicit ScriptTlsGuard(rage::script::Thread* new_thread) {
        tls_ = rage::TlsContext::Get();
        old_thread_ = tls_->current_script_thread;
        old_running_in_scrthread_ = tls_->script_thread_active;

        tls_->current_script_thread = new_thread;
        tls_->script_thread_active = true;
      }

      ~ScriptTlsGuard() {
        if (!tls_)
          return;

        tls_->script_thread_active = old_running_in_scrthread_;
        tls_->current_script_thread = old_thread_;
      }

      ScriptTlsGuard(const ScriptTlsGuard&) = delete;
      ScriptTlsGuard& operator=(const ScriptTlsGuard&) = delete;

    private:
      rage::TlsContext* tls_{};
      rage::script::Thread* old_thread_{};
      bool old_running_in_scrthread_{};
    };
  }

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
    if (!thread)
      return;

    ScriptTlsGuard tls_guard(thread);
      callback();
  }
}