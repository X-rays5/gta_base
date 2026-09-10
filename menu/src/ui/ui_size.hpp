//
// Created by X-ray on 10/09/2026.
//

#pragma once
#include <glaze/core/meta.hpp>

namespace base::menu::ui {
  struct UiSize {
    explicit constexpr UiSize(const std::float_t value) : value(value) {}

    operator std::float_t() const;

    [[nodiscard]] std::float_t Scaled() const;
    [[nodiscard]] constexpr std::float_t Raw() const;

    std::float_t value;
  };
}

template <>
  struct glz::meta<::base::menu::ui::UiSize> {
  using T = ::base::menu::ui::UiSize;

  static constexpr auto value = &T::value;
};