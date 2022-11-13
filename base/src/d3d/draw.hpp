//
// Created by X-ray on 07/02/22.
//

#pragma once
#ifndef GTA_BASE_DRAW_HPP
#define GTA_BASE_DRAW_HPP
#include <string>
#include <memory>
#include <iostream>
#include <mutex>
#include <numeric>
#include <cassert>
#include <utility>
#include <d3d11.h>
#include <imgui.h>
#include "renderer.hpp"

namespace gta_base::d3d::draw {
      inline ImDrawList* GetDrawList() {
        return ImGui::GetForegroundDrawList();
      }

      inline ImVec2 GetSize(ImVec2 pos, ImVec2 size) {
        return {pos.x + size.x, pos.y + size.y};
      }

      // scale float in range [0, 1] to [0, screen_size]
      inline ImVec2 ScaleToScreen(ImVec2 xy) {
        if (xy.x < 0 || xy.x > 1 || xy.y < 0 || xy.y > 1) { // Can't throw an exception here, because it will sometimes randomly happen for one frame. So just log it.
          LOG_WARN("ScaleToScreen: xy out of range: {} {}", xy.x, xy.y);
        }

        ImVec2 cur_res = ImGui::GetIO().DisplaySize;

        xy.x = (xy.x * cur_res.x);
        xy.y = (xy.y * cur_res.y);

        return xy;
      }

      // scale float in range [0, screen_size] to [0, 1]
      inline ImVec2 ScaleFromScreen(ImVec2 xy) {
        ImVec2 cur_res = ImGui::GetIO().DisplaySize;
        if (cur_res.x == 0 || cur_res.y == 0) {
          return {}; // Happens when the window is minimized.
        }

        if (xy.x < 0 || xy.x > cur_res.x || xy.y < 0 || xy.y > cur_res.y) { // Can't throw an exception here, because it will sometimes randomly happen for one frame. So just log it.
          LOG_WARN("ScaleFromScreen: xy out of range: {} {}", xy.x, xy.y);
        }

        xy.x = (xy.x / cur_res.x);
        xy.y = (xy.y / cur_res.y);

        return xy;
      }

      inline float ScaleXToScreen(float x) {
        return ScaleToScreen({x, 0}).x;
      }

      inline float ScaleXFromScreen(float x) {
        return ScaleFromScreen({x, 0}).x;
      }

      inline float ScaleYToScreen(float y) {
        return ScaleToScreen({0, y}).y;
      }

      inline float ScaleYFromScreen(float y) {
        return ScaleFromScreen({0, y}).y;
      }

      inline ImVec2 ScaleSquare(float y) {
        auto tmp = ScaleYToScreen(y);
        return ScaleFromScreen({tmp, tmp});
      }

      inline float ScaleFont(float size) {
        return ScaleYToScreen(size);
      }

      inline ImVec2 CalcTextSizeRaw(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f) {
        if (!font)
          font = ImGui::GetFont();

        ImVec2 text_size = font->CalcTextSizeA(ScaleFont(font_size), ImGui::GetIO().DisplaySize.x, wrap_width, text.c_str());
        text_size.x = ((float)(int)(text_size.x + 0.99999f));

        return text_size;
      }

      inline ImVec2 CalcTextSize(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f) {
        return ScaleFromScreen(CalcTextSizeRaw(font, font_size, text, wrap_width));
      }

      inline std::string WordWrapGetString(std::string* lines, std::uint32_t line_count) {
        std::string res;
        for (std::uint32_t i = 0; i < line_count; i++) {
          auto line = lines[i];
          while (line.back() == ' ') {
            line.pop_back();
          }
          res = fmt::format("{}{}\n", res, line);
        }

        return res;
      }

      class Animate {
      public:
        Animate() = default;
        Animate(double start, double end, std::uint64_t duration) : start_(start), end_(end), duration_(duration) {}

        double GetNow() {
          // use lerp to go from start to end
          curr_time_ += kRENDERER->GetDeltaTime();
          if (curr_time_ >= duration_) {
            return end_;
          }

          double percentage = ((double )(curr_time_ * 100) / (double)duration_) / 100;
          if (percentage >= 1)
            return end_;

          return std::lerp(start_, end_, percentage);
        }
      private:
        double start_{};
        double end_{};
        std::uint64_t curr_time_{};
        std::uint64_t duration_{};
      };

      // FIXME: Make it work with non monospaced fonts.
      // 2 lines take 0.01 ms in debug so it's fine for now
      inline std::uint32_t WordWrap(float font_size, std::string& str, float max_x, std::uint32_t max_lines) {
        auto real_max_x = ScaleXToScreen(max_x);

        if (str.empty() || CalcTextSizeRaw(ImGui::GetFont(), font_size, str).x <= real_max_x) {
          return 1; // Wrapping is not needed so just return that there is 1 line
        }

        auto char_x_size = CalcTextSizeRaw(ImGui::GetFont(), font_size, " ").x;
        auto chars_per_line = (std::uint32_t)(real_max_x / char_x_size);

        auto* lines = new std::string[max_lines];
        std::uint32_t line_count = 0;
        while (!str.empty()) {
          if (line_count >= max_lines) {
            auto last_line = lines[max_lines - 1];
            last_line.replace(last_line.end() - 3, last_line.end(), "...");
            lines[max_lines - 1] = last_line;
            break;
          }

          std::size_t pos = chars_per_line < str.size() ? str.rfind(' ', chars_per_line) : str.size();
          if (pos != std::string::npos) {
            lines[line_count] = str.substr(0, pos + 1);
            str.erase(0, pos + 1);
          } else {
            lines[line_count] = str.substr(0, chars_per_line);
            str.erase(0, chars_per_line);
          }
          line_count += 1;

          if (str.empty() && CalcTextSizeRaw(ImGui::GetFont(), font_size, lines[line_count - 1]).x <= real_max_x) {
            break;
          }
        }

        str = WordWrapGetString(lines, line_count);
        delete[] lines;

        return line_count;
      }

      class BaseDrawCommand {
      public:
        virtual inline void Draw() = 0;
      };

      // This can be used to execute code that should run on the render thread on render of a draw list
      // Also usable to draw ImGui windows
      class DrawCallback : public BaseDrawCommand {
      public:
        inline explicit DrawCallback(std::function<void()> cb) : cb_(std::move(cb)) {}

        inline void Draw() final {
          cb_();
        };

      private:
        std::function<void()> cb_;
      };

      class Rect : public BaseDrawCommand {
      public:
        inline Rect(ImVec2 pos, ImVec2 size, ImU32 color) : pos_(pos), size_(size), color_(color) {}

        inline void Draw() final {
          GetDrawList()->AddRectFilled(ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), color_);
        }

      private:
        ImVec2 pos_;
        ImVec2 size_;
        ImU32 color_;
      };

      class RectOutline : public BaseDrawCommand {
      public:
        inline RectOutline(ImVec2 pos, ImVec2 size, ImU32 color) : pos_(pos), size_(size), color_(color) {}

        inline void Draw() final {
          GetDrawList()->AddRect(ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), color_);
        }

      private:
        ImVec2 pos_;
        ImVec2 size_;
        ImU32 color_;
      };

      class Text : public BaseDrawCommand {
      public:
        inline Text(ImVec2 pos, ImU32 color, std::string text, bool right_align, bool center, float y_size_text, const ImFont* font = nullptr) : pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), center_(center), y_size_text_(y_size_text), font_(font)
        {
        }

        inline void Draw() final {
          if (right_align_) {
            if (!font_) {
              font_ = ImGui::GetFont();
            }
            ImVec2 text_size = CalcTextSize(font_, y_size_text_, text_);
            pos_.x -= text_size.x;
            pos_.y += (text_size.y / 2);
          } else {
            if (center_) {
              if (!font_) {
                font_ = ImGui::GetFont();
              }
              ImVec2 text_size = CalcTextSize(font_, y_size_text_, text_);
              pos_.y += ((text_size.y / 2));
            }
          }

          GetDrawList()->AddText(font_, ScaleFont(y_size_text_), ScaleToScreen(pos_), color_, text_.c_str());
        }

      private:
        ImVec2 pos_;
        ImU32 color_;
        std::string text_;
        bool right_align_;
        bool center_;
        float y_size_text_;
        const ImFont* font_;
      };

      class Image : public BaseDrawCommand {
      public:
        inline Image(ID3D11ShaderResourceView* texture, ImVec2 pos, ImVec2 size, ImU32 col = IM_COL32_WHITE, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1)) : texture_(texture), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col)
        {
        }

        inline void Draw() final {
          GetDrawList()->AddImage((void*)texture_, ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), uv_min_, uv_max_, col_);
        }

      private:
        ID3D11ShaderResourceView* texture_;
        ImVec2 pos_;
        ImVec2 size_;
        ImVec2 uv_min_;
        ImVec2 uv_max_;
        ImU32 col_;
      };

      class DrawList {
      public:
        inline explicit DrawList(std::size_t draw_command_buffer_count) {
          assert(draw_command_buffer_count >= 2);

          for (std::size_t i = 0; i < draw_command_buffer_count; ++i) {
            draw_commands_.push_back(draw_list_t{});
          }
          draw_list_idx_.resize(draw_command_buffer_count);
          draw_commands_.shrink_to_fit();
        }

        template<typename T>
        inline void AddCommand(T command) {
          std::unique_lock lock(mtx_);
          draw_commands_[cur_write_target_][draw_list_idx_[cur_write_target_]] = std::make_unique<T>(std::move(command));
          draw_list_idx_[cur_write_target_] += 1;
        }

        inline void Draw() {
          std::unique_lock lock(mtx_);
          for (auto&& command : draw_commands_[cur_render_target_]) {
            if (command != nullptr)
              command->Draw();
            else
              break;
          }
        }

        inline void NextTargets() {
          std::unique_lock lock(mtx_);
          NextRenderTarget();
          NextWriteTarget();
        }

        [[nodiscard]] inline std::size_t GetWriteTarget() const {
          return cur_write_target_;
        }

        [[nodiscard]] inline std::size_t GetRenderTarget() const {
          return cur_render_target_;
        }

      private:
        // can queue 4096 draw commands per tick. This should be plenty and can always be increased. Since this won't use huge amounts of memory.
        constexpr static const std::size_t max_draw_commands_ = 4096;

        using draw_list_t = std::array<std::unique_ptr<BaseDrawCommand>, max_draw_commands_>;
        using draw_commands_t = std::vector<draw_list_t>;

        std::recursive_mutex mtx_;
        std::size_t cur_write_target_ = 1;
        std::size_t cur_render_target_ = 0;
        draw_commands_t draw_commands_;
        std::vector<int> draw_list_idx_;

      private:
        inline void ClearDrawList(std::size_t idx) {
          draw_list_idx_[idx] = 0;
          for (int i = 0; i < max_draw_commands_; i++) {
            if (draw_commands_[idx][i]) {
              draw_commands_[idx][i].reset();
            } else {
              break; // past data
            }
          }
        }

        inline void NextRenderTarget() {
          cur_render_target_ += 1;
          if (cur_render_target_ >= draw_commands_.size())
            cur_render_target_ = 0;
        }

        inline void NextWriteTarget() {
          cur_write_target_ += 1;
          if (cur_write_target_ >= draw_commands_.size())
            cur_write_target_ = 0;

          ClearDrawList(cur_write_target_);
        }
      };
    }
#endif //GTA_BASE_DRAW_HPP