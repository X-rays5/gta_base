//
// Created by X-ray on 01/13/23.
//

#pragma once
#ifndef GTA_BASE_EXEC_COMMAND_1ADC0F7524ED4F3887BB9E25B4CACC4C_HPP
#define GTA_BASE_EXEC_COMMAND_1ADC0F7524ED4F3887BB9E25B4CACC4C_HPP
#include <utility>
#include "base_command.hpp"

namespace gta_base::commands::base_commands {
  class Exec : public Base {
  public:
    Exec(const std::string& id, const std::string& name_key, const std::string& description_key, std::function<void()> fn, std::size_t num_args = 0, bool fiber_pool = false) :
    Base(id, name_key, description_key, num_args, fiber_pool), fn_(std::move(fn)) {}

  private:
    std::function<void()> fn_;

  protected:
    void Execute(std::vector<std::string> args) final {
      if (fiber_pool_) {
        fiber::kPOOL->AddJob(fn_);
      } else {
        fn_();
      }
    }
  };
}
#endif //GTA_BASE_EXEC_COMMAND_1ADC0F7524ED4F3887BB9E25B4CACC4C_HPP
