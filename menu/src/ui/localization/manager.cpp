//
// Created by X-ray on 29/12/2023.
//

#include "manager.hpp"
#include <glaze/glaze.hpp>
#include "../../../../common/src/vfs.hpp"

namespace base::menu::ui::localization {
    namespace {
        std::string GetProfilePath(const std::string& name) {
            return fmt::format("{}/{}.json", util::vfs::GetTranslationDir(), name);
        }
    }

    Status Translation::Load(const std::string& name) {
        std::string json_buffer;
        translation_map_t tmp_translation;

        std::string profile_path = GetProfilePath(name);
        if (!std::filesystem::is_regular_file(profile_path)) {
            LOG_INFO("Requested translation file '{}' doesn't exist, loading default translation", name);
            WriteDefaultTranslation();
            profile_path = GetProfilePath("default");
        }

        const auto ec = glz::read_file_json(tmp_translation, profile_path, json_buffer);
        if (ec) {
            return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, json_buffer));
        }

        auto res = Merge(tmp_translation);
        if (res.error()) {
            LOG_ERROR(res);
        }

        return res;
    }

    Status Translation::Save(const std::string& name) {
        const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(loaded_translation_, GetProfilePath(name), std::string{});
        if (ec) {
            return MakeFailure<ResultCode::kIO_ERROR>(magic_enum::enum_name(ec.ec).data());
        }

        return {};
    }

    Status Translation::Merge(translation_map_t tmp_translation) {
        auto merged = glz::merge{loaded_translation_, tmp_translation};

        std::string tmp_buff;
        auto ec = glz::write_json(merged, tmp_buff);
        if (ec) {
            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to merge default and loaded translation files");
        }

        ec = glz::read_json(loaded_translation_, tmp_buff);
        if (ec) {
            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to merge default and loaded translation files");
        }

        return {};
    }

    void Translation::WriteDefaultTranslation() {
        const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(default_translation, GetProfilePath("default"), std::string{});
        if (ec) {
            LOG_ERROR("Failed to write default translation to disk: {}", magic_enum::enum_name(ec.ec));
        }
    }

    Manager::Manager() {
        auto status = translation_.Load("default");
        if (status.error()) {
            LOG_CRITICAL("Failed to load translation from disk: {}", status.error());
        }

        kMANAGER = this;
    }

    Manager::~Manager() {
        kMANAGER = nullptr;

        auto status = translation_.Save(active_translation_);
        if (status.error()) {
            LOG_ERROR("Failed to save active translation to disk: {}", status.error());
        }
    }

    std::string Manager::Localize(const char* key) {
        const auto it = translation_.loaded_translation_.find(key);

        return (it != translation_.loaded_translation_.end()) ? it->second : fmt::format("NOT_FOUND: {}", key);
    }

    Status Manager::SetActiveTranslation(const std::string& name, bool save_current) {
        if (save_current && translation_.Save(name).error()) {
            LOG_ERROR("Failed to save current translation");

            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Unable to save current translation file");
        }

        Status res = translation_.Load(name);
        if (res.error()) {
            LOG_ERROR("Failed to load translation '{}': {}", name, res.error());
            return res;
        }

        active_translation_ = name;
        return res;
    }
}
