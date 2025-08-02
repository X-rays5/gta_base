//
// Created by X-ray on 31/05/2025.
//

#ifndef SUBMENU_HPP_05191835
#define SUBMENU_HPP_05191835
#include <utility>
#include <vector>
#include <base-common/concurrency/spinlock.hpp>
#include "components/base_component.hpp"

namespace base::menu::ui {
  class Submenu {
  public:
    struct Component {
      std::shared_ptr<components::BaseComponent> component;
    };

    enum class ScrollDirection {
      kUP,
      kDOWN,
    };

  public:
    using component_list_t = std::vector<Component>;
    using update_components_cb_t = std::function<void(Submenu*)>;

  public:
    /**
     *
     * @param name The name of the submenu.
     * @param update_components Callback function that will be called to update the components of the submenu.
     */
    explicit Submenu(const std::string& name, const update_components_cb_t& update_components) : sub_name_(name), update_components_cb_(update_components) {}
    ~Submenu() = default;

    // Move constructor
    Submenu(Submenu&& other) noexcept
        : prev_opt_count_(other.prev_opt_count_),
          cur_opt_idx_(other.cur_opt_idx_),
          scroll_offset_(other.scroll_offset_),
          sub_name_(std::move(other.sub_name_)),
          components_(std::move(other.components_)),
          update_components_cb_(std::move(other.update_components_cb_)) {}

    Submenu& operator=(Submenu&& other) = delete;
    Submenu(const Submenu&) = delete;
    Submenu& operator=(const Submenu&) = delete;

    template <typename T> requires std::is_base_of_v<components::BaseComponent, T>
    void AddComponent(T&& component) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      components_.emplace_back(Component{std::make_shared<T>(std::forward<T>(component))});
    }

    /**
     * Updates the components of the submenu by calling the update callback. Through this the submenu can dynamically change its components.
     */
    void UpdateComponents() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (update_components_cb_) {
        prev_opt_count_ = components_.size();
        components_.clear();
        update_components_cb_(this);
        if (components_.size() > prev_opt_count_) {
          // If new components were added, reset the current option index to 0
          cur_opt_idx_ = 0;
        } else if (cur_opt_idx_ >= components_.size()) {
          // If the current option index is out of bounds, reset it to the last valid index
          cur_opt_idx_ = components_.empty() ? 0 : components_.size() - 1;
        }

        if (components_[cur_opt_idx_].component->HasFlag(components::Flags::NotSelectable)) {
          // If the current option is not selectable, scroll to find the next selectable option
          Scroll(ScrollDirection::kDOWN);
        }
      }
    }

    /**
     * Returns the list of components in the submenu.
     * @return List of components this submenu has.
     */
    [[nodiscard]] component_list_t& GetComponents() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return components_;
    }

    /**
     * Returns the list of components in the submenu (const version).
     * @return List of components this submenu has.
     */
    [[nodiscard]] const component_list_t& GetComponents() const {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return components_;
    }

    /**
     * Get the name of this submenu.
     * @return The name of this submenu.
     */
    [[nodiscard]] std::string_view GetName() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return sub_name_;
    }

    /**
     * The current option index of the submenu.
     * @return The current option index of the submenu.
     */
    [[nodiscard]] std::size_t GetCurrentOptionIndex() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return cur_opt_idx_;
    }

    /**
     * The current option index of the submenu (const version).
     * @return The current option index of the submenu.
     */
    [[nodiscard]] std::size_t GetCurrentOptionIndex() const {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return cur_opt_idx_;
    }

    /**
     * Get the current option index for display purposes.
     * @return The current option index for display purposes, which is 1-based.
     */
    [[nodiscard]] std::size_t GetCurrentOptionIndexForDisplay() {
      common::concurrency::ScopedSpinlock lock(spinlock_);

      std::size_t component_count = components_.size();
      for (const auto [component] : components_) {
        if (component->HasFlag(components::Flags::NotSelectable)) {
          --component_count; // Decrease count for non-selectable components
        }
      }

      if (component_count == 0) {
        return 0; // No selectable components to display
      }

      return (cur_opt_idx_ % component_count) + 1; // Return 1-based index for display
    }

    /**
     * Get the total number of options in the submenu.
     * @return The total number of options in the submenu.
     */
    [[nodiscard]] std::size_t GetOptionCount() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return components_.size();
    }

    /**
     * Get the total number of selectable options in the submenu.
     * @return The number of selectable options in the submenu.
     */
    [[nodiscard]] std::size_t GetOptionCountForDisplay() {
      common::concurrency::ScopedSpinlock lock(spinlock_);

      std::size_t component_count = components_.size();
      for (const auto [component] : components_) {
        if (component->HasFlag(components::Flags::NotSelectable)) {
          --component_count; // Decrease count for non-selectable components
        }
      }

      return component_count;
    }

    /**
     * Scrolls the submenu in the specified direction.
     * @param direction The direction to scroll the submenu in.
     * @return The new current option index after scrolling.
     */
    std::size_t Scroll(const ScrollDirection direction) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (components_.empty()) {
        return cur_opt_idx_; // No components to scroll through
      }

      if (direction == ScrollDirection::kUP) {
        if (cur_opt_idx_ == 0) {
          cur_opt_idx_ = components_.size() - 1; // Wrap around to the last option
        } else {
          --cur_opt_idx_;
        }
      } else if (direction == ScrollDirection::kDOWN) {
        if (cur_opt_idx_ >= components_.size() - 1) {
          cur_opt_idx_ = 0; // Wrap around to the first option
        } else {
          ++cur_opt_idx_;
        }
      } else {
        LOG_ERROR("Invalid scroll direction: {}", static_cast<std::uint32_t>(std::to_underlying(direction)));
        return cur_opt_idx_; // Return current index if invalid direction
      }

      if (components_[cur_opt_idx_].component->HasFlag(components::Flags::NotSelectable)) {
        return Scroll(direction); // If the current option is not selectable, scroll again
      }
      return cur_opt_idx_;
    }

    /**
     * Get the current scroll offset for rendering.
     * @return The current scroll offset.
     */
    [[nodiscard]] std::size_t GetScrollOffset() const {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return scroll_offset_;
    }

    /**
     * Update the scroll offset for rendering based on current selection and max visible options.
     * @param max_visible_options The maximum number of options that can be displayed at once.
     */
    void UpdateScrollOffset(std::uint32_t max_visible_options) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      const std::size_t total_components = components_.size();

      if (total_components <= max_visible_options) {
        scroll_offset_ = 0;
        return;
      }

      // Check if current selection is above the visible window
      if (cur_opt_idx_ < scroll_offset_) {
        scroll_offset_ = cur_opt_idx_;
      }
      // Check if current selection is below the visible window
      else if (cur_opt_idx_ >= scroll_offset_ + max_visible_options) {
        scroll_offset_ = cur_opt_idx_ - max_visible_options + 1;
      }

      // Ensure we don't scroll past the end
      if (scroll_offset_ + max_visible_options > total_components) {
        scroll_offset_ = total_components - max_visible_options;
      }
    }

  private:
    mutable common::concurrency::RecursiveSpinlock spinlock_;
    std::size_t prev_opt_count_ = 0;
    std::size_t cur_opt_idx_ = 0;
    std::size_t scroll_offset_ = 0; // Track the scroll position for rendering
    std::string sub_name_;
    component_list_t components_;
    update_components_cb_t update_components_cb_ = nullptr;
  };

  enum class SubmenuIDs : std::uint32_t {
    kMAIN_MENU = 0,
  };
}

#endif //SUBMENU_HPP_05191835
