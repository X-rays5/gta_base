//
// Created by X-ray on 06/05/2023.
//

module;
import <cstdlib>;
import <filesystem>;
import <string>;

export module fs;

export namespace menu::fs {
  class VirtualFileSystem {
  public:
    VirtualFileSystem() {
      root_ = std::string(std::getenv("appdata")) + "menu";

      if (!std::filesystem::exists(root_))
        std::filesystem::create_directories(root_);
    }

    std::filesystem::path GetPath(const std::filesystem::path& path) {
      return root_ / path;
    }

  private:
    std::filesystem::path root_;

  private:

  };
  inline VirtualFileSystem kVFS{};
}
