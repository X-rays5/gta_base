//
// Created by X-ray on 08/04/2025.
//

#ifndef GLAZE_FORMATTER_HPP_04004256
#define GLAZE_FORMATTER_HPP_04004256
#include <fmt/format.h>
#include <glaze/glaze.hpp>

template <>
struct fmt::formatter<glz::error_ctx> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(const glz::error_ctx& err, FormatContext& ctx) const {
    return formatter<std::string_view>::format(glz::format_error(err, std::string{}), ctx);
  }
};
#endif //GLAZE_FORMATTER_HPP_04004256
