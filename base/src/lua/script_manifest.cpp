//
// Created by X-ray on 12/12/22.
//

#include "script_manifest.hpp"

namespace gta_base::lua {
  namespace {
    std::optional<sol::state> CreateManifestState(const std::filesystem::path& script_file) {
      sol::state lua_state;
      lua_state.open_libraries(sol::lib::base, sol::lib::string);

      sol::protected_function_result script = lua_state.safe_script_file(script_file.string());
      if (!script.valid()) {
        LOG_ERROR("Failed to load manifest file: {}\r\n", script_file.string(), script.get<sol::error>().what());
        return std::nullopt;
      }

      return lua_state;
    }
  }

  Manifest::Manifest(const std::filesystem::path& script_dir) {
    auto lua_state_opt = CreateManifestState(script_dir / globals::lua_manifest_file_name);
    if (!lua_state_opt.has_value()) {
      LOG_ERROR("Failed to load manifest file: {}", (script_dir / globals::lua_manifest_file_name).string());
      return;
    }
    auto lua_state = std::move(*lua_state_opt);

    auto main_file = lua_state["main_file"];
    if (main_file.valid() && main_file.get_type() == sol::type::string) {
      main_file_ = (script_dir / main_file.get<std::string>()).string();
    } else {
      LOG_ERROR("Failed to get required field main_file from manifest.lua");
    }

    auto name = lua_state["name"];
    if (name.valid() && name.get_type() == sol::type::string) {
      name_ = name.get<std::string>();
    } else {
      LOG_ERROR("Failed to get required field name from manifest.lua");
    }

    auto description = lua_state["description"];
    if (description.valid() && description.get_type() == sol::type::string) {
      description_ = description.get<std::string>();
    }

    auto version = lua_state["version"];
    if (version.valid() && version.get_type() == sol::type::string) {
      version_ = version.get<std::string>();
    } else {
      LOG_ERROR("Failed to get required field version from manifest.lua");
    }

    auto authors = lua_state["authors"];
    if (authors.valid()) {
      if (authors.get_type() == sol::type::table && authors.is<std::vector<std::string>>()) {
        authors_ = authors.get<std::vector<std::string>>();
      }
    }

    auto author = lua_state["author"];
    if (author.valid() && author.get_type() == sol::type::string) {
      if (authors_.empty()) {
        authors_.push_back(author.get<std::string>());
      } else {
        LOG_WARN("Both author and authors fields are present in manifest.lua, using authors");
      }
    }

    auto repository = lua_state["repository"];
    if (repository.valid() && repository.get_type() == sol::type::string) {
      repository_ = repository.get<std::string>();
    }

    auto license = lua_state["license"];
    if (license.valid() && license.get_type() == sol::type::string) {
      license_ = license.get<std::string>();
    }

    auto license_file = lua_state["license_file"];
    if (license_file.valid() && license_file.get_type() == sol::type::string) {
      license_file_ = license_file.get<std::string>();
    }

    auto hidden_lib = lua_state["hidden_lib"];
    if (hidden_lib.valid() && hidden_lib.get_type() == sol::type::boolean) {
      hidden_lib_ = hidden_lib.get<bool>();
    }
  }
}