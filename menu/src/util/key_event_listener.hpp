//
// Created by X-ray on 01/06/2025.
//

#ifndef KEY_EVENT_LISTENER_HPP_06155902
#define KEY_EVENT_LISTENER_HPP_06155902
#include "../hooking/wndproc.hpp"
#include <ankerl/unordered_dense.h>

namespace base::menu::util {
  /**
   * KeyEventListener is an interface for classes that want to listen for key events through KeyEventWatcher.
   */
  class KeyEventListener {
  public:
    enum class ModifierKey : std::int32_t {
      kNONE,
      kSHIFT,
      kCTRL,
    };

    virtual ~KeyEventListener() = default;

    /**
     * Called when a key is pressed down through the WndProc.
     * @param vk_key Virtual key code of the key that was pressed.
     * @param modifier Modifier key that was pressed along with the key, if any.
     */
    virtual void KeyDown(std::uint32_t vk_key, ModifierKey modifier) = 0;

    /**
     * Called when a key is released through the WndProc.
     * @param vk_key Virtual key code of the key that was released.
     * @param modifier Modifier key that was pressed along with the key, if any.
     */
    virtual void KeyUp(std::uint32_t vk_key, ModifierKey modifier) = 0;
  };

  /**
   * KeyEventWatcher is a class that listens for key events through the WndProc and notifies registered KeyEventListeners.
   */
  class KeyEventWatcher final : public hooking::WndProcEventListener {

  public:
    KeyEventWatcher();
    virtual ~KeyEventWatcher() override;

    virtual void OnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

    /**
     * Adds a key event listener to the watcher.
     * @param listener Pointer to the KeyEventListener to be added.
     * @return A unique identifier for the listener that can be used to remove it later.
     */
    std::size_t AddKeyEventListener(KeyEventListener* listener);

    /**
     * Removes a key event listener from the watcher.
     * @param id Unique identifier of the KeyEventListener to be removed.
     */
    void RemoveKeyEventListener(std::size_t id);

  private:
    common::concurrency::Spinlock spinlock_;
    std::size_t wndproc_handler_id_{};
    ankerl::unordered_dense::map<std::size_t, KeyEventListener*> event_listeners_{};
    std::atomic<std::size_t> next_event_handler_id_{};
  };
  inline KeyEventWatcher* kKEY_EVENT_WATCHER{};
}

#endif //KEY_EVENT_LISTENER_HPP_06155902
