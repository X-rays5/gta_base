//
// Created by X-ray on 28/03/2026.
//

#include "base_component.hpp"
#include "../../hotkey/hotkey_manager.hpp"
#include "../../options/base_option.hpp"
#include "../../options/option_registry.hpp"

namespace base::menu::ui::components {
  bool BaseComponent::IsHotkeyAble() const {
    return option_ ? option_->IsHotkeyAble() : false;
  }

  void BaseComponent::AddNewHotkey() const {
    if (!option_ || !option_->IsHotkeyAble()) {
      LOG_ERROR("Cannot add hotkey for component '{}', no associated option or option is not hotkey-able", GetLeftText());
      return;
    }

    const auto opt = options::kOPTION_REGISTRY->GetOption(option_->GetName());
    if (!opt) {
      LOG_ERROR("Cannot add hotkey for component '{}', associated option '{}' not found in registry", GetLeftText(), option_->GetName());
      return;
    }

    hotkey::kHOTKEY_MANAGER->AddNewHotkey(opt);
  }

  std::optional<std::string> BaseComponent::GetHotkeyText() const {
    // An option that cannot take a hotkey is not worth the lookup, and neither is a menu with no hotkey
    // manager behind it - which is a test or a harness rather than a running game, and there is nothing
    // bound to anything in one.
    if (!IsHotkeyAble() || !hotkey::kHOTKEY_MANAGER) {
      return std::nullopt;
    }

    // Looked up by name rather than by pointer: the manager holds whatever the registry handed it when
    // the key was bound, which is not necessarily the same option object this component was built with.
    const auto hotkey = hotkey::kHOTKEY_MANAGER->GetHotkeyForOption(option_->GetName());
    if (!hotkey) {
      return std::nullopt;
    }

    return hotkey->AsString();
  }

  bool BaseComponent::IsSavable() const {
    return option_ ? option_->IsSavable() : false;
  }

  void BaseComponent::Save() const {
    if (!IsSavable() && !option_) {
      LOG_ERROR("Component '{}' is not savable and has no associated option, cannot save", GetLeftText());
      return;
    }

    auto status = options::kOPTION_REGISTRY->SaveOption(option_);
    if (!status) {
      LOG_ERROR("Failed to save option for component '{}': {}", GetLeftText(), status);
    }
  }
}
