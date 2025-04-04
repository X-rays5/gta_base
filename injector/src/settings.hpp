//
// Created by X-ray on 04/04/2025.
//

#ifndef SETTIUNGS_HPP_04144242
#define SETTIUNGS_HPP_04144242
#include <base-common/globals.hpp>
#include <base-common/result.hpp>

namespace base::injector {
  struct Settings {
    std::string dll_path;
    std::string target_window_name = common::globals::target_window_name;
    std::string target_window_class = common::globals::target_window_class;
    std::string target_process_name = common::globals::target_process_name;
  };

  StatusOr<Settings> LoadSettings();
  Status SaveSettings(Settings settings);
}

#endif //SETTIUNGS_HPP_04144242
