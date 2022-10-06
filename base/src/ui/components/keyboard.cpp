//
// Created by X-ray on 09/30/22.
//

#include "keyboard.hpp"
#include <utility>
#include "../manager.hpp"

namespace gta_base {
  namespace ui {
    namespace keyboard {
      void Instance::Tick() {
        // Don't draw multiple times on 1 render tick
        if (last_render_write_target_ != ui::kMANAGER->GetDrawList()->GetWriteTarget() && state_ == Result::kNone) {
          last_render_write_target_ = ui::kMANAGER->GetDrawList()->GetWriteTarget();
          ui::kMANAGER->GetDrawList()->AddCommand(d3d::draw::DrawCallback{[this] {
            ImGui::SetNextWindowSize(d3d::draw::ScaleToScreen(size_));
            ImGui::SetNextWindowPos(d3d::draw::ScaleToScreen(pos_));
            if (ImGui::Begin(window_title_.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
              ImGui::InputText(input_title_.c_str(), (char*) &text_buf_, sizeof(text_buf_));
              ImGui::SameLine();
              if (ImGui::Button("Done")) {
                state_ = Result::kDone;
              }
              ImGui::SameLine();
              if (ImGui::Button("Cancel")) {
                state_ = Result::kCancel;
              }
            }
            ImGui::End();
          }});
        }
      }

      void Manager::Tick() {
        ImGui::GetIO().MouseDrawCursor = !keyboards_.empty();
        if (ImGui::GetIO().MouseDrawCursor) {
          ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        } else {
          ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }

        if (!keyboards_.empty()) {
          globals::block_input = true;
        } else {
          globals::block_input = false;
        }

        for (int i = 0; i < keyboards_.size(); i++) {
          auto&& keyboard = keyboards_[i];

          keyboard->Tick();
          if (keyboard->GetState() == Result::kDone) {
            keyboard->Callback();
          }
          if (keyboard->GetState() != Result::kNone)
            keyboards_.erase(keyboards_.begin() + i);
        }
      }
    }
  }
}