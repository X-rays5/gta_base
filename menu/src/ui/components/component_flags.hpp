//
// Created by X-ray on 01/06/2025.
//

#ifndef COMPONENT_FLAGS_HPP_06042934
#define COMPONENT_FLAGS_HPP_06042934
#include <base-common/util/enum_bitops.hpp>

namespace base::menu::ui::components {
  enum class Flags : std::uint32_t {
    None = 0,
    NotSelectable = 1 << 0,
  };

}

#endif //COMPONENT_FLAGS_HPP_06042934
