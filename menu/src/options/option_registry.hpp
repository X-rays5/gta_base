//
// Created by X-ray on 27/03/2026.
//

#pragma once
#include <ankerl/unordered_dense.h>
#include "base_option.hpp"
#include "options.hpp"

namespace base::menu::options {
  class OptionRegistry {
  public:
    OptionRegistry();
    ~OptionRegistry();

    void AddOption(std::shared_ptr<BaseOption> option) {
      common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
      opt_name_to_option_.emplace(option->GetName(), option);
    }

    std::shared_ptr<BaseOption> GetOption(const std::string& name) {
      common::concurrency::ScopedSpinlock lock(opt_registry_lock_);

      const auto it = opt_name_to_option_.find(name);
      if (it != opt_name_to_option_.end()) {
        return it->second;
      }

      return nullptr;
    }

    std::weak_ptr<AvailableOptions> options() {
      common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
      return available_options_;
    }

    std::vector<std::shared_ptr<BaseOption>> GetAllOptions() {
      common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
      std::vector<std::shared_ptr<BaseOption>> options;
      options.reserve(opt_name_to_option_.size());
      for (const auto& val : opt_name_to_option_ | std::views::values) {
        options.push_back(val);
      }
      return options;
    }

    Status SaveOption(BaseOption* opt);
    Status SaveOptions(const std::string& profile_name);
    Status LoadOptions(const std::string& profile_name);

  private:
    common::concurrency::Spinlock opt_registry_lock_;
    std::string active_profile_name_;
    ankerl::unordered_dense::map<std::string, std::shared_ptr<BaseOption>> opt_name_to_option_{};
    std::shared_ptr<AvailableOptions> available_options_;
  };
  inline OptionRegistry* kOPTION_REGISTRY{};
}


