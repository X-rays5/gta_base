//
// Created by X-ray on 12/12/22.
//

#pragma once
#ifndef GTA_BASE_SCRIPT_INFO_HPP
#define GTA_BASE_SCRIPT_INFO_HPP
#include <filesystem>
#include "script_manifest.hpp"
#include "script.hpp"

namespace gta_base::lua {
struct ScriptInfo {
  Manifest manifest;
  std::filesystem::path script_dir;
  std::unique_ptr<Script> lua_script_inst;
};
}
#endif //GTA_BASE_SCRIPT_INFO_HPP
