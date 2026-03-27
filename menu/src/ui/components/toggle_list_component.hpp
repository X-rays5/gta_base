//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_list_component.hpp"

namespace base::menu::ui::components {
  class ToggleListComponent final : public BaseListComponent {
  public:
    ToggleListComponent(const std::string& name, const std::string& description, const std::shared_ptr<std::vector<std::string>>& items,
                        const std::shared_ptr<std::atomic<std::size_t>>& selected_index, const std::shared_ptr<std::atomic<bool>>& toggle_state)
        : BaseListComponent(name, description, items, selected_index), toggle_state_(toggle_state) {
    }

    virtual std::string GetRightText() const override {
      if (!items_ || items_->empty()) {
        return "";
      }
      const bool is_enabled = toggle_state_ && toggle_state_->load();
      const std::size_t current_idx = GetClampedIndex() + 1; // 1-based for display
      const std::size_t total_count = items_->size();
      return fmt::format("{} [{}/{}] {}", GetCurrentItemText(), current_idx, total_count, is_enabled ? ICON_FA_CHECK : ICON_FA_XMARK);
    }

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        // Toggle the boolean state
        if (toggle_state_) {
          toggle_state_->store(!toggle_state_->load());
        }
      } else {
        // Use parent's navigation logic for LEFT/RIGHT
        BaseListComponent::HandleButtonPress(button);
      }
    }

  private:
    std::shared_ptr<std::atomic<bool>> toggle_state_;
  };
}

