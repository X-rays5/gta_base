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

    /// Register `option` under its own name, answering whether it was registered.
    ///
    /// A name that is already taken is what makes that false, and the option that is there stays: the
    /// built-in options are the menu's own and the code that made them still holds them, so
    /// overwriting one from a script would take it out of the registry while everything else went on
    /// using it. The failure is reported here as well as returned, because the caller that ignores it
    /// is the caller this would otherwise be silent for.
    bool AddOption(std::shared_ptr<BaseOption> option) {
      if (!option) {
        return false;
      }

      const std::string name = option->GetName();

      // Logged after the lock is dropped: the message is the registry's business, but holding a
      // spinlock across an allocation is not.
      bool added = false;
      {
        common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
        added = opt_name_to_option_.try_emplace(name, option).second;
      }

      if (!added) {
        LOG_ERROR("[Options] An option named '{}' is already registered, so this one is not: {}",
                  name, option->GetOwnerScript().empty() ? "it was registered by the menu" : "it was registered by the script " + option->GetOwnerScript());
      }

      return added;
    }

    /// Take the option registered under `name` out of the registry, answering whether one was there.
    ///
    /// Whatever else holds the option keeps it alive; this only means it can no longer be found by
    /// name, which is what unregistering is.
    bool RemoveOption(const std::string& name) {
      common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
      return opt_name_to_option_.erase(name) > 0;
    }

    /// Take out every option that `script_name` registered, which is what a script that unloads has
    /// to leave behind: the registry holds the options it registered strongly, so nothing else would
    /// take them out, and an option whose script is gone has nothing left to run.
    ///
    /// A script's name is asked of the options rather than tracked here, so a script that never
    /// registered anything is not a case this has to know about, and registering under a name the
    /// menu already uses changes nothing about who owns it.
    void RemoveOptionsOwnedBy(const std::string& script_name) {
      std::size_t removed = 0;
      {
        common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
        for (auto it = opt_name_to_option_.begin(); it != opt_name_to_option_.end();) {
          if (it->second && it->second->GetOwnerScript() == script_name) {
            it = opt_name_to_option_.erase(it);
            ++removed;
          } else {
            ++it;
          }
        }
      }

      if (removed > 0) {
        LOG_INFO("[Options] {} option(s) registered by the script '{}' have been unregistered with it", removed, script_name);
      }
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


