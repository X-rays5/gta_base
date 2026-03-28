//
// Created by X-ray on 31/05/2025.
//

#ifndef THEME_HPP_05125856
#define THEME_HPP_05125856
#include <base-common/util/ranged_value.hpp>
#include <imgui/imgui.h>
#include <glaze/core/meta.hpp>

namespace base::menu::ui {
  struct RgbColor {
    RgbColor(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

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
    const RgbColor text_color = RgbColor(240, 240, 240);
    const RgbColor inverse_text_color = text_color;
    const RgbColor sec_text_color = RgbColor(155, 155, 155);
  };

  struct Theme {
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> x_position;
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> y_position;
    const RgbColor seperator_color = RgbColor(74, 144, 226);
    const RgbColor background_color = RgbColor(30, 30, 30);
    Text text_props;

    Status Save(const std::string& theme_name);
    Status Load(const std::string& theme_name);
  };

  struct MenuRenderProperties {
    std::shared_ptr<Theme> theme = std::make_shared<Theme>();
    const std::chrono::milliseconds menu_ui_key_state_cooldown = std::chrono::milliseconds(200);
    const std::chrono::milliseconds menu_ui_navigation_key_state_cooldown = std::chrono::milliseconds(200);
    const std::float_t header_height = 0.1f;
    const std::float_t seperator_height = 0.0015f;
    const std::float_t menu_item_height = 0.025f;
    const std::float_t menu_width = 0.16f;
    const std::uint32_t max_options_drawn = 12;
    const RgbColor selector_color = RgbColor(47, 47, 47);
  };
}

template <>
struct glz::meta<::base::menu::ui::RgbColor> {
  using T = ::base::menu::ui::RgbColor;
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
