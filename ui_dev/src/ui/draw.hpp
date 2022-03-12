//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_DRAW_HPP
#define GTA_BASE_DRAW_HPP
#include <utility>
#include <vector>
#include <mutex>
#include <imgui.h>
#include <d3d11.h>

namespace ui {
  namespace draw {
    inline ImDrawList* GetDrawList() {
      return ImGui::GetForegroundDrawList();
    }

    inline ImVec2 GetSize(ImVec2 pos, ImVec2 size) {
      return {pos.x + size.x, pos.y + size.y};
    }

    inline ImVec2 CalcTextSize(const ImFont* font, float font_size, const std::string& text, float wrap_width = 0.0f) {
      if (!font)
        return {0,0};

      ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text.c_str());
      text_size.x = ((float)(int)(text_size.x + 0.99999f));

      return text_size;
    }

    inline void ScaleInBounds(ImVec2& check) {
      if (check.x > 1000) {
        check.x = 1000.f;
      }
      if (check.x < 0) {
        check.x = 0.f;
      }
      if (check.y > 1000) {
        check.y = 1000.f;
      }
      if (check.y < 0) {
        check.y = 0.f;
      }
    }

    inline ImVec2 ScaleImpl(ImVec2& vec) {
      ScaleInBounds(vec);

      float x_scale = ImGui::GetIO().DisplaySize.x / 1000.f;
      float y_scale = ImGui::GetIO().DisplaySize.y / 1000.f;

      return {vec.x * x_scale, vec.y * y_scale};
    }

    inline ImVec2 Scale(ImVec2 vec) {
      return ScaleImpl(vec);
    }

    inline float Scale(float scale) {
      ImVec2 vec = {scale, scale};
      return ScaleImpl(vec).x;
    }

    class BaseDrawCommand {
    public:
      virtual inline void Draw() = 0;
    };

    class Rect : public BaseDrawCommand{
    public:
      inline Rect(ImVec2 pos, ImVec2 size, ImU32 color) : pos_(pos), size_(size), color_(color)
      {
      }

      inline void Draw() final {
        GetDrawList()->AddRectFilled(Scale(pos_), Scale(GetSize(pos_, size_)), color_);
      }

    private:
      ImVec2 pos_;
      ImVec2 size_;
      ImU32 color_;
    };

    class Text : public BaseDrawCommand {
    public:
      inline Text(ImVec2 pos, ImU32 color, std::string  text, bool right_align, float font_size, const ImFont* font = nullptr) : pos_(pos), color_(color), text_(std::move(text)), right_align_(right_align), font_size_(font_size), font_(font)
      {
      }

      inline void Draw() final {
        if (right_align_) {
          if (!font_) {
            font_ = ImGui::GetFont();
          }
          ImVec2 text_size = CalcTextSize(font_, font_size_, text_);
          pos_.x -= text_size.x;
          pos_.y += (text_size.y / 2);
        } else {
          if (!font_) {
            font_ = ImGui::GetFont();
          }
          ImVec2 text_size = CalcTextSize(font_, font_size_, text_);
          pos_.y += (text_size.y / 2);
        }


        GetDrawList()->AddText(font_, Scale(font_size_), Scale(pos_), color_, text_.c_str());
      }

    private:
      ImVec2 pos_;
      ImU32 color_;
      std::string text_;
      bool right_align_;
      float font_size_;
      const ImFont* font_;
    };

    class Image : public BaseDrawCommand {
    public:
      inline Image(ID3D11ShaderResourceView* texture, ImVec2 pos, ImVec2 size, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE) : texture_(texture), pos_(pos), size_(size), uv_min_(uv_min), uv_max_(uv_max), col_(col)
      {
      }

      inline void Draw() final {
        GetDrawList()->AddImage((void*)texture_, Scale(pos_), Scale(GetSize(pos_, size_)), uv_min_, uv_max_, col_);
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
      template<typename T>
      inline void AddCommand(T command) {
        std::lock_guard lock(mtx_);
        draw_commands_.push_back(std::make_shared<T>(std::forward<T>(command)));
      }

      inline void Draw() {
        std::lock_guard lock(mtx_);
        for (auto&& command : draw_commands_) {
          command->Draw();
        }
        draw_commands_.clear();
      }

    private:
      std::mutex mtx_;
      std::vector<std::shared_ptr<BaseDrawCommand>> draw_commands_;
    };
  }
}
#endif //GTA_BASE_DRAW_HPP
