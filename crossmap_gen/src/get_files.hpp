//
// Created by X-ray on 12/14/22.
//

#pragma once
#ifndef GTA_BASE_GET_FILES_HPP
#define GTA_BASE_GET_FILES_HPP
#include <filesystem>

namespace crossmap_gen::files {
  struct ScriptSet {
    std::filesystem::path old_path;
    std::filesystem::path new_path;
  };

  std::vector<ScriptSet> GetFiles(const std::filesystem::path& base);
}
#endif //GTA_BASE_GET_FILES_HPP
