//
// Created by X-ray on 3/9/2022.
//

#pragma once

#ifndef GTA_BASE_BASE_OPTION_HPP
#define GTA_BASE_BASE_OPTION_HPP
#include <filesystem>
#include <utility>
#include <functional>
#include "../enums.hpp"
#include "../../translation/translation_manager.hpp"

namespace gta_base::ui::option {
      class BaseOption {
      public:
        using on_event_t = std::function<void(Event)>;

        explicit BaseOption(std::string name_key = "", std::string description_key = "", std::string center_text_key_ = "", std::string right_text_key = "", bool save_able = true, bool hotkey_able = true, on_event_t event_handler = nullptr) :
          name_key_(std::move(name_key)), description_key_(std::move(description_key)), center_text_key_(std::move(center_text_key_)), right_text_key_(std::move(right_text_key)), save_able_(save_able), hotkey_able_(hotkey_able), event_handler_(std::move(event_handler))
        {}

        inline std::string GetName() {
          return std::string((*kTRANSLATION_MANAGER)[name_key_]);
        }

        inline std::string_view GetNameKey() {
          return name_key_;
        }

        inline void SetNameKey(std::string name_key) {
          name_key_ = std::move(name_key);
        }

        inline std::string GetDescription() {
          return std::string((*kTRANSLATION_MANAGER)[description_key_]);
        }

        inline void SetDescriptionKey(std::string description_key) {
          description_key_ = std::move(description_key);
        }

        inline std::string GetCenterText() {
          auto res = std::string((*kTRANSLATION_MANAGER)[center_text_key_]);
          if (HasFlag(OptionFlag::kLabel))
            res = fmt::format("[{}]", res);

          return res;
        }

        inline void SetCenterTextKey(std::string center_text_key) {
          center_text_key_ = std::move(center_text_key);
        }

        inline std::string GetRightText() {
          return std::string((*kTRANSLATION_MANAGER)[right_text_key_]);
        }

        inline void SetRightTextKey(std::string right_text_key) {
          right_text_key_ = std::move(right_text_key);
        }

        inline void OnEvent(on_event_t handler) {
          event_handler_ = std::move(handler);
        }

        virtual void HandleKey(KeyInput key) = 0;
        virtual bool HasFlag(OptionFlag flag) = 0;
        virtual std::string GetSaveVal() {return {};};
        virtual void SetSavedVal(const std::string& val) {};
      protected:
        std::string name_key_;
        std::string description_key_;
        std::string center_text_key_;
        std::string right_text_key_;
        bool save_able_;
        bool hotkey_able_;
        on_event_t event_handler_;

      protected:
        inline void SendEvent(Event event) {
          if (event_handler_)
            std::invoke(event_handler_,event);
        }
      };
    }
#endif //GTA_BASE_BASE_OPTION_HPP