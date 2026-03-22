//
// Created by X-ray on 30/05/2025.
//

#ifndef WNDPROC_HPP_05224335
#define WNDPROC_HPP_05224335
#include <ankerl/unordered_dense.h>
#include <base-common/concurrency/spinlock.hpp>
#include <functional>
#include "helpers/detour.hpp"

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

    DetourHook* GetHook() const { return wndproc_hook_.get(); }

    std::size_t AddWndProcHandler(WndProcEventListener* listener) {
      // Create a wrapper that converts the listener to WNDPROC signature
      auto wrapper = [listener](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        listener->OnWndProc(hwnd, msg, wparam, lparam);
        return 0;
      };

      // Call the template overload to store the wrapper
      return AddWndProcHandler(wrapper);
    }

    template <std::predicate<HWND, UINT, WPARAM, LPARAM> Handler>
    std::size_t AddWndProcHandler(const Handler& handler) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      const std::size_t id = next_event_handler_id_.fetch_add(1);
      wnd_proc_handlers_[id] = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>(
        [handler](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
          handler(hwnd, msg, wparam, lparam);
          return 0;
        }
      );

      LOG_DEBUG("Added WndProc handler with id: {}", id);
      return id;
    }

    void RemoveWndProcHandler(const std::size_t id) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      wnd_proc_handlers_.erase(id);
    }

  private:
    std::unique_ptr<DetourHook> wndproc_hook_;
    ankerl::unordered_dense::map<std::size_t, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>> wnd_proc_handlers_;
    std::atomic<std::size_t> next_event_handler_id_ = 0;
    common::concurrency::RecursiveSpinlock spinlock_;
  };

  inline WndProc* kWNDPROC{};
}

#endif //WNDPROC_HPP_05224335