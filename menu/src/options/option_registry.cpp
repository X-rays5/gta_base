//
// Created by X-ray on 27/03/2026.
//

#include "option_registry.hpp"
#include <base-common/fs/vfs.hpp>
#include "../default.hpp"

namespace base::menu::options {
  namespace {
    std::filesystem::path GetSettingsFilePath(const std::string& profile_name) {
      return common::fs::vfs::GetOptionSettingsDir() / fmt::format("{}.json", profile_name);
    }
  }

  OptionRegistry::OptionRegistry() {
    auto status = LoadOptions(kDEFAULT_PROFILES->GetDefaultOptSettings());

    kOPTION_REGISTRY = this;

    available_options_ = std::make_shared<AvailableOptions>();
    LOG_INFO("Loaded default options profile '{}': {}. {} options are ready for use", kDEFAULT_PROFILES->GetDefaultOptSettings(), status, opt_name_to_option_.size());
  }

  OptionRegistry::~OptionRegistry() {
    kOPTION_REGISTRY = nullptr;
  }

  Status OptionRegistry::SaveOptions(const std::string& profile_name) {
    LOG_INFO("Saving options to profile '{}'", profile_name);

    glz::generic save;

    common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
    for (auto&& opt : opt_name_to_option_) {
      LOG_INFO("Saving options from file: {}", opt.first);
      if (opt.second->IsSavable()) {
        save[opt.first] = opt.second->Save();
        LOG_DEBUG("Saving option '{}': {}", opt.first, save[opt.first].get_string());
      }
    }
    lock.Unlock();

    const auto ec = glz::write_file_json(save, GetSettingsFilePath(profile_name).string(), std::string{});
    if (ec) {
      LOG_ERROR("Failed to save options to file: {}", ec);
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to save options to file: {}", ec);
    }

    return {};
  }

  Status OptionRegistry::LoadOptions(const std::string& profile_name) {
    LOG_INFO("Loading options from profile '{}'", profile_name);

    const auto path = GetSettingsFilePath(profile_name);
    if (!std::filesystem::exists(path)) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Options profile not found: {}", profile_name);
    }

    glz::generic save;
    const auto ec = glz::read_file_json(save, path.string(), std::string{});
    if (ec) {
      LOG_ERROR("Failed to load options from file: {}", ec);
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to load options from file: {}", ec);
    }

    common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
    for (auto&& opt : opt_name_to_option_) {
      if (opt.second->IsSavable() && save.contains(opt.first)) {
        opt.second->Load(save[opt.first].get_string());
      }
    }

    return {};
  }
}
