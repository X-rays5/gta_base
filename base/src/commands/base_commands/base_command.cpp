//
// Created by X-ray on 01/08/23.
//

#include "base_command.hpp"
#include "../command_manager.hpp"

namespace gta_base::commands::base_commands {
  Base::Base(const std::string& id, const std::string& name_key, const std::string& description_key, std::size_t num_args, bool fiber_pool) {
    id_ = rage::joaat(id);
    name_key_ = name_key;
    description_key_ = description_key;
    num_args_ = num_args;
    fiber_pool_ = fiber_pool;
    kCOMMAND_MANAGER.RegisterCommand(this);
  }
}
