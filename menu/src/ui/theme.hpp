//
// Created by X-ray on 31/05/2025.
//

#ifndef THEME_HPP_05125856
#define THEME_HPP_05125856
#include <base-common/util/ranged_value.hpp>
#include <glaze/core/meta.hpp>
#include <imgui/imgui.h>
#include "ui_size.hpp"

namespace base::menu::ui {
  struct RgbColor {
    constexpr RgbColor(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    RgbColor(RgbColor&&) = default;
    RgbColor& operator=(RgbColor&&) = default;
    RgbColor(const RgbColor&) = default;
    RgbColor& operator=(const RgbColor&) = default;

    union {
      struct {
        std::uint8_t r, g, b, a;
      };
      std::uint32_t value;
    };

    constexpr operator ImU32() const {
      return value;
    }
  };

  struct Text {
    const std::string font = "roboto-regular";
    const std::string font_bold = "roboto-bold";
    const std::float_t x_margin = 0.005f;
    const std::float_t y_margin = 0.005f;
    const UiSize font_size = UiSize(0.012f);
    const UiSize scroll_indicator_size = UiSize(0.02f);
    const RgbColor text_color = RgbColor(240, 240, 240);
    const RgbColor inverse_text_color = text_color;
    const RgbColor sec_text_color = RgbColor(155, 155, 155);
  };

  struct Theme {
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> x_position;
    common::util::RangedValue<std::float_t, 0.005F, 1.0F> y_position;
    common::util::RangedValue<std::float_t, 0.2F, 2.0F> menu_ui_scale = 1.0f;
    const RgbColor seperator_color = RgbColor(74, 144, 226);
    const RgbColor background_color = RgbColor(30, 30, 30);
    Text text_props;
    std::atomic<bool> mouse_input_enabled = false;

    Status Save(const std::string& theme_name);
    Status Load(const std::string& theme_name);
  };

  struct NotificationRenderProperties {
    const UiSize notification_width = UiSize(0.12f);
    const UiSize notification_height = UiSize(0.08f);
    const UiSize title_text_size = UiSize(0.013f);
    const UiSize message_text_size = UiSize(0.01f);
    const UiSize x_margin = UiSize(0.006f);
    const UiSize y_margin = UiSize(0.005f);
    const UiSize text_margin = UiSize(0.002f);
  };

  struct MenuRenderProperties {
    std::shared_ptr<Theme> theme = std::make_shared<Theme>();
    const std::chrono::milliseconds menu_ui_key_state_cooldown = std::chrono::milliseconds(200);
    const std::chrono::milliseconds menu_ui_navigation_key_state_cooldown = std::chrono::milliseconds(200);
    const std::chrono::milliseconds option_interaction_key_state_cooldown = std::chrono::milliseconds(500);
    const UiSize header_height = UiSize(0.1f);
    const UiSize seperator_height = UiSize(0.0015f);
    const UiSize menu_item_height = UiSize(0.025f);
    const UiSize menu_width = UiSize(0.16f);
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
