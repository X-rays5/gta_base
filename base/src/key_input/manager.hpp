//
// Created by X-ray on 02/16/23.
//

#pragma once
#ifndef GTA_BASE_MANAGER_808024A174394D97AB8B6B732D0FAB3A_HPP
#define GTA_BASE_MANAGER_808024A174394D97AB8B6B732D0FAB3A_HPP
#include "keyboard.hpp"
#include "controller.hpp"
#include "cooldown_input.hpp"

namespace gta_base::key_input {
  enum class KeyBinds : std::uint32_t {
    ui_toggle,
    ui_up,
    ui_down,
    ui_left,
    ui_right,
    ui_select,
    ui_back,
    ui_create_hotkey,
    ui_save_option,
    ui_modify_value,
  };

  class Manager {
  public:
    struct KeyBinding {
      std::vector<Keyboard::INPUT> keyboard_key_bind;
      std::vector<Controller::INPUT> controller_key_bind;
    };

  public:
    Manager();
    ~Manager();

    FORCE_INLINE void PushBlockInput() {
      misc::ScopedSpinlock lock(block_input_lock_);
      block_push_input_count_ += 1;
    }

    FORCE_INLINE void PopBlockInput() {
      misc::ScopedSpinlock lock(block_input_lock_);
      GTA_BASE_ASSERT(block_push_input_count_ > 0, "There is nothing to pop.");
      if (block_push_input_count_ == 0)
        return;

      block_push_input_count_ -= 1;
    }

    FORCE_INLINE bool IsInputBlocked() {
      misc::ScopedSpinlock lock(block_input_lock_);
      return block_push_input_count_;
    }

    bool IsControlPressed(std::uint32_t key_bind_id);
    bool IsControlJustPressed(std::uint32_t key_bind_id);

    bool AddKeybinding(std::uint32_t key_bind_id, std::initializer_list<Keyboard::INPUT> keyboard_bind, std::initializer_list<Controller::INPUT> controller_bind);
    void RemoveKeybinding(std::uint32_t key_bind_id);

  private:
    misc::Spinlock block_input_lock_;
    std::uint64_t block_push_input_count_{};
    Keyboard keyboard_;
    Controller controller_;
    misc::Spinlock key_binds_lock_;
    robin_hood::unordered_map<std::uint32_t, KeyBinding> key_binds_;
  };

  inline Manager* kINPUT_MGR{};
}
#endif //GTA_BASE_MANAGER_808024A174394D97AB8B6B732D0FAB3A_HPP
