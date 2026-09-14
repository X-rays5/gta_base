//
// Created by X-ray on 31/05/2025.
//

#ifndef MENU_RENDERER_HPP_05191653
#define MENU_RENDERER_HPP_05191653
#include <atomic>
#include <deque>
#include <ankerl/unordered_dense.h>
#include <enchantum/enchantum.hpp>
#include "submenu.hpp"
#include "theme.hpp"
#include "../render/animate.hpp"
#include "../util/key_input/key_state.hpp"
#include "../util/input/mouse_input_listener.hpp"
#include "components/label_component.hpp"
#include "header/base_header.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::menu::ui {
  class MenuRenderer final : public util::input::MouseInputListener {
  public:
    MenuRenderer();
    ~MenuRenderer();

    void RenderMenu(render::DrawQueueBuffer* draw_queue);

    void AddSubmenu(const std::string& id, Submenu&& submenu) {
      std::shared_ptr<Submenu> opened;
      {
        common::concurrency::ScopedSpinlock lock(submenus_lock_);
        submenus_.emplace(id, std::make_shared<Submenu>(std::move(submenu)));
        if (submenu_stack_.empty()) {
          submenu_stack_.push_back(id);
          // The first submenu registered becomes the root of the stack, and the root is what the
          // user is looking at from the moment the menu exists: it has been opened, not merely
          // stored. A later AddSubmenu changes nothing anybody can see, so it notifies nobody.
          opened = CurrentSubmenuLocked();
        }
      }
      OpenSubmenu(opened);
    }

    void AddSubmenu(const SubmenuIDs id, Submenu&& submenu) {
      AddSubmenu(std::string(enchantum::to_string(id)), std::move(submenu));
    }

    std::shared_ptr<Submenu> GetCurrentSubmenu() {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      if (submenu_stack_.empty()) {
        return fallback_submenu_;
      }

      const auto it = submenus_.find(submenu_stack_.back());
      return (it != submenus_.end() && it->second) ? it->second : fallback_submenu_;
    }

    Status PushSubmenu(const std::string& id) {
      std::shared_ptr<Submenu> opened;
      {
        common::concurrency::ScopedSpinlock lock(submenus_lock_);
        const auto it = submenus_.find(id);
        if (it == submenus_.end()) {
          return MakeFailure<ResultCode::kNOT_FOUND>("Submenu not found: {}", id);
        }

        submenu_stack_.push_back(id);

        // The submenu is the current one from here on, which is what its open callback is for. The
        // stack is already updated by the time it runs, so a callback that navigates further sees
        // itself as current.
        opened = CurrentSubmenuLocked();
      }
      OpenSubmenu(opened);
      return {};
    }

    bool IsOnHomeSubmenu() {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      return submenu_stack_.size() == 1;
    }

    void PopSubmenu() {
      std::shared_ptr<Submenu> opened;
      {
        common::concurrency::ScopedSpinlock lock(submenus_lock_);
        if (!IsOnHomeSubmenu()) {
          submenu_stack_.pop_back();
          // Going back is also arriving: the submenu underneath is the one being looked at now, so
          // its open callback runs here exactly as it did on the way in. Closing the menu instead
          // leaves the root current, which is not a change of submenu and so not an opening.
          opened = CurrentSubmenuLocked();
        } else {
          CloseMenu();
        }
      }
      OpenSubmenu(opened);
    }

    /**
     * Forgets a submenu entirely: it is erased from the map, and every entry of the stack that names
     * it is dropped, so a page removed while the player is standing on it lands them on the one
     * underneath rather than on an id nothing answers to.
     *
     * The renderer's keys are its own - a script's page is keyed by the id its registry handed out -
     * so this is what a script's cleanup calls, and what a test calls to put the renderer back.
     * Returns whether there was such a submenu at all.
     */
    bool RemoveSubmenu(const std::string& id) {
      std::shared_ptr<Submenu> opened;
      {
        common::concurrency::ScopedSpinlock lock(submenus_lock_);
        if (submenus_.erase(id) == 0) {
          return false;
        }

        const bool was_top = !submenu_stack_.empty() && submenu_stack_.back() == id;
        // The whole stack, not just its top: a page can be removed from the middle of it, which is
        // why the stack is a deque rather than a stack.
        std::erase(submenu_stack_, id);

        // Only a change of what is on top is a change of what the player is looking at; a page that
        // was merely somewhere further down the stack left the view alone.
        if (was_top) {
          opened = CurrentSubmenuLocked();
        }
      }
      OpenSubmenu(opened);
      return true;
    }

    bool IsMenuOpened() const {
      return is_menu_opened_;
    }

    std::shared_ptr<Theme> GetTheme() {
      return ui_props_.theme;
    }

    std::shared_ptr<Submenu> GetSubmenu(const std::string& id) {
      common::concurrency::ScopedSpinlock lock(submenus_lock_);
      const auto it = submenus_.find(id);
      return (it != submenus_.end() && it->second) ? it->second : nullptr;
    }

    std::float_t GetCurrentUiScale() const {
      return ui_props_.theme->menu_ui_scale;
    }

  private:
    /**
     * The submenu on top of the stack, or null when the stack is empty or names one that is not
     * registered. Call with submenus_lock_ held.
     */
    std::shared_ptr<Submenu> CurrentSubmenuLocked() const {
      if (submenu_stack_.empty()) {
        return nullptr;
      }

      const auto it = submenus_.find(submenu_stack_.back());
      return (it != submenus_.end() && it->second) ? it->second : nullptr;
    }

    /**
     * Tells a submenu it is the one being looked at.
     *
     * Every path that changes that - the first AddSubmenu that gives the stack a root, a PushSubmenu
     * onto it, a PopSubmenu back off it, a RemoveSubmenu of the page being stood on - ends here, so
     * that "opened" means one thing rather than four.
     *
     * Called with submenus_lock_ **released**, which is the point of it taking the pointer rather
     * than looking one up: OnOpened() takes the submenu's own spinlock, and the render path holds
     * that spinlock while it takes submenus_lock_, so running it under the lock would cross the two
     * orders and can deadlock. The caller keeps the submenu alive across the call by holding the
     * shared_ptr it passes here.
     */
    static void OpenSubmenu(const std::shared_ptr<Submenu>& submenu) {
      if (submenu) {
        submenu->OnOpened();
      }
    }

  private:
    /// Rebuilds the header the theme asks for - see MakeHeader.
    void RebuildHeader();

    /// Rebuilds only if the theme asks for a header other than the one that was built.
    void SyncHeader();

    MenuRenderProperties ui_props_{};
    util::KeyState menu_ui_key_state_ = {{VK_F4, VK_BACK}, ui_props_.menu_ui_key_state_cooldown};
    util::KeyState menu_ui_navigation = {{VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_RETURN}, ui_props_.menu_ui_navigation_key_state_cooldown};
    util::KeyState option_interaction = {{VK_F11, VK_F12}, ui_props_.option_interaction_key_state_cooldown};
    ankerl::unordered_dense::map<std::string, std::shared_ptr<Submenu>> submenus_;
    /**
     * What the player is looking at, deepest last. A deque rather than a stack because a submenu can
     * be dropped from the middle of it - see RemoveSubmenu - and not only off the top.
     */
    std::deque<std::string> submenu_stack_;

    common::concurrency::RecursiveSpinlock submenus_lock_;

    std::unique_ptr<BaseHeader> header_;
    /// The theme values header_ was built from, so that the next frame notices them changing - whether
    /// the player chose another type or image, or loaded another theme over this one.
    HeaderType header_type_ = HeaderType::kText;
    std::string header_image_path_;

    std::unique_ptr<base::render::animate::Lerp<std::float_t>> selector_animation_;
    std::unique_ptr<base::render::animate::Lerp<std::float_t>> fade_animation_;
    std::unique_ptr<base::render::animate::Lerp<std::float_t>> height_animation_;
    std::atomic<bool> is_menu_opened_ = true;
    std::float_t current_selector_y_ = 0.0f;
    std::float_t current_alpha_ = 1.0f;
    std::float_t current_menu_height_ = 0.0f;
    std::float_t target_menu_height_ = 0.0f;
    std::chrono::steady_clock::time_point last_update_time_ = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point fade_animation_update_time_ = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point height_animation_update_time_ = std::chrono::steady_clock::now();

    // Used when the current sub is empty
    std::shared_ptr<components::BaseComponent> fallback_option_;
    // Used when no sub can be found
    std::shared_ptr<Submenu> fallback_submenu_ = std::make_shared<Submenu>("Fallback SubMenu", [this](Submenu* sub) {;
      sub->AddComponent(components::LabelComponent("label/invalid_submenu"));
    });

    // Mouse state tracking
    bool mouse_in_menu_bounds_ = false;
    bool mouse_was_enabled_last_frame_ = false;

  private:
    std::float_t DrawTopBar(render::DrawQueueBuffer* draw_queue, std::string_view sub_name, std::size_t cur_item_idx, std::size_t item_count, std::float_t y_offset);
    std::float_t DrawComponents(render::DrawQueueBuffer* draw_queue, std::float_t top_bar_y_offset, Submenu* submenu, const Submenu::component_list_t& components, std::float_t y_offset);
    void DrawComponent(render::DrawQueueBuffer* draw_queue, const components::BaseComponent* component, std::float_t y_offset, bool inverse_text) const;
    std::float_t DrawBottomBar(render::DrawQueueBuffer* draw_queue, std::size_t cur_item_idx, std::size_t item_count, std::float_t y_offset) const;
    std::float_t DrawItemSelector(render::DrawQueueBuffer* draw_queue, std::float_t top_bar_y_offset, const Submenu* submenu);
    std::float_t DrawInfoBox(render::DrawQueueBuffer* draw_queue, const Submenu* submenu, std::float_t y_offset) const;

    std::float_t GetMenuCenterX() const;

    // Helper methods for fade animation
    void UpdateFadeAnimation();
    RgbColor ApplyAlphaToColor(const RgbColor& color) const;

    // Helper methods for height animation
    void UpdateHeightAnimation();
    void SetTargetMenuHeight(std::float_t target_height);

    // Menu opening/closing with animations
    void OpenMenu();
    void CloseMenu();

    // Mouse support
    void HandleMouseInput(Submenu* submenu);

    // MouseInputListener implementations
    void OnMouseLeftClick() override;
    void OnMouseRightClick() override;
    void OnMouseWheel(float delta) override;
  };

  inline MenuRenderer* kMENU_RENDERER{};
}

#endif //MENU_RENDERER_HPP_05191653
