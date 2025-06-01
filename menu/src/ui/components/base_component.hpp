//
// Created by X-ray on 31/05/2025.
//

#ifndef BASE_COMPONENT_HPP_05184123
#define BASE_COMPONENT_HPP_05184123

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

    virtual std::string_view GetLeftText() const {
      return left_text_;
    }

    virtual bool HasLeftText() const {
      return !GetLeftText().empty();
    }

    virtual std::string_view GetCenterText() const {
      return center_text_;
    }

    virtual bool HasCenterText() const {
      return !GetCenterText().empty();
    }

    virtual std::string_view GetRightText() const {
      return right_text_;
    }

    virtual bool HasRightText() const {
      return !GetRightText().empty();
    }

    virtual void HandleButtonPress(PressedButton) {}

  protected:
    std::string left_text_;
    std::string center_text_;
    std::string right_text_;
  };
}

#endif //BASE_COMPONENT_HPP_05184123
