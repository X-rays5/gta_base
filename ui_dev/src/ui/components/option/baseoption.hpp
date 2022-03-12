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
        virtual std::string GetName() {
          return name_;
        }
        virtual void SetName(std::string name) {
          name_ = std::move(name);
        }

        virtual std::string GetDescription() {
          return description_;
        }
        virtual void SetDescription(std::string description) {
          description_ = std::move(description);
        }

        virtual std::string GetRightText() {
          return right_text_;
        }
        virtual void SetRightText(std::string rightText) {
          right_text_ = std::move(rightText);
        }
        virtual std::filesystem::path GetIconPath() {
          return icon_path_;
        }
        virtual void SetIconPath(const std::filesystem::path& iconPath) {
          icon_path_ = iconPath;
        }

        virtual void HandleKey(KeyInput key) = 0;
        virtual bool HasFlag(OptionFlag flag) = 0;
      protected:
        std::string name_{};
        std::string description_{};
        std::string right_text_{};
        std::filesystem::path icon_path_{};
      };
    }
  }
}
#endif //GTA_BASE_BASEOPTION_HPP
