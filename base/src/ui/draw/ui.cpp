//
// Created by X-ray on 02/09/23.
//

#include "ui.hpp"
#include "list_ui.hpp"
#include "components/options/submenu_link.hpp"
#include "../../settings/option_state.hpp"
#include "../../misc/hotkey_manager.hpp"

namespace gta_base::ui::draw {
  UI::UI(d3d::draw::DrawList* draw_list) : draw_list_(draw_list) {
    RegisterKeyBinds();
  }

  void UI::Tick() {
    HandleKeyInput();

    if (!show_ui_) {
      return;
    }

    auto cur_sub = GetCurrentSubmenu();
    if (!cur_sub.has_value()) {
      LOG_CRITICAL("Current submenu is a nullptr");
      return;
    }
    list_ui::Draw(&theme_, cur_sub->get(), draw_list_);
  }

  void UI::TriggerHotkey(const std::string& opt_name) {
    misc::ScopedSpinlock lock(lock_);
    for (auto&& sub : submenus_) {
      sub.second->IterateOptions([&](const std::shared_ptr<components::BaseOption>& opt) {
        if (opt->GetNameRaw() == opt_name && opt->HasFlag(flags::OptionFlags::kHotkeyAble))
          opt->HandleKey(key_input::KeyBinds::ui_select);
      });
    }
  }

  void UI::HandleKeyInput() {
    if (toggle_ui_input_.Get()) {
      show_ui_ = !show_ui_;
      return;
    }
    if (!show_ui_)
      return;

    auto cur_sub_opt = GetCurrentSubmenu();
    if (!cur_sub_opt.has_value()) {
      LOG_CRITICAL("Current submenu is a nullptr");
      return;
    }
    auto cur_sub = cur_sub_opt->get();

    if (back_input_.Get()) {
      PopSubmenu();
    } else if (select_input_.Get()) {
      auto sub_link = dynamic_cast<components::SubmenuLink*>(cur_sub->GetSelectedOption().get());
      if (sub_link) {
        PushSubmenu(sub_link->GetLinkDestination());
      }
    } else if (up_input_.Get()) {
      cur_sub->ScrollUp();
    } else if (down_input_.Get()) {
      cur_sub->ScrollDown();
    } else if (left_input_.Get()) {
      cur_sub->GetSelectedOption()->HandleKey(key_input::KeyBinds::ui_left);
    } else if (right_input_.Get()) {
      cur_sub->GetSelectedOption()->HandleKey(key_input::KeyBinds::ui_right);
    } else if (modify_value_input_.Get()) {
      cur_sub->GetSelectedOption()->HandleKey(key_input::KeyBinds::ui_modify_value);
    } else if (save_option_input_.Get()) {
      auto cur_opt = cur_sub->GetSelectedOption();
      if (!kSETTINGS.menu.save_option_state_on_exit && cur_opt->HasFlag(flags::OptionFlags::kSaveAble)) {
        settings::option_state::SaveSingle(settings::option_state::GetSavePath(), cur_opt->GetNameRaw(), cur_opt->Serialize());
      }
    } else if (hotkey_create_input_.Get()) {
      auto cur_opt = cur_sub->GetSelectedOption();
      if (cur_opt->HasFlag(flags::OptionFlags::kHotkeyAble)) {
        misc::kHOTKEY_MANAGER->StartHotkeyAdd(cur_opt->GetNameRaw());
      }
    }
  }

  void UI::RegisterKeyBinds() {
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_toggle, {key_input::Keyboard::INPUT::F4}, {key_input::Controller::INPUT::LEFT_SHOULDER, key_input::Controller::INPUT::RIGHT_SHOULDER});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_up, {key_input::Keyboard::INPUT::UP_ARROW}, {key_input::Controller::INPUT::DPAD_UP});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_down, {key_input::Keyboard::INPUT::DOWN_ARROW}, {key_input::Controller::INPUT::DPAD_DOWN});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_left, {key_input::Keyboard::INPUT::LEFT_ARROW}, {key_input::Controller::INPUT::DPAD_LEFT});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_right, {key_input::Keyboard::INPUT::RIGHT_ARROW}, {key_input::Controller::INPUT::DPAD_RIGHT});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_select, {key_input::Keyboard::INPUT::RETURN}, {key_input::Controller::INPUT::A});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_back, {key_input::Keyboard::INPUT::BACKSPACE}, {key_input::Controller::INPUT::B});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_create_hotkey, {key_input::Keyboard::INPUT::F1}, {key_input::Controller::INPUT::RIGHT_SHOULDER, key_input::Controller::INPUT::DPAD_UP});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_save_option, {key_input::Keyboard::INPUT::F11}, {key_input::Controller::INPUT::RIGHT_SHOULDER, key_input::Controller::INPUT::DPAD_DOWN});
    key_input::kINPUT_MGR->AddKeybinding((std::uint32_t) key_input::KeyBinds::ui_modify_value, {key_input::Keyboard::INPUT::SHIFT, key_input::Keyboard::INPUT::F5}, {key_input::Controller::INPUT::RIGHT_SHOULDER, key_input::Controller::INPUT::DPAD_LEFT});
  }
}