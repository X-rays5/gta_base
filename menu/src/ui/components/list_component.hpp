//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_component.hpp"

namespace base::menu::ui::components {
  class ListComponent final : public BaseComponent {
  public:
    ListComponent(const std::string& name, const std::shared_ptr<std::vector<std::string>>& items, const std::shared_ptr<std::atomic<std::size_t>>& selected_index)
        : items_(items), selected_index_(selected_index) {
      left_text_ = name;
    }

    virtual std::string GetRightText() const override {
      if (!items_ || items_->empty()) {
        return "";
      }

      const std::size_t current_index = selected_index_->load();
      const std::size_t clamped_index = std::min(current_index, items_->size() - 1);
      const bool is_first = clamped_index == 0;
      const bool is_last = clamped_index >= items_->size() - 1;

      return fmt::format("{} {} {}", is_first ? "" : ICON_FA_ANGLE_LEFT, (*items_)[clamped_index], is_last ? "" : ICON_FA_ANGLE_RIGHT);
    }

    virtual bool HasRightText() const override {
      return items_ && !items_->empty();
    }

    virtual void HandleButtonPress(const PressedButton button) override {
      if (!items_ || items_->empty()) {
        return;
      }

      std::size_t current_index = selected_index_->load();
      const std::size_t clamped_index = std::min(current_index, items_->size() - 1);

      if (button == PressedButton::kLEFT) {
        if (clamped_index == 0) {
          return; // Already at the beginning
        }
        selected_index_->store(clamped_index - 1);
      } else if (button == PressedButton::kRIGHT) {
        if (clamped_index >= items_->size() - 1) {
          return; // Already at the end
        }
        selected_index_->store(clamped_index + 1);
      }
    }

  private:
    std::shared_ptr<std::vector<std::string>> items_;
    std::shared_ptr<std::atomic<std::size_t>> selected_index_;
  };
}

