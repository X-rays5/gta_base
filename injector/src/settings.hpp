//
// Created by X-ray on 04/04/2025.
//

#ifndef SETTIUNGS_HPP_04144242
#define SETTIUNGS_HPP_04144242
#include <base-common/result.hpp>

namespace base::injector {
  struct Settings {
    std::string dll_path;
  };

  StatusOr<Settings> LoadSettings();
  Status SaveSettings(Settings settings);
}

#endif //SETTIUNGS_HPP_04144242
