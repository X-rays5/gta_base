//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_list_component.hpp"

namespace base::menu::ui::components {
  class ListComponent final : public BaseListComponent {
  public:
    ListComponent(const std::string& name, const std::shared_ptr<std::vector<std::string>>& items, const std::shared_ptr<std::atomic<std::size_t>>& selected_index)
        : BaseListComponent(name, items, selected_index) {
    }

    virtual std::string GetRightText() const override {
      if (!items_ || items_->empty()) {
        return "";
      }
      const std::size_t current_idx = GetClampedIndex() + 1; // 1-based for display
      const std::size_t total_count = items_->size();
      return fmt::format("{} [{}/{}]", GetCurrentItemText(), current_idx, total_count);
    }
  };
}

