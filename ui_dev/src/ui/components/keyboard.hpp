//
// Created by X-ray on 09/09/22.
//

#pragma once
#ifndef GTA_BASE_KEYBOARD_HPP
#define GTA_BASE_KEYBOARD_HPP
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <memory>

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
        explicit Instance(std::string title, callback_t cb) : title_(std::move(title)), cb_(std::move(cb)) {}

        void Tick() {
          static bool show = true; // should never change

          ImGui::SetNextWindowSize(size_);
          ImGui::SetNextWindowPos(pos_, ImGuiCond_Appearing);
          ImGui::Begin(title_.c_str(), &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
          ImGui::InputText("###keyboard", (char*)&text_buf_, sizeof(text_buf_));
          ImGui::SameLine();
          if (ImGui::Button("Done")) {
            state_ = Result::kDone;
            cb_(std::string(text_buf_), state_);
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            state_ = Result::kCancel;
            cb_(std::string(text_buf_), state_);
          }
          ImGui::End();
        }

        std::string GetTitle() {
          return title_;
        }

        Result GetState() {
          return state_;
        }

      private:
        std::string title_;
        Result state_ = Result::kNone;
        callback_t cb_;
        char text_buf_[256]{};
        constexpr static const ImVec2 size_ = {400, 50};
        constexpr static const ImVec2 pos_ = {1000, 600};
      };

      class Manager {
      public:
        void ShowKeyboard(const std::string& title, const Instance::callback_t& cb) {
          keyboards_.emplace_back(std::make_shared<Instance>(title, cb));
        }

        void Tick() {
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
#endif //GTA_BASE_KEYBOARD_HPP
