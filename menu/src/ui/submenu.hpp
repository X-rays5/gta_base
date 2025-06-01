//
// Created by X-ray on 31/05/2025.
//

#ifndef SUBMENU_HPP_05191835
#define SUBMENU_HPP_05191835
#include <atomic>
#include <vector>
#include <base-common/util/result.hpp>
#include "components/base_component.hpp"

namespace base::menu::ui {
  class Submenu {
  public:
    struct Component {
      std::size_t id;
      std::shared_ptr<components::BaseComponent> component;
    };

  public:
    using component_list = std::vector<Component>;

  public:
    explicit Submenu(const std::string& name) : sub_name_(name) {}
    
    std::size_t AddComponent(components::BaseComponent&& component) {
      const auto component_id = next_component_id_->fetch_add(1, std::memory_order_relaxed);
      components_.emplace_back(Component{component_id, std::make_shared<components::BaseComponent>(component)});
      return component_id;
    }

    Status RemoveComponent(const std::size_t component_id) {
      const auto it = std::ranges::remove_if(components_, [component_id](const Component& comp) { return comp.id == component_id; }).begin();
      if (it != components_.end()) {
        components_.erase(it, components_.end());
        return {};
      }
      return MakeFailure<ResultCode::kIO_ERROR>("Component with ID {} not found in submenu {}", component_id, sub_name_);
    }

    [[nodiscard]] component_list& GetComponents() {
      return components_;
    }

    [[nodiscard]] std::string_view GetName() const {
      return sub_name_;
    }

  private:
    std::string sub_name_;
    component_list components_;
    std::shared_ptr<std::atomic<std::size_t>> next_component_id_ = std::make_shared<std::atomic<std::size_t>>(0);
  };

  enum class SubmenuIDs : std::uint32_t {
    kMAIN_MENU = 0,
  };
}

#endif //SUBMENU_HPP_05191835
