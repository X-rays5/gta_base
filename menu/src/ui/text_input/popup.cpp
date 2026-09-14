//
// Created by X-ray on 14/09/2026.
//

#include "popup.hpp"

#include <cmath>
#include <imgui/imgui.h>
#include "../../render/renderer.hpp"
#include "../../render/render_thread.hpp"
#include "../../render/draw/draw_commands.hpp"
#include "../localization/manager.hpp"

namespace base::menu::ui::text_input {
  namespace {
    /// Share of the screen width the popup takes. Its height follows from the field it holds.
    constexpr float kWindowWidthFraction = 0.2F;
    /// Drawn after the menu and its notifications, which are at 0 and 999.
    constexpr std::size_t kRenderCallbackZIndex = 1000;
    /// Stands in for an empty title: a window needs a name, and the empty string is not one ImGui will
    /// take. Everything up to "##" is what a title bar shows, so this one shows nothing.
    constexpr const char* kWindowName = "##text_input";
  }

  Popup::Popup() {
    kPOPUP = this;

    // Absent when there is no renderer to draw the popup in, which is what a test runs with.
    if (render::kRENDER_THREAD) {
      render::kRENDER_THREAD->AddRenderCallback(kRenderCallbackZIndex, [](render::DrawQueueBuffer* draw_queue_buffer) {
        if (kPOPUP) {
          kPOPUP->Render(draw_queue_buffer);
        }
      });
    }
  }

  Popup::~Popup() {
    // Whatever was on screen is going away with the menu, and so is the callback it was waiting on,
    // which nobody is left to answer.
    Close();
    kPOPUP = nullptr;
  }

  void Popup::Open(const std::string_view title, const std::string_view initial_text, submit_cb_t on_submit, cancel_cb_t on_cancel) {
    const common::concurrency::ScopedSpinlock lock(lock_);

    // A popup that is already open is replaced rather than cancelled: the caller that just opened this
    // one is the one waiting for an answer now.
    (void)Dismiss(Action::kNone);

    is_open_ = true;
    title_ = localization::kMANAGER ? localization::kMANAGER->Localize(title) : std::string(title);

    const std::size_t length = std::min(initial_text.size(), kMaxTextLength);
    std::memcpy(buffer_.data(), initial_text.data(), length);
    buffer_[length] = '\0';

    // Bumped so that this opening's field is a field ImGui has not seen before: it keeps the caret,
    // the selection and the text of the last popup's field under that field's id, and would otherwise
    // carry them over into this one.
    ++generation_;

    on_submit_ = std::move(on_submit);
    on_cancel_ = std::move(on_cancel);

    // The field is what the player is here for, so the pointer comes up with it.
    if (render::kRENDERER) {
      render::kRENDERER->RequestShowCursor();
    }
  }

  void Popup::Close() {
    const common::concurrency::ScopedSpinlock lock(lock_);
    if (is_open_) {
      (void)Dismiss(Action::kNone);
    }
  }

  bool Popup::IsOpen() const {
    const common::concurrency::ScopedSpinlock lock(lock_);
    return is_open_;
  }

  void Popup::Submit() {
    Pending pending;
    {
      const common::concurrency::ScopedSpinlock lock(lock_);
      if (!is_open_) {
        return;
      }

      pending = Dismiss(Action::kSubmit);
    }

    Run(pending);
  }

  void Popup::Cancel() {
    Pending pending;
    {
      const common::concurrency::ScopedSpinlock lock(lock_);
      if (!is_open_) {
        return;
      }

      pending = Dismiss(Action::kCancel);
    }

    Run(pending);
  }

  void Popup::Render(render::DrawQueueBuffer* draw_queue_buffer) {
    if (!draw_queue_buffer || !IsOpen()) {
      return;
    }

    // Through the queue because the window cannot be made here: this runs on the render thread, while
    // the ImGui frame - and with it any window - belongs to the thread the queue is drawn on.
    draw_queue_buffer->AddCommand(render::RunRenderCode([this] {
      DrawWindow();
    }));
  }

  void Popup::DrawWindow() {
    Pending pending;

    {
      const common::concurrency::ScopedSpinlock lock(lock_);
      if (!is_open_) {
        return;
      }

      const ImVec2 screen = ImGui::GetIO().DisplaySize;
      // Locked to the middle of the screen, and its height is left at zero so that it is whatever the
      // field needs rather than a size of its own.
      ImGui::SetNextWindowPos({screen.x / 2.F, screen.y / 2.F}, ImGuiCond_Always, {0.5F, 0.5F});
      ImGui::SetNextWindowSize({std::floor(screen.x * kWindowWidthFraction), 0.F}, ImGuiCond_Always);

      constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

      if (ImGui::Begin(title_.empty() ? kWindowName : title_.c_str(), nullptr, flags)) {
        // Until the field is the active item there is nothing that takes typing, and the popup would
        // sit there taking every key but Escape and doing nothing with it.
        if (!ImGui::IsAnyItemActive()) {
          ImGui::SetKeyboardFocusHere();
        }

        ImGui::PushID(static_cast<int>(generation_));

        if (ImGui::InputText("##text", buffer_.data(), buffer_.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
          pending = Dismiss(Action::kSubmit);
        } else if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
          // Read here rather than left to the field, which only gives Escape up when the field itself
          // is what has the keyboard.
          pending = Dismiss(Action::kCancel);
        }

        ImGui::PopID();
      }
      ImGui::End();
    }

    // With the lock down, so that a callback is free to open another popup, or to call in here again.
    Run(pending);
  }

  Popup::Pending Popup::Dismiss(const Action action) {
    is_open_ = false;

    if (render::kRENDERER) {
      render::kRENDERER->RequestHideCursor();
    }

    Pending pending;
    if (action == Action::kSubmit) {
      pending.on_submit = std::move(on_submit_);
      pending.text = buffer_.data();
    } else if (action == Action::kCancel) {
      pending.on_cancel = std::move(on_cancel_);
    }

    // Cleared whichever way the popup came down, so that a callback left over from a popup that is
    // gone cannot be run by the next one.
    on_submit_ = nullptr;
    on_cancel_ = nullptr;

    return pending;
  }

  void Popup::Run(const Pending& pending) {
    if (pending.on_submit) {
      pending.on_submit(pending.text);
    } else if (pending.on_cancel) {
      pending.on_cancel();
    }
  }
}
