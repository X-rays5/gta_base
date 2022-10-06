//
// Created by X-ray on 09/09/22.
//

#pragma once
#ifndef GTA_BASE_KEYBOARD_MANAGER_HPP
#define GTA_BASE_KEYBOARD_MANAGER_HPP
#include <string>
#include <functional>
#include <imgui.h>
#include "../../misc/common.hpp"

namespace gta_base::ui::keyboard {
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

        void Tick();

        inline Result GetState() {
          return state_;
        }

        inline std::string GetInput() {
          return {text_buf_};
        }

        inline void Callback() {
          std::invoke(cb_, GetInput(), state_);
        }

      private:
        std::string window_title_;
        std::string input_title_;
        std::string title_;
        Result state_ = Result::kNone;
        callback_t cb_;
        char text_buf_[256]{};
        std::size_t last_render_write_target_{};
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

        void Tick();

      private:
        std::vector<std::shared_ptr<Instance>> keyboards_;
      };
    }
#endif //GTA_BASE_KEYBOARD_MANAGER_HPP
