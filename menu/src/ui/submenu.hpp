//
// Created by X-ray on 31/05/2025.
//

#ifndef SUBMENU_HPP_05191835
#define SUBMENU_HPP_05191835
#include <atomic>
#include <vector>
#include <base-common/util/result.hpp>
#include "components/base_component.hpp"
#include <base-common/concurrency/spinlock.hpp>

namespace base::menu::ui {
  class Submenu {
  public:
    struct Component {
      std::shared_ptr<components::BaseComponent> component;
    };

  public:
    using component_list_t = std::vector<Component>;
    using update_components_cb_t = std::function<void(Submenu*)>;

  public:
    explicit Submenu(const std::string& name, const update_components_cb_t& update_components) : sub_name_(name), update_components_cb_(update_components) {}
    ~Submenu() = default;

    // Move constructor
    Submenu(Submenu&& other) noexcept
        : prev_opt_count_(other.prev_opt_count_),
          sub_name_(std::move(other.sub_name_)),
          components_(std::move(other.components_)),
          update_components_cb_(std::move(other.update_components_cb_)) {}

    Submenu& operator=(Submenu&& other) = delete;
    Submenu(const Submenu&) = delete;
    Submenu& operator=(const Submenu&) = delete;

    template <typename T> requires std::is_base_of_v<components::BaseComponent, T>
    void AddComponent(T&& component) {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      components_.emplace_back(Component{std::make_shared<T>(component)});
    }

    void UpdateComponents() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      if (update_components_cb_) {
        prev_opt_count_ = components_.size();
        components_.clear();
        update_components_cb_(this);
      }
    }

    [[nodiscard]] component_list_t& GetComponents() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return components_;
    }

    [[nodiscard]] std::string_view GetName() {
      common::concurrency::ScopedSpinlock lock(spinlock_);
      return sub_name_;
    }

  private:
    common::concurrency::RecursiveSpinlock spinlock_;
    std::size_t prev_opt_count_ = 0;
    std::string sub_name_;
    component_list_t components_;
    update_components_cb_t update_components_cb_ = nullptr;
  };

  enum class SubmenuIDs : std::uint32_t {
    kMAIN_MENU = 0,
  };
}

#endif //SUBMENU_HPP_05191835
