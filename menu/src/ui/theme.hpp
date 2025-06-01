//
// Created by X-ray on 31/05/2025.
//

#ifndef THEME_HPP_05125856
#define THEME_HPP_05125856
#include <base-common/util/ranged_value.hpp>
#include <imgui/imgui.h>

namespace base::menu::ui {
  struct Theme {
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> x_position;
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> y_position;

    Status Save(const std::string& theme_name);
    Status Load(const std::string& theme_name);
  };

  struct Text {
    const std::float_t x_margin = 0.005f;
    const std::float_t y_margin = 0.005f;
    const std::float_t font_size = 0.012f;
    const ImColor text_color = ImColor(240, 240, 240);
    const ImColor inverse_text_color = ImColor(47, 47, 47);
    const ImColor sec_text_color = ImColor(155, 155, 155);
  };

  struct MenuRenderProperties {
    Theme theme;
    Text text_props;
    const std::float_t header_height = 0.1f;
    const std::float_t seperator_height = 0.001f;
    const std::float_t menu_item_height = 0.025f;
    const std::float_t menu_width = 0.16f;
    const std::uint32_t max_options_drawn = 16;
    const ImColor seperator_color = ImColor(255, 255, 255);
    const ImColor background_color = ImColor(0, 0, 0);
  };
}

#endif //THEME_HPP_05125856
