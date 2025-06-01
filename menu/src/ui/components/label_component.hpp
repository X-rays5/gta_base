//
// Created by X-ray on 31/05/2025.
//

#ifndef LABEL_COMPONENT_HPP_05184249
#define LABEL_COMPONENT_HPP_05184249
#include <string>

#include "base_component.hpp"

namespace base::menu::ui::components {
  class LabelComponent final : public BaseComponent {
  public:
    explicit LabelComponent(const std::string& label) : BaseComponent() {
      center_text_ = fmt::format("[{}]", label);
    }
  };
}

#endif //LABEL_COMPONENT_HPP_05184249
