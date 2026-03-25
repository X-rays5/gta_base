//
// Created by X-ray on 30/05/2025.
//

#ifndef WNDPROC_HPP_05224335
#define WNDPROC_HPP_05224335
#include <ankerl/unordered_dense.h>
#include <base-common/concurrency/spinlock.hpp>
#include "helpers/detour.hpp"

namespace base::menu::hooking {
  class WndProcEventListener {
  public:
    virtual ~WndProcEventListener() = default;

    virtual void OnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
  };

  class WndProc {
  private:
    class HandlerBase {
    public:
      virtual ~HandlerBase() = default;
      virtual void Call(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
    };

    template <typename Handler>
    class ConcreteHandler : public HandlerBase {
    private:
      Handler handler_;

    public:
      explicit ConcreteHandler(const Handler& handler) : handler_(handler) {}

      void Call(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override {
        // Handle different types of handlers
        if constexpr (std::is_pointer_v<Handler> && std::is_base_of_v<WndProcEventListener, std::remove_pointer_t<Handler>>) {
          // It's a WndProcEventListener pointer
          handler_->OnWndProc(hwnd, msg, wparam, lparam);
        } else if constexpr (std::is_function_v<std::remove_pointer_t<Handler>>) {
          // It's a function pointer
          handler_(hwnd, msg, wparam, lparam);
        } else {
          // It's a callable object (functor, lambda, etc.)
          handler_(hwnd, msg, wparam, lparam);
        }
      }
    };

  public:
    WndProc();
    ~WndProc();

    void HandleWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    DetourHook* GetHook() const { return wndproc_hook_.get(); }

    template <typename Handler>
    std::size_t AddWndProcHandler(Handler&& handler) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      const std::size_t id = next_event_handler_id_.fetch_add(1);
      wnd_proc_handlers_[id] = std::make_unique<ConcreteHandler<Handler>>(std::forward<Handler>(handler));

      LOG_DEBUG("Added WndProc handler with id: {}", id);
      return id;
    }

    std::size_t AddWndProcHandler(WndProcEventListener* listener) {
      struct ListenerCallable {
        WndProcEventListener* listener;

        void operator()(const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam) const {
          listener->OnWndProc(hwnd, msg, wparam, lparam);
        }
      };

      // Call the template overload with the callable wrapper
      return AddWndProcHandler(ListenerCallable{listener});
    }

    void RemoveWndProcHandler(const std::size_t id) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      wnd_proc_handlers_.erase(id);
    }

  private:
    std::unique_ptr<DetourHook> wndproc_hook_;
    ankerl::unordered_dense::map<std::size_t, std::unique_ptr<HandlerBase>> wnd_proc_handlers_;
    std::atomic<std::size_t> next_event_handler_id_ = 0;
    common::concurrency::RecursiveSpinlock spinlock_;
  };

  inline WndProc* kWNDPROC{};
}

#endif //WNDPROC_HPP_05224335