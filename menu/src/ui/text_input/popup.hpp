//
// Created by X-ray on 14/09/2026.
//

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <base-common/concurrency/spinlock.hpp>

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::menu::ui::text_input {
  /**
   * The menu's one popup for typing something in: a window locked to the centre of the screen holding
   * nothing but a text field. Enter submits what was typed, Escape throws it away.
   *
   * Not a component and not tied to one: whatever needs a string - a component, a script, a hotkey -
   * opens the single instance through kPOPUP. While it is open MenuRenderer leaves the keyboard alone,
   * so the keys that edit the field are not also the keys that walk the menu underneath.
   */
  class Popup {
  public:
    /// What the popup hands the text to. Cancel is passed nothing: the player threw the text away.
    using submit_cb_t = std::function<void(const std::string& text)>;
    using cancel_cb_t = std::function<void()>;

    /// How much can be typed. The field is edited in a buffer of this size, and ImGui does not grow it.
    static constexpr std::size_t kMaxTextLength = 256;

  public:
    Popup();
    ~Popup();

    /**
     * Opens the popup, replacing one that is already open - the cancel callback of the popup being
     * replaced does not run, since the player is not the one dismissing it.
     *
     * @param title Shown in the window's title bar, localized like every other piece of menu text.
     * @param initial_text What the field starts with.
     * @param on_submit Called with the text when the player presses Enter.
     * @param on_cancel Called when the player presses Escape.
     */
    void Open(std::string_view title, std::string_view initial_text, submit_cb_t on_submit, cancel_cb_t on_cancel = {});

    /// Dismisses it and runs neither callback. Nothing happens when it is not open.
    void Close();

    [[nodiscard]] bool IsOpen() const;

    /// Accepts the text as Enter would. The callback runs on the calling thread.
    void Submit();

    /// Throws the text away as Escape would. The callback runs on the calling thread.
    void Cancel();

  private:
    enum class Action {
      kNone,
      kSubmit,
      kCancel,
    };

    /// What the key that closed the popup left behind, to be run once the lock is down.
    struct Pending {
      submit_cb_t on_submit;
      cancel_cb_t on_cancel;
      std::string text;
    };

    /// Registered as a render callback by the constructor, so that opening the popup is all the rest
    /// of the menu ever has to do with it.
    void Render(render::DrawQueueBuffer* draw_queue_buffer);

    /// Draws the window and, when a key closed the popup, takes it down. Runs on the thread that
    /// draws the frame, which is the only place a window may be created.
    void DrawWindow();

    /**
     * Takes the popup down and hands back the callback `action` asks for - neither of them when the
     * popup is being replaced or closed rather than answered. Call with the lock held.
     */
    [[nodiscard]] Pending Dismiss(Action action);

    /// Runs what Dismiss handed back, with the lock down so that a callback is free to open another
    /// popup - or to call back in here at all - without being overwritten by this one being cleared.
    static void Run(const Pending& pending);

    mutable common::concurrency::Spinlock lock_;
    bool is_open_ = false;
    /// Counts the openings, and names this opening's field: ImGui keeps a field's caret, selection and
    /// text under its id, so a fresh id is what stops one opening from inheriting the last one's.
    std::uint64_t generation_ = 0;
    std::string title_;
    std::array<char, kMaxTextLength + 1> buffer_{};
    submit_cb_t on_submit_;
    cancel_cb_t on_cancel_;
  };

  inline Popup* kPOPUP{};
}
