//
// Created by X-ray on 12/14/22.
//

#include "get_files.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace crossmap_gen::files {
  namespace {
    std::filesystem::path GetScriptsBaseDir(const std::filesystem::path& base, const std::string& dir_name) {
      std::filesystem::path scripts_dir = base / dir_name;
      if (!std::filesystem::is_directory(scripts_dir)) {
        LOG(INFO) << scripts_dir.string() << " doesn't exist";
        scripts_dir_input:
        std::cout << "Input path to " << dir_name << " dir: ";
        std::string input;
        std::getline(std::cin, input);

        scripts_dir = input;
        if (!std::filesystem::is_directory(scripts_dir)) {
          LOG(ERROR) << scripts_dir.string() << " doesn't exist";
          goto scripts_dir_input;
        }
      }

      return scripts_dir;
    }

    std::unordered_map<std::string, std::filesystem::path> GetScriptFiles(const std::filesystem::path& dir) {
      std::filesystem::directory_iterator it(dir);
      std::unordered_map<std::string, std::filesystem::path> files;

      for (const auto& entry : it) {
        if (entry.is_directory()) {
          if (entry.path().filename().string().ends_with("_ysc")) {
            auto file_name = entry.path().filename().string();
            // replace _ysc with .ysc
            file_name.replace(file_name.size() - 4, 4, ".ysc");
            file_name.append(".full");
            if (std::filesystem::is_regular_file(entry.path() / file_name)) {
              files[file_name] = (entry.path() / file_name);
            } else {
              LOG(WARNING) << "Couldn't find " << file_name << " in " << entry.path().string();
            }
          }
        }
      }

      return files;
    }
  }

  std::vector<ScriptSet> GetFiles(const std::filesystem::path& base) {
    auto old_script_dir = GetScriptsBaseDir(base, "old_scripts");
    auto new_scripts_dir = GetScriptsBaseDir(base, "new_scripts");

    auto old_scripts = GetScriptFiles(old_script_dir);
    auto new_scripts = GetScriptFiles(new_scripts_dir);

    // only keep files that are in both dirs
    std::vector<ScriptSet> files;
    for (const auto& [file_name, old_path] : old_scripts) {
      if (auto it = new_scripts.find(file_name); it != new_scripts.end()) {
        files.push_back({old_path, it->second});
      }
    }

    return files;
  }
}