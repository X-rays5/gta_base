//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#define GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
#include <filesystem>

namespace base::util {
  class Vfs {
  public:
    Vfs() {
      // TODO: use the win32 api to get directories like this
      auto appdata_path = std::filesystem::path(std::getenv("APPDATA")) / "base";
      std::filesystem::create_directories(appdata_path);
      std::filesystem::current_path(appdata_path);
    }

    static Vfs& Get() {
      static Vfs inst;
      return inst;
    }

  private:

  private:

  };
}
#endif //GTA_BASE_VFS_16AC40FC6BC24763B42CAF7CBB740E5B_HPP
