//
// Created by X-ray on 03/02/2024.
//

#ifndef FORMATTER_HPP_02215435
#define FORMATTER_HPP_02215435
#include <fmt/format.h>
#include <filesystem>

template <>
struct fmt::formatter<std::filesystem::path> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(const std::filesystem::path &path, FormatContext &ctx) const {
    return formatter<std::string_view>::format(path.string(), ctx);
  }
};
#endif //FORMATTER_HPP_02215435