//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_UI_27F54B7D9A92484F85FD37DD872BC063_HPP
#define GTA_BASE_UI_27F54B7D9A92484F85FD37DD872BC063_HPP
#include <stack>
#include "../../d3d/draw.hpp"
#include "theme.hpp"
#include "types.hpp"
#include "components/submenu.hpp"
#include "../../key_input/manager.hpp"

namespace gta_base::ui::draw {
  class UI {
  public:
    using submenu_ptr = std::shared_ptr<components::Submenu>;

  public:
    explicit UI(d3d::draw::DrawList* draw_list);

    inline Theme* GetTheme() {
      return &theme_;
    }

    void Tick();

    void TriggerHotkey(const std::string& opt_name);

    inline sub_id_t CreateSubmenu(const std::string& name, const components::Submenu::create_options_cb_t& create_options_cb) {
      misc::ScopedSpinlock lock(lock_);
      auto id = GetNextSubmenuId();
      submenus_[id] = std::make_shared<components::Submenu>(id, name, create_options_cb);
      return id;
    }

    template<typename... Args>
    inline sub_id_t SetRootSubmenu(Args&& ... args) {
      static const sub_id_t sub_id = 0;

      misc::ScopedSpinlock lock(lock_);
      if (submenus_.contains(sub_id)) {
        LOG_CRITICAL("Tried to register a second root");
        return -2; // invalid id
      }

      submenus_[sub_id] = std::make_shared<components::Submenu>(sub_id, std::forward<Args>(args)...);
      submenus_stack_.push(submenus_[sub_id]);

      return sub_id;
    }

    inline std::optional<submenu_ptr> GetSubmenu(sub_id_t id) {
      misc::ScopedSpinlock lock(lock_);
      if (submenus_.empty())
        return std::nullopt;

      return submenus_[id];
    }

    inline std::optional<submenu_ptr> GetRootSubmenu() {
      return GetSubmenu(0);
    }

    inline std::optional<submenu_ptr> GetCurrentSubmenu() {
      misc::ScopedSpinlock lock(lock_);
      return submenus_stack_.top();
    }

    inline void PushSubmenu(sub_id_t id) {
      misc::ScopedSpinlock lock(lock_);
      if (!submenus_.contains(id)) {
        LOG_CRITICAL("Tried to push a submenu that doesn't exist");
        return;
      }

      submenus_stack_.push(submenus_[id]);
    }

    inline void PopSubmenu() {
      misc::ScopedSpinlock lock(lock_);
      if (submenus_stack_.size() <= 1) {
        show_ui_ = false;
        return;
      }

      submenus_stack_.pop();
    }

  private:
    misc::Spinlock lock_;
    std::atomic<bool> show_ui_{true};
    sub_id_t next_sub_id_{}; // While this could technically run out of IDs, it's unlikely to happen.
    std::stack<submenu_ptr> submenus_stack_;
    robin_hood::unordered_map<sub_id_t, submenu_ptr> submenus_;
    d3d::draw::DrawList* draw_list_;
    Theme theme_;

    // key_binds
    key_input::CooldownInput toggle_ui_input_{(std::uint32_t) key_input::KeyBinds::ui_toggle, 200};
    key_input::CooldownInput left_input_{(std::uint32_t) key_input::KeyBinds::ui_left, 140};
    key_input::CooldownInput right_input_{(std::uint32_t) key_input::KeyBinds::ui_right, 140};
    key_input::CooldownInput up_input_{(std::uint32_t) key_input::KeyBinds::ui_up, 100};
    key_input::CooldownInput down_input_{(std::uint32_t) key_input::KeyBinds::ui_down, 100};
    key_input::CooldownInput select_input_{(std::uint32_t) key_input::KeyBinds::ui_select, 300};
    key_input::CooldownInput back_input_{(std::uint32_t) key_input::KeyBinds::ui_back, 300};
    key_input::CooldownInput hotkey_create_input_{(std::uint32_t) key_input::KeyBinds::ui_create_hotkey, 300};
    key_input::CooldownInput save_option_input_{(std::uint32_t) key_input::KeyBinds::ui_save_option, 300};
    key_input::CooldownInput modify_value_input_{(std::uint32_t) key_input::KeyBinds::ui_modify_value, 200};

  private:
    sub_id_t GetNextSubmenuId() {
      return next_sub_id_++;
    }

    void HandleKeyInput();
    static void RegisterKeyBinds();
  };
}
#endif //GTA_BASE_UI_27F54B7D9A92484F85FD37DD872BC063_HPP
