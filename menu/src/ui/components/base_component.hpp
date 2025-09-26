//
// Created by X-ray on 31/05/2025.
//

#ifndef BASE_COMPONENT_HPP_05184123
#define BASE_COMPONENT_HPP_05184123
#include "component_flags.hpp"
#include "../localization/manager.hpp"

namespace base::menu::ui::components {
  class BaseComponent {
  public:
    enum class PressedButton {
      kLEFT,
      kRIGHT,
      kSUBMIT
    };

  public:
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

    virtual void HandleButtonPress(PressedButton) {}

    virtual bool HasFlag(const Flags flag) const {
      return (flags_ & flag) != Flags::None;
    }

  protected:
    std::string left_text_;
    std::string center_text_;
    std::string right_text_;
    Flags flags_ = Flags::None;
  };
}

#endif //BASE_COMPONENT_HPP_05184123
