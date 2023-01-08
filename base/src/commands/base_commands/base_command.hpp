//
// Created by X-ray on 01/08/23.
//

#pragma once
#ifndef GTA_BASE_BASE_COMMAND_0C15A5CE2F4F4F498EC0FC1BCA49E4B2_HPP
#define GTA_BASE_BASE_COMMAND_0C15A5CE2F4F4F498EC0FC1BCA49E4B2_HPP
#include <string>
#include "../../rage/joaat.hpp"
#include "../../ui/translation/translation_manager.hpp"

namespace gta_base::commands::base_commands {
  class Base {
  public:
    Base(const std::string& id, const std::string& name_key, const std::string& description_key, std::size_t num_args = 0, bool fiber_pool = false);

    [[nodiscard]] FORCE_INLINE rage::joaat_t GetId() const {
      return id_;
    }

    [[nodiscard]] FORCE_INLINE std::string GetName() {
      return ui::kTRANSLATION_MANAGER->Get(name_key_);
    }

    [[nodiscard]] FORCE_INLINE std::string GetNameKey() {
      return name_key_;
    }

    [[nodiscard]] FORCE_INLINE std::string GetDescription() {
      return ui::kTRANSLATION_MANAGER->Get(description_key_);
    }

    [[nodiscard]] FORCE_INLINE std::string GetDescriptionKey() {
      return description_key_;
    }

  protected:
    rage::joaat_t id_;
    std::string name_key_;
    std::string description_key_;
    std::size_t num_args_;
    bool fiber_pool_;
  };
}
#endif //GTA_BASE_BASE_COMMAND_0C15A5CE2F4F4F498EC0FC1BCA49E4B2_HPP
