//
// Created by X-ray on 31/05/2025.
//

#ifndef BASE_COMPONENT_HPP_05184123
#define BASE_COMPONENT_HPP_05184123
#include <optional>
#include "component_flags.hpp"
#include "../localization/manager.hpp"

namespace base::menu::options {
  class BaseOption;
}

namespace base::menu::ui {
  class Submenu;
}

namespace base::menu::ui::components {
  class BaseComponent {
  public:
    enum class PressedButton {
      kLEFT,
      kRIGHT,
      kSUBMIT
    };

  public:
    friend Submenu;

    explicit BaseComponent(options::BaseOption* opt = nullptr) : option_(opt) {}
    virtual ~BaseComponent() = default;

    virtual std::string GetLeftText() const {
      return localization::kMANAGER->Localize(left_text_);
    }

    virtual bool HasLeftText() const {
      return !GetLeftText().empty();
    }

    virtual std::string GetCenterText() const {
      return localization::kMANAGER->Localize(center_text_);
    }

    virtual bool HasCenterText() const {
      return !GetCenterText().empty();
    }

    virtual std::string GetRightText() const {
      return localization::kMANAGER->Localize(right_text_);
    }

    virtual bool HasRightText() const {
      return !GetRightText().empty();
    }

    virtual std::string GetDescription() const {
      if (description_.empty()) {
        const auto tmp_key = left_text_ + "/desc";
        const auto locale =  localization::kMANAGER->Localize(tmp_key);
        return locale != tmp_key ? locale : "";
      }

      return localization::kMANAGER->Localize(description_);
    }

    virtual bool HasDescription() const {
      return !GetDescription().empty();
    }

    virtual void HandleButtonPress(PressedButton) {}

    virtual bool HasFlag(const Flags flag) const {
      return (flags_ & flag) != Flags::None;
    }

    bool IsHotkeyAble() const;
    void AddNewHotkey() const;

    /**
     * The key this component's option is bound to, written the way the hotkey list writes it - so that
     * the two read the same way - or nothing when it is bound to none.
     *
     * Nothing is the answer for two different situations and they are the same to a caller: an option
     * that cannot take a hotkey never has a key to show, and an option that could take one but has not
     * been given any yet has none to show either. What tells those apart is IsHotkeyAble(), which is
     * what a caller that needs to say "you could set one" has to ask in addition.
     */
    [[nodiscard]] std::optional<std::string> GetHotkeyText() const;

    bool IsSavable() const;
    void Save() const;

  protected:
    std::string left_text_;
    std::string center_text_;
    std::string right_text_;
    std::string description_;
    Flags flags_ = Flags::None;
    options::BaseOption* option_ = nullptr;

    virtual BaseComponent& SetOption(options::BaseOption* opt) {
      option_ = opt;
      return *this;
    }

  };
}

#endif //BASE_COMPONENT_HPP_05184123
