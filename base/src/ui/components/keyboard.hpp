//
// Created by X-ray on 09/09/22.
//

#pragma once
#ifndef GTA_BASE_KEYBOARD_MANAGER_HPP
#define GTA_BASE_KEYBOARD_MANAGER_HPP
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "../manager.hpp"

namespace gta_base {
  namespace ui {
    namespace components {
      namespace keyboard {
        enum class Result {
          kNone,
          kDone,
          kCancel
        };

        class Instance {
        public:
          using callback_t = std::function<void(std::string, Result)>;

        public:
          explicit inline Instance(std::string title, callback_t& cb) : title_(std::move(title)), cb_(cb) {
            window_title_ = "###keyboard_title" + title_;
            input_title_ = "###keyboard" + title_;
          }

          void Tick() {
            // Don't draw multiple times on 1 render tick
            if (last_render_write_target_ != kMANAGER->GetDrawList()->GetWriteTarget() && state_ == Result::kNone) {
              last_render_write_target_ = kMANAGER->GetDrawList()->GetWriteTarget();
              kMANAGER->GetDrawList()->AddCommand(d3d::draw::DrawCallback{[this]{
                ImGui::SetNextWindowSize(d3d::draw::ScaleToScreen(size_));
                ImGui::SetNextWindowPos(d3d::draw::ScaleToScreen(pos_));
                if (ImGui::Begin(window_title_.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
                  ImGui::InputText(input_title_.c_str(), (char*)&text_buf_, sizeof(text_buf_));
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

            switch(state_) {
              case Result::kDone:
              case Result::kCancel:
                std::invoke(cb_, std::string((char*)&text_buf_), state_);
                break;
              default:
                break;
            }
          }

          inline Result GetState() {
            return state_;
          }

        private:
          std::string window_title_;
          std::string input_title_;
          std::string title_;
          Result state_ = Result::kNone;
          callback_t cb_;
          char text_buf_[256]{};
          std::size_t last_render_write_target_;
          constexpr static const ImVec2 size_ = {0.18f, 0.035f};
          constexpr static const ImVec2 pos_ = {0.5f - (size_.x / 2), 0.5f - (size_.y / 2)};
        };

        class Manager;
        inline Manager* kMANAGER{};

        class Manager {
        public:
          Manager() {
            kMANAGER = this;
          }

          ~Manager() {
            kMANAGER = nullptr;
          }

          inline void ShowKeyboard(const std::string& title, Instance::callback_t cb) {
            keyboards_.emplace_back(std::make_shared<Instance>(title, cb));
          }

          inline bool KeyBoardActive() {
            return !keyboards_.empty();
          }

          void Tick() {
            ImGui::GetIO().MouseDrawCursor = !keyboards_.empty();
            if (ImGui::GetIO().MouseDrawCursor) {
              ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            } else {
              ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
            }

            for (int i = 0; i < keyboards_.size(); i++) {
              auto&& keyboard = keyboards_[i];

              keyboard->Tick();
              if (keyboard->GetState() != Result::kNone)
                keyboards_.erase(keyboards_.begin() + i);
            }
          }

        private:
          std::vector<std::shared_ptr<Instance>> keyboards_;
        };
      }
    }
  }
}
#endif //GTA_BASE_KEYBOARD_MANAGER_HPP
