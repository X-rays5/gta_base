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
#include <cassert>
#include <d3d11.h>
#include "imgui.h"

namespace ui {
  namespace util {
    namespace draw {
      __forceinline ImDrawList* GetDrawList() {
        return ImGui::GetForegroundDrawList();
      }

      __forceinline ImVec2 GetSize(ImVec2 pos, ImVec2 size) {
        return {pos.x + size.x, pos.y + size.y};
      }

      // scale float in range [0, 1] to [0, screen_size]
      __forceinline ImVec2 ScaleToScreen(ImVec2 xy) {
        if (xy.x < 0 || xy.x > 1 || xy.y < 0 || xy.y > 1) { // best to just throw here since this should never happen
          throw std::out_of_range("ScaleToScreen: xy out of bounds");
        }

        ImVec2 cur_res = ImGui::GetIO().DisplaySize;

        xy.x = (xy.x * cur_res.x);
        xy.y = (xy.y * cur_res.y);

        return xy;
      }

      // scale float in range [0, screen_size] to [0, 1]
      __forceinline ImVec2 ScaleFromScreen(ImVec2 xy) {
        ImVec2 cur_res = ImGui::GetIO().DisplaySize;

        if (xy.x < 0 || xy.x > cur_res.x || xy.y < 0 || xy.y > cur_res.y) { // best to just throw here since this should never happen
          throw std::out_of_range("ScaleFromScreen: xy out of bounds");
        }

        xy.x = (xy.x / cur_res.x);
        xy.y = (xy.y / cur_res.y);

        return xy;
      }

      __forceinline float ScaleXToScreen(float x) {
        return ScaleToScreen({x, 0}).x;
      }

      __forceinline float ScaleXFromScreen(float x) {
        return ScaleFromScreen({x, 0}).x;
      }

      __forceinline float ScaleYToScreen(float y) {
        return ScaleToScreen({0, y}).y;
      }

      __forceinline float ScaleYFromScreen(float y) {
        return ScaleFromScreen({0, y}).y;
      }

      __forceinline float ScaleFont(float size) {
        return ScaleYToScreen(size);
      }

      __forceinline ImVec2 CalcTextSize(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f) {
        if (!font)
          return {0,0};

        ImVec2 text_size = font->CalcTextSizeA(ScaleFont(font_size), ImGui::GetIO().DisplaySize.x, wrap_width, text.c_str());
        text_size.x = ((float)(int)(text_size.x + 0.99999f));

        return ScaleFromScreen(text_size);
      }

      class BaseDrawCommand {
      public:
        virtual __forceinline void Draw() = 0;
      };

      class Rect : public BaseDrawCommand{
      public:
        __forceinline Rect(ImVec2 pos, ImVec2 size, ImU32 color) : pos_(pos), size_(size), color_(color) {}

        __forceinline void Draw() final {
          GetDrawList()->AddRectFilled(ScaleToScreen(pos_), ScaleToScreen(GetSize(pos_, size_)), color_);
        }

      private:
        ImVec2 pos_;
        ImVec2 size_;
        ImU32 color_;
      };

      class Text : public BaseDrawCommand {
      public:
        __forceinline Text(ImVec2 pos, ImU32 color, std::string text, bool right_align, bool center, float y_size_text, const ImFont* font = nullptr) : pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), center_(center), y_size_text_(y_size_text), font_(font)
        {
        }

        __forceinline void Draw() final {
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
        __forceinline Image(ID3D11ShaderResourceView* texture, ImVec2 pos, ImVec2 size, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE) : texture_(texture), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col)
        {
        }

        __forceinline void Draw() final {
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
        __forceinline explicit DrawList(std::size_t draw_command_buffer_count) {
          assert(draw_command_buffer_count >= 2);

          for (std::size_t i = 0; i < draw_command_buffer_count; ++i) {
            draw_commands_.push_back(draw_list_t{});
          }
          draw_commands_.shrink_to_fit();
        }

        template<typename T>
        __forceinline void AddCommand(T command) {
          mtx_.lock();
          draw_commands_[cur_write_target_].push_back(std::make_shared<T>(std::forward<T>(command)));
          mtx_.unlock();
        }

        __forceinline void Draw() {
          mtx_.lock();
          for (auto& command : draw_commands_[cur_render_target_]) {
            command->Draw();
          }
          mtx_.unlock();
        }

        __forceinline void NextTargets() {
          mtx_.lock();
          NextRenderTarget();
          NextWriteTarget();
          mtx_.unlock();
        }

      private:
        using draw_list_t = std::vector<std::shared_ptr<BaseDrawCommand>>;
        using draw_commands_t = std::vector<draw_list_t>;


        std::mutex mtx_;
        std::size_t cur_write_target_ = 1;
        std::size_t cur_render_target_ = 0;
        draw_commands_t draw_commands_;

      private:
        __forceinline void NextRenderTarget() {
          cur_render_target_ += 1;
          if (cur_render_target_ >= draw_commands_.size())
            cur_render_target_ = 0;
        }

        __forceinline void NextWriteTarget() {
          cur_write_target_ += 1;
          if (cur_write_target_ >= draw_commands_.size())
            cur_write_target_ = 0;

          draw_commands_[cur_write_target_].clear();
        }
      };
    }
  }
}
#endif //GTA_BASE_DRAW_HPP
