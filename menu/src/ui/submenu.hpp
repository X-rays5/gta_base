//
// Created by X-ray on 31/05/2025.
//

#ifndef SUBMENU_HPP_05191835
#define SUBMENU_HPP_05191835
#include <utility>
#include <vector>
#include <base-common/concurrency/spinlock.hpp>
#include "components/base_component.hpp"

namespace base::menu::options {
  class BaseOption;
}

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
     * Adds a component linked to an option. The component will automatically have the option set.
     * This is the preferred way to add option-backed components.
     */
    template <typename T, typename V> requires std::is_base_of_v<components::BaseComponent, T> and std::is_base_of_v<options::BaseOption, V>
    void AddComponent(V* option, T&& component) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      component.SetOption(option);
      components_.emplace_back(Component{std::make_shared<T>(std::forward<T>(component))});
    }

    /**
     * Updates the components of the submenu by calling the update callback. Through this the submenu can dynamically change its components.
     */
    void UpdateComponents() {
      common::concurrency::ScopedSpinlock lock(spinlock_);

      if (!update_components_cb_) {
        return; // No callback to update with
      }

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

      if (!components_.empty() && components_[cur_opt_idx_].component->HasFlag(components::Flags::NotSelectable)) {
        // If the current option is not selectable, scroll to find the next selectable option
        Scroll(ScrollDirection::kDOWN);
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
     * Get a specific component by index.
     * @param idx The index of the component to retrieve.
     * @return The component at the specified index.
     */
    [[nodiscard]] Component& GetComponent(const size_t idx) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (idx >= components_.size()) {
        throw std::out_of_range("Index out of range for submenu components");
      }
      return components_[idx];
    }

    /**
     * Get a specific component by index (const version).
     * @param idx The index of the component to retrieve.
     * @return The component at the specified index.
     */
    [[nodiscard]] const Component& GetComponent(const size_t idx) const {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (idx >= components_.size()) {
        throw std::out_of_range("Index out of range for submenu components");
      }
      return components_[idx];
    }

    /**
     * Get the current component based on the current option index.
     * @return The current component, or nullptr if no component is available.
     */
    [[nodiscard]] std::shared_ptr<components::BaseComponent> GetCurrentComponent() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (cur_opt_idx_ >= components_.size()) {
        return nullptr; // No current component available
      }
      return components_[cur_opt_idx_].component;
    }

    /**
     * Get the current component based on the current option index (const version).
     * @return The current component, or nullptr if no component is available.
     */
    [[nodiscard]] std::shared_ptr<components::BaseComponent> GetCurrentComponent() const {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (cur_opt_idx_ >= components_.size()) {
        return nullptr; // No current component available
      }
      return components_[cur_opt_idx_].component;
    }

    /**
     * Get the name of this submenu.
     * @return The name of this submenu.
     */
    [[nodiscard]] std::string GetName() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return localization::kMANAGER->Localize(sub_name_);
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
     * @return The current option index for display purposes, which is 1-based among selectable components only.
     */
    [[nodiscard]] std::size_t GetCurrentOptionIndexForDisplay() {
      common::concurrency::ScopedSpinlock lock(spinlock_);

      if (components_.empty()) {
        return 0; // No components to display
      }

      // Count how many selectable components are before or at the current index
      std::size_t selectable_index = 1;
      for (std::size_t i = 0; i <= cur_opt_idx_ && i < components_.size(); ++i) {
        if (!components_[i].component->HasFlag(components::Flags::NotSelectable)) {
          if (i < cur_opt_idx_) {
            ++selectable_index;
          }
        }
      }

      return selectable_index;
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
     * Get the total number of selectable options in the submenu for display purposes.
     * @return The number of selectable options in the submenu.
     */
    [[nodiscard]] std::size_t GetOptionCountForDisplay() {
      common::concurrency::ScopedSpinlock lock(spinlock_);

      std::size_t selectable_count = 0;
      for (const auto& comp : components_) {
        if (!comp.component->HasFlag(components::Flags::NotSelectable)) {
          ++selectable_count;
        }
      }

      return selectable_count;
    }

    /**
     * Scrolls the submenu in the specified direction.
     * @param direction The direction to scroll the submenu in.
     * @return The new current option index after scrolling.
     */
    std::size_t Scroll(const ScrollDirection direction) {
      common::concurrency::ScopedSpinlock lock(spinlock_);

      // Check if there are any components at all
      if (components_.empty()) {
        return cur_opt_idx_; // No components to scroll through
      }

      // Check if there are any selectable components
      bool has_selectable = false;
      for (const auto& comp : components_) {
        if (!comp.component->HasFlag(components::Flags::NotSelectable)) {
          has_selectable = true;
          break;
        }
      }

      // If no selectable components exist, return without scrolling
      if (!has_selectable) {
        return cur_opt_idx_;
      }

      bool is_wrapping_to_top = false;
      bool is_wrapping_to_bottom = false;

      if (direction == ScrollDirection::kUP) {
        if (cur_opt_idx_ == 0) {
          cur_opt_idx_ = components_.size() - 1; // Wrap around to the last option
          is_wrapping_to_bottom = true;
        } else {
          --cur_opt_idx_;
        }
      } else if (direction == ScrollDirection::kDOWN) {
        if (cur_opt_idx_ >= components_.size() - 1) {
          cur_opt_idx_ = 0; // Wrap around to the first option
          is_wrapping_to_top = true;
        } else {
          ++cur_opt_idx_;
        }
      } else {
        LOG_ERROR("Invalid scroll direction: {}", static_cast<std::uint32_t>(std::to_underlying(direction)));
        return cur_opt_idx_; // Return current index if invalid direction
      }

      // Reset scroll offset when wrapping to ensure view range is updated
      if (is_wrapping_to_top) {
        scroll_offset_ = 0;
      } else if (is_wrapping_to_bottom) {
        // When wrapping to bottom, position the view at the end of the list
        // Set scroll_offset to show items at the bottom, with last item at the end
        scroll_offset_ = components_.size() > max_visible_options_ ? components_.size() - max_visible_options_ : 0;
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
      max_visible_options_ = max_visible_options;
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
    std::uint32_t max_visible_options_ = 12; // Default max visible options
    std::string sub_name_;
    component_list_t components_;
    update_components_cb_t update_components_cb_ = nullptr;
  };

  enum class SubmenuIDs : std::uint32_t {
    kMAIN_MENU = 0,
    kSELF,
    kSELF_HEALTH,
    kVEHICLE,
    kSETTINGS,
    kLOAD_TRANSLATIONS,
    kLOAD_FEATURE_SETTINGS,
    kUNLOAD_CONFIRM,
    kTHEME_SETTINGS,
  };
}

#endif //SUBMENU_HPP_05191835
