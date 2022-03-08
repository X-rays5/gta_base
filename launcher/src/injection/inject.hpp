//
// Created by X-ray on 3/8/2022.
//

#pragma once

#ifndef GTA_BASE_INJECT_HPP
#define GTA_BASE_INJECT_HPP
#include <filesystem>

namespace launcher {
    namespace inject {
      bool Inject(const char* target_process_name, const std::filesystem::path& dll_path);
    }// namespace inject
}// namespace launcher
#endif //GTA_BASE_INJECT_HPP
