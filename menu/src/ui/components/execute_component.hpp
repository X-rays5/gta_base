//
// Created by X-ray on 01/06/2025.
//

#ifndef EXECUTE_COMPONENT_HPP_06150022
#define EXECUTE_COMPONENT_HPP_06150022
#include "base_component.hpp"

namespace base::menu::ui::components {
  template <typename Func>
  class ExecuteComponent : public BaseComponent {

  public:
    ExecuteComponent(const std::string& name, const Func handler) :
      BaseComponent(),
      exec_handler_(handler) {
      left_text_ = name;
    };

    virtual void HandleButtonPress(const PressedButton button) override {
      if (button == PressedButton::kSUBMIT) {
        exec_handler_();
      }
    }

  private:
    Func exec_handler_;
  };
}

#endif //EXECUTE_COMPONENT_HPP_06150022
