//
// Created by X-ray on 30/05/2025.
//

#ifndef WNDPROC_HPP_05224335
#define WNDPROC_HPP_05224335
#include <ankerl/unordered_dense.h>
#include <base-common/concurrency/spinlock.hpp>

namespace base::menu::hooking {
  class WndProcEventListener {
  public:
    virtual ~WndProcEventListener() = default;

    virtual void OnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
  };

  class WndProc {
  public:
    WndProc();
    ~WndProc();

    void HandleWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    WNDPROC GetOriginalWndProc() const {
      return og_wnd_proc_;
    }

    std::size_t AddWndProcHandler(WndProcEventListener* handler) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      const std::size_t id = next_event_handler_id_.fetch_add(1); // Shouldn't run out realistically
      wnd_proc_handlers_[id] = handler;

      LOG_DEBUG("Added WndProc handler with id: {}", id);
      return id;
    }

    void RemoveWndProcHandler(const std::size_t id) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      wnd_proc_handlers_.erase(id);
    }

  private:
    WNDPROC og_wnd_proc_{};
    ankerl::unordered_dense::map<std::size_t, WndProcEventListener*> wnd_proc_handlers_;
    std::atomic<std::size_t> next_event_handler_id_ = 0;
    common::concurrency::RecursiveSpinlock spinlock_;
  };

  inline WndProc* kWNDPROC{};
}

#endif //WNDPROC_HPP_05224335
