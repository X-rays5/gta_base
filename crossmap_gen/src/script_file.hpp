//
// Created by X-ray on 12/14/22.
//

#pragma once
#ifndef GTA_BASE_SCRIPT_FILE_HPP
#define GTA_BASE_SCRIPT_FILE_HPP
#include <filesystem>

namespace crossmap_gen::script {
  class File {
  public:
    struct Native {
      std::uint8_t native_index;
      std::uint32_t offset;
      std::uint8_t return_and_arg_data;
    };

  public:
    explicit File(const std::filesystem::path& path);

  private:
    std::filesystem::path file_path_;

  private:
    void ParseFile();
  };
}
#endif //GTA_BASE_SCRIPT_FILE_HPP
