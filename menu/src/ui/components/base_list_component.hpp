//
// Created by X-ray on 24/03/2026.
//

#pragma once

#include "base_component.hpp"

namespace base::menu::ui::components {
  /**
   * Base class for list-based components to reduce code duplication.
   * Handles common list navigation logic (LEFT/RIGHT arrows).
   */
  class BaseListComponent : public BaseComponent {
  protected:
    std::shared_ptr<std::vector<std::string>> items_;
    std::shared_ptr<std::atomic<std::size_t>> selected_index_;

  public:
    BaseListComponent(const std::string& name, const std::shared_ptr<std::vector<std::string>>& items,
                      const std::shared_ptr<std::atomic<std::size_t>>& selected_index)
        : items_(items), selected_index_(selected_index) {
      left_text_ = name;
    }

    virtual ~BaseListComponent() = default;

    /**
     * Get the current clamped index (safe access to the list)
     */
    std::size_t GetClampedIndex() const {
      if (!items_ || items_->empty()) {
        return 0;
      }
      const std::size_t current_index = selected_index_->load();
      return std::min(current_index, items_->size() - 1);
    }

    /**
     * Check if at the beginning of the list
     */
    bool IsAtStart() const {
      return GetClampedIndex() == 0;
    }

    /**
     * Check if at the end of the list
     */
    bool IsAtEnd() const {
      if (!items_ || items_->empty()) {
        return true;
      }
      return GetClampedIndex() >= items_->size() - 1;
    }

    /**
     * Navigate to previous item (no wrapping)
     */
    void NavigatePrevious() {
      if (!items_ || items_->empty()) {
        return;
      }
      const std::size_t clamped_index = GetClampedIndex();
      if (clamped_index > 0) {
        selected_index_->store(clamped_index - 1);
      }
    }

    /**
     * Navigate to next item (no wrapping)
     */
    void NavigateNext() {
      if (!items_ || items_->empty()) {
        return;
      }
      const std::size_t clamped_index = GetClampedIndex();
      if (clamped_index < items_->size() - 1) {
        selected_index_->store(clamped_index + 1);
      }
    }

    /**
     * Get the current item text
     */
    std::string GetCurrentItemText() const {
      if (!items_ || items_->empty()) {
        return "";
      }
      return (*items_)[GetClampedIndex()];
    }

    virtual bool HasRightText() const override {
      return items_ && !items_->empty();
    }

    /**
     * Default button press handling for list navigation
     * Derived classes should call this or override with their own logic
     */
    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kLEFT) {
        NavigatePrevious();
      } else if (button == PressedButton::kRIGHT) {
        NavigateNext();
      }
    }
  };
}

