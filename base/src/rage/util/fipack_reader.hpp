//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_FIPACK_READER_HPP
#define GTA_BASE_FIPACK_READER_HPP
#include <string>
#include <filesystem>
#include <functional>
#include <memory>
#include <pugixml.hpp>
#include "../classes/fidevice.hpp"

namespace rage::util {
class FipackReader {
 public:
  using file_contents_cb_t = std::function<void(const std::unique_ptr<std::uint8_t[]> &file_content, const std::int32_t data_size)>;
  using xml_file_contents_cb_t = std::function<void(const pugi::xml_document &doc)>;

 public:
  FipackReader(rage::fiPackfile *rpf, std::string mount_name);

  /// @note This should be called in a fiber job
  static void ForEachFipackFile(const std::function<std::size_t(FipackReader &rpf_wrapper)> &cb);
  std::vector<std::filesystem::path> GetFilePaths(std::string parent = "");

  void ReadFile(const std::filesystem::path &path, file_contents_cb_t &&cb);
  void ReadXmlFile(const std::filesystem::path &path, xml_file_contents_cb_t cb);

 private:
  static constexpr const char *default_mount_name = "fipack_mount:/";
  rage::fiPackfile *rpf_;
  std::string mount_name_;
};
}
#endif //GTA_BASE_FIPACK_READER_HPP
