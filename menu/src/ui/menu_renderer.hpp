//
// Created by X-ray on 31/05/2025.
//

#ifndef MENU_RENDERER_HPP_05191653
#define MENU_RENDERER_HPP_05191653
#include <atomic>
#include <stack>
#include <ankerl/unordered_dense.h>
#include <magic_enum/magic_enum.hpp>
#include "submenu.hpp"
#include "theme.hpp"
#include "../render/animate.hpp"
#include "../scripts/base_script.hpp"
#include "../util/key_input/key_state.hpp"
#include "components/label_component.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::menu::ui {
  class MenuRenderer final : public scripts::BaseScript {
  public:
    MenuRenderer();
    virtual ~MenuRenderer() override;

    virtual void ScriptInit() override {}
    virtual void ScriptTick() override;

    void RenderMenu();

    virtual Type GetScriptType() const override {
      return Type::MenuRenderThread;
    }

    void AddSubmenu(const std::string& id, Submenu&& submenu) {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      submenus_.emplace(id, std::make_shared<Submenu>(std::move(submenu)));
      if (submenu_stack_.empty()) {
        submenu_stack_.push(id);
      }
    }

    void AddSubmenu(const SubmenuIDs id, Submenu&& submenu) {
      AddSubmenu(std::string(magic_enum::enum_name(id)), std::move(submenu));
    }

    std::shared_ptr<Submenu> GetCurrentSubmenu() {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      if (submenu_stack_.empty()) {
        return fallback_submenu_;
      }

      const auto it = submenus_.find(submenu_stack_.top());
      return (it != submenus_.end() && it->second) ? it->second : fallback_submenu_;
    }

    Status PushSubmenu(const std::string& id) {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      if (submenus_.find(id) == submenus_.end()) {
        return MakeFailure<ResultCode::kNOT_FOUND>("Submenu not found: {}", id);
      }

      submenu_stack_.push(id);
      return {};
    }

    bool IsOnHomeSubmenu() {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      return submenu_stack_.size() == 1;
    }

    void PopSubmenu() {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      if (!IsOnHomeSubmenu()) {
        submenu_stack_.pop();
      } else {
        is_menu_opened_ = false;
      }
    }

  private:
    MenuRenderProperties ui_props_{};
    util::KeyState menu_ui_key_state_ = {{VK_F4, VK_BACK}, ui_props_.menu_ui_key_state_cooldown};
    util::KeyState menu_ui_navigation = {{VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_RETURN}, ui_props_.menu_ui_navigation_key_state_cooldown};
    ankerl::unordered_dense::map<std::string, std::shared_ptr<Submenu>> submenus_;
    std::stack<std::string> submenu_stack_;
    std::size_t script_id_ = 0;
    std::atomic<bool> is_menu_opened_ = true;
    common::concurrency::RecursiveSpinlock submenus_lock_;

    // Animation state for item selector
    std::unique_ptr<base::render::animate::Lerp<std::float_t>> selector_animation_;
    std::float_t current_selector_y_ = 0.0f;
    std::chrono::steady_clock::time_point last_update_time_ = std::chrono::steady_clock::now();

    // Used when the current sub is empty
    std::shared_ptr<components::BaseComponent> fallback_option_;
    // Used when no sub can be found
    std::shared_ptr<Submenu> fallback_submenu_ = std::make_shared<Submenu>("Fallback SubMenu", [this](Submenu* sub) {;
      sub->AddComponent(components::LabelComponent("label/invalid_submenu"));
    });

  private:
    std::float_t DrawTopBar(render::DrawQueueBuffer* draw_queue, std::string_view sub_name, std::float_t y_offset);
    std::float_t DrawComponents(render::DrawQueueBuffer* draw_queue, std::float_t top_bar_y_offset, Submenu* submenu, const Submenu::component_list_t& components, std::float_t y_offset);
    void DrawComponent(render::DrawQueueBuffer* draw_queue, const components::BaseComponent* component, std::float_t y_offset, bool inverse_text) const;
    std::float_t DrawBottomBar(render::DrawQueueBuffer* draw_queue, std::size_t cur_item_idx, std::size_t item_count, std::float_t y_offset);
    std::float_t DrawItemSelector(render::DrawQueueBuffer* draw_queue, std::float_t top_bar_y_offset, const Submenu* submenu);

    std::float_t GetMenuCenterX() const;
  };

  inline MenuRenderer* kMENU_RENDERER{};
}

#endif //MENU_RENDERER_HPP_05191653
