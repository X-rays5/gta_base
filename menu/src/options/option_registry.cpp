//
// Created by X-ray on 27/03/2026.
//

#include "option_registry.hpp"
#include <base-common/fs/vfs.hpp>
#include <glaze/toml.hpp>
#include "../default.hpp"

namespace base::menu::options {
  namespace {
    std::filesystem::path GetSettingsFilePath(const std::string& profile_name) {
      return common::fs::vfs::GetOptionSettingsDir() / fmt::format("{}.json", profile_name);
    }

    StatusOr<glz::generic> ReadProfile(const std::string& profile_name) {
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

      return save;
    }

    Status WriteProfile(const std::string& profile_name, const glz::generic& data) {
      const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(data, GetSettingsFilePath(profile_name).string(), std::string{});
      if (ec) {
        LOG_ERROR("Failed to save options to file: {}", ec);
        return MakeFailure<ResultCode::kIO_ERROR>("Failed to save options to file: {}", ec);
      }

      return {};
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

  Status OptionRegistry::SaveOption(BaseOption* opt) {
    if (!opt) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Option pointer is null");
    }

    if (!opt->IsSavable()) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Option '{}' is not savable", opt->GetName());
    }

    LOG_INFO("Saving option '{}'", opt->GetName());

    common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
    auto res = ReadProfile(active_profile_name_);
    if (!res) {
      LOG_ERROR("Failed to read active options profile '{}': {}", active_profile_name_, res);
      return res.error().Forward();
    }

    std::string opt_saved;
    glz::generic opt_data;
    opt->Save(opt_data);
    auto ec = glz::write_toml(opt_data, opt_saved);
    if (ec) {
      LOG_ERROR("Failed to save options from file: {}", ec);
      return MakeFailure<ResultCode::kIO_ERROR>("Failed to save options from file: {}", ec);
    }

    glz::generic save = res.value();
    save[opt->GetName()] = opt_saved;

    return WriteProfile(active_profile_name_, save);
  }

  Status OptionRegistry::SaveOptions(const std::string& profile_name) {
    LOG_INFO("Saving options to profile '{}'", profile_name);

    glz::generic save;
    std::string glz_buff;

    common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
    for (auto&& opt : opt_name_to_option_) {
      LOG_INFO("Saving options from file: {}", opt.first);
      if (opt.second->IsSavable()) {
        glz::generic opt_data;
        opt.second->Save(opt_data);
        const auto ec = glz::write_toml(opt_data, glz_buff);
        if (ec) {
          LOG_ERROR("Failed to save options to file: {}", ec);
          continue;
        }

        save[opt.first] = glz_buff;
        LOG_DEBUG("Saving option '{}': {}", opt.first, save[opt.first].get_string());
      }
    }
    lock.Unlock();

    return WriteProfile(profile_name, save);
  }

  Status OptionRegistry::LoadOptions(const std::string& profile_name) {
    LOG_INFO("Loading options from profile '{}'", profile_name);

    auto res = ReadProfile(profile_name);
    if (!res) {
      LOG_ERROR("Failed to load options profile '{}': {}", profile_name, res);
      return res.error().Forward();
    }

    auto save = res.value();
    common::concurrency::ScopedSpinlock lock(opt_registry_lock_);
    for (auto&& opt : opt_name_to_option_) {
      if (opt.second->IsSavable() && save.contains(opt.first)) {
        opt.second->Load(save[opt.first]);
      }
    }

    active_profile_name_ = profile_name;
    return {};
  }
}
