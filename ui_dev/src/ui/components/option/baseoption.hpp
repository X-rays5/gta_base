//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_BASEOPTION_HPP
#define GTA_BASE_BASEOPTION_HPP
#include <filesystem>
#include <utility>
#include "../enums.hpp"

namespace ui {
  namespace components {
    namespace option {
      class BaseOption {
      public:
        using on_event_t = void(*)(Event event);

        inline std::string GetName() {
          return name_;
        }

        inline void SetName(std::string name) {
          name_ = std::move(name);
        }

        inline std::string GetDescription() {
          return description_;
        }

        inline void SetDescription(std::string description) {
          description_ = std::move(description);
        }

        inline std::string GetCenterText() {
          return center_text_;
        }

        inline void SetCenterText(std::string center_text) {
          center_text_ = std::move(center_text);
        }

        inline std::string GetRightText() {
          return right_text_;
        }

        inline void SetRightText(std::string rightText) {
          right_text_ = std::move(rightText);
        }

        inline std::filesystem::path GetIconPath() {
          return icon_path_;
        }

        inline void SetIconPath(const std::filesystem::path& iconPath) {
          icon_path_ = iconPath;
        }

        inline void OnEvent(on_event_t handler) {
          event_handler_ = handler;
        }

        virtual void HandleKey(KeyInput key) = 0;
        virtual bool HasFlag(OptionFlag flag) = 0;
      protected:
        std::string name_{};
        std::string description_{};
        std::string center_text_{};
        std::string right_text_{};
        std::filesystem::path icon_path_{};
        on_event_t event_handler_ = nullptr;

      protected:
        inline void SendEvent(Event event) {
          if (event_handler_)
            event_handler_(event);
        }
      };
    }
  }
}
#endif //GTA_BASE_BASEOPTION_HPP
