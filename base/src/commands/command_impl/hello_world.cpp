//
// Created by X-ray on 01/13/23.
//

#include "../base_commands/exec_command.hpp"

namespace gta_base::commands::impl {
  [[maybe_unused]] base_commands::Exec kHELLO_WORLD_COMMAND("hello_world", "Hello, World!", "Print Hello, World! to the console", []() {
    LOG_INFO("Hello, World!");
  });
}