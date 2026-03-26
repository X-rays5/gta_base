//
// Created by X-ray on 31/05/2025.
//

#ifndef THEME_HPP_05125856
#define THEME_HPP_05125856
#include <base-common/util/ranged_value.hpp>
#include <imgui/imgui.h>
#include <glaze/core/meta.hpp>

namespace base::menu::ui {
  struct RdrColor {
    RdrColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    union {
      struct {
        std::uint8_t r, g, b, a;
      };
      std::uint32_t value;
    };

    operator ImU32() const {
      return value;
    }
  };

  struct Text {
    const std::string font = "roboto-regular";
    const std::string font_bold = "roboto-bold";
    const std::float_t x_margin = 0.005f;
    const std::float_t y_margin = 0.005f;
    const std::float_t font_size = 0.012f;
    const std::float_t scroll_indicator_size = 0.02f;
    const RdrColor text_color = RdrColor(240, 240, 240);
    const RdrColor inverse_text_color = RdrColor(47, 47, 47);
    const RdrColor sec_text_color = RdrColor(155, 155, 155);
  };

  struct Theme {
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> x_position;
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> y_position;
    const RdrColor seperator_color = RdrColor(255, 255, 255);
    const RdrColor background_color = RdrColor(0, 0, 0);
    Text text_props;

    Status Save(const std::string& theme_name);
    Status Load(const std::string& theme_name);
  };

  struct MenuRenderProperties {
    std::shared_ptr<Theme> theme = std::make_shared<Theme>();
    const std::chrono::milliseconds menu_ui_key_state_cooldown = std::chrono::milliseconds(200);
    const std::chrono::milliseconds menu_ui_navigation_key_state_cooldown = std::chrono::milliseconds(200);
    const std::float_t header_height = 0.1f;
    const std::float_t seperator_height = 0.001f;
    const std::float_t menu_item_height = 0.025f;
    const std::float_t menu_width = 0.16f;
    const std::uint32_t max_options_drawn = 12;
    const RdrColor selector_color = theme->text_props.text_color;
  };
}

template <>
struct glz::meta<::base::menu::ui::RdrColor> {
  using T = ::base::menu::ui::RdrColor;
  static constexpr auto value = object(
    &T::r,
    &T::g,
    &T::b,
    &T::a
  );
};

template <>
struct glz::meta<base::menu::ui::Text> {
  static constexpr bool skip(const std::string_view key, const glz::meta_context&) {
    return key == "font" ||
      key == "font_bold" ||
        key == "x_margin" ||
          key == "y_margin" ||
            key == "font_size" ||
              key == "scroll_indicator_size";
  }
};

#endif //THEME_HPP_05125856
