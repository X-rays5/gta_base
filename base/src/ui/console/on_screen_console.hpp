//
// Created by X-ray on 01/12/23.
//

#pragma once
#ifndef GTA_BASE_ON_SCREEN_CONSOLE_C13ACFACDEAF44CBB876D2D0517CAD37_HPP
#define GTA_BASE_ON_SCREEN_CONSOLE_C13ACFACDEAF44CBB876D2D0517CAD37_HPP
#include "imgui.h"
#include "../../d3d/draw.hpp"
#include "../../misc/thread_pool.hpp"
#include "../../misc/timed_input.hpp"
#include "../manager.hpp"

namespace gta_base::ui {
  // log_buff_size amount: of lines to store before needing to make room
  // log_buff_scroll_count: log_buff_size / log_buff_scroll_count amount of items removed when needing more room
  template<std::size_t log_buff_size, std::size_t log_buff_scroll_count>
  class OnScreenConsole;

  using OnScreenConsoleDefault = OnScreenConsole<200, 4>;
  inline OnScreenConsoleDefault* kON_SCREEN_CONSOLE{};

  template<std::size_t log_buff_size, std::size_t log_buff_scroll_count>
  class OnScreenConsole {
  public:
    OnScreenConsole() {
      static_assert(log_buff_size > 1);
      static_assert(log_buff_scroll_count > 1);

      input_show_window_ = std::make_unique<util::TimedInput>(VK_INSERT, 250);

      kON_SCREEN_CONSOLE = this;
    }

    ~OnScreenConsole() {
      kON_SCREEN_CONSOLE = nullptr;
    }

    FORCE_INLINE void Print(const std::string& txt) {
      if (txt.empty())
        return;

      misc::kTHREAD_POOL->AddJob([this, txt] {
        auto lines = common::SplitStr(txt, "\n");
        if (lines.empty())
          return;

        for (auto&& line : lines) {
          while (!line.empty() && line.back() == '\n') {
            line.pop_back();
          }
        }

        std::unique_lock lock(mtx_);
        if (lines.size() == 1)
          ProcessedPrint(lines[0]);
        else
          ProcessedPrint(lines);
      });
    }

    FORCE_INLINE bool GetShowWindow() {
      return render_window_;
    }

    FORCE_INLINE void SetShowWindow(bool val) {
      if (render_window_ != val) {
        if (render_window_) {
          kMANAGER->PopBlockInput();
        } else {
          kMANAGER->PushBlockInput();
        }
      }

      render_window_ = val;
    }

    FORCE_INLINE void ToggleWindow() {
      SetShowWindow(!GetShowWindow());
    }

    void Tick() {
      static const std::string window_title = fmt::format("{}##console_window", globals::name);

      if (input_show_window_->Get())
        ToggleWindow();

      if (render_window_) {
        ui::kMANAGER->GetDrawList()->AddCommand(d3d::draw::DrawCallback([=] {
          ImGui::SetNextWindowSize(d3d::draw::ScaleToScreen({0.5, 0.8}), ImGuiCond_Once);
          ImGui::SetNextWindowPos(d3d::draw::ScaleToScreen({0.05, 0.05}), ImGuiCond_Once);

          if (ImGui::Begin(window_title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetWindowFontScale(0.85);

            auto space = ImGui::GetContentRegionAvail();
            auto log_child_y_size = space.y * 0.96f;
            auto cmd_child_y_size = space.y - log_child_y_size;

            if (ImGui::BeginChild("##logs", {space.x, log_child_y_size}, true)) {
              mtx_.lock();
              for (std::size_t i = log_idx_ - 1; i > 0; i--) {
                ImGui::TextWrapped(log_buff_[i].c_str());
              }
              mtx_.unlock();
              ImGui::SetScrollHereY(1);
              ImGui::EndChild();

              if (ImGui::BeginChild("##cmd_child", {space.x, cmd_child_y_size}, true, ImGuiWindowFlags_NoScrollbar)) {
                if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                  ImGui::SetKeyboardFocusHere();

                ImGui::PushItemWidth(space.x * 0.92f);
                ImGui::InputText("##cmd_input", (char*) &cmd_buff, GTA_BASE_ARRAY_SIZE(cmd_buff));
                ImGui::SameLine();
                if (ImGui::Button("Execute##cmd_box")) {
                  memset(&cmd_buff, 0, GTA_BASE_ARRAY_SIZE(cmd_buff));
                }

                ImGui::EndChild();
              }
            }
            ImGui::End();
          }
        }));
      }
    }

  private:
    std::mutex mtx_;
    std::atomic<bool> render_window_ = false;
    std::atomic<std::size_t> log_idx_{};
    std::string log_buff_[log_buff_size];
    char cmd_buff[256]{};
    std::unique_ptr<util::TimedInput> input_show_window_;

  private:
    FORCE_INLINE void ProcessedPrint(std::string_view str) {
      if (log_idx_ == log_buff_size) {
        MakeRoom();
      }

      log_buff_[log_idx_] = str;
      log_idx_ += 1;
    }

    FORCE_INLINE void ProcessedPrint(const std::vector<std::string>& lines) {
      for (auto&& line : lines)
        ProcessedPrint(line);
    }

    FORCE_INLINE void MakeRoom() {
      constexpr static const std::size_t remove_count = log_buff_size / log_buff_scroll_count;
      constexpr static const std::size_t move_count = log_buff_size - remove_count;

      for (std::size_t i = 0; i < move_count; ++i) {
        log_buff_[i] = std::move(log_buff_[i + remove_count]);
      }

      log_idx_ -= remove_count;
    }
  };
}
#endif //GTA_BASE_ON_SCREEN_CONSOLE_C13ACFACDEAF44CBB876D2D0517CAD37_HPP
