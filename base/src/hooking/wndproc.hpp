//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_WNDPROC_HPP
#define GTABASE_WNDPROC_HPP
#include <functional>
#include "../misc/spinlock.hpp"

namespace gta_base::hooking {
  class WndProcHook {
  public:
    using wnd_proc_t = std::function<void(HWND, UINT, WPARAM, LPARAM)>;

  public:
    WndProcHook();
    ~WndProcHook();

    inline void AddHandler(const wnd_proc_t& wndproc) {
      misc::ScopedSpinlock lock(spinlock_);
      wnd_proc_handlers_.emplace_back(wndproc);
    }

    inline void Enable() {
      if (og_wndproc_)
        return;

      og_wndproc_ = (WNDPROC) SetWindowLongPtr(common::GetHwnd(globals::target_window_class, globals::target_window_name), GWLP_WNDPROC, (LONG_PTR) WndProcHook::WndProc);
    }

    inline void Disable() {
      if (!og_wndproc_)
        return;

      auto hwnd = common::GetHwnd(globals::target_window_class, globals::target_window_name);

      LONG_PTR old_wndproc = SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) og_wndproc_);
      if ((LONG_PTR) og_wndproc_ != old_wndproc)
        LOG_DEBUG("WNDPROC unhooked");
      else
        LOG_CRITICAL("Failed to disable WNDPROC hook (0x{:X} != 0x{:X})", (LONG_PTR) og_wndproc_, old_wndproc);

      og_wndproc_ = nullptr;
    }

  private:
    misc::Spinlock spinlock_;
    WNDPROC og_wndproc_ = nullptr;
    std::vector<wnd_proc_t> wnd_proc_handlers_;

  private:
    static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM parameter_uint_ptr, LPARAM parameter_long_ptr);
  };

  inline WndProcHook* kWND_PROC_HOOK{};
}
#endif //GTABASE_WNDPROC_HPP
