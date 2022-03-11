//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_BASEOPTION_HPP
#define GTA_BASE_BASEOPTION_HPP
#include <filesystem>
#include "../enums.hpp"

namespace ui {
  namespace components {
    namespace option {
      class BaseOption {
      public:
        virtual const char* GetName() {
          return name_;
        }
        virtual void SetName(const char* name) {
          name_ = (char*)name;
        }

        virtual const char* GetDescription() {
          return description_;
        }
        virtual void SetDescription(const char* description) {
          description_ = (char*)description;
        }

        virtual char* GetRightText() {
          return rightText_;
        }
        virtual void SetRightText(const char* rightText) {
          rightText_ = (char*)rightText;
        }
        virtual std::filesystem::path GetIconPath() {
          return icon_path_;
        }
        virtual void SetIconPath(const std::filesystem::path& iconPath) {
          icon_path_ = iconPath;
        }

        virtual void HandleKey(KeyInput key) = 0;
      protected:
        char* name_{};
        char* description_{};
        char* rightText_{};
        std::filesystem::path icon_path_{};
      };
    }
  }
}
#endif //GTA_BASE_BASEOPTION_HPP
