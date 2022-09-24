//
// Created by X-ray on 09/24/22.
//

#pragma once
#ifndef GTA_BASE_CONVERT_HPP
#define GTA_BASE_CONVERT_HPP
#include <filesystem>

namespace lua_codegen::convert {
  bool ConvertFile(const std::filesystem::path& path);
}
#endif //GTA_BASE_CONVERT_HPP
