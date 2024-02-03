//
// Created by X-ray on 29/12/2023.
//

#include "manager.hpp"
#include <glaze/glaze.hpp>
#include "../../util/vfs.hpp"

namespace base::ui::localization {
    Status Translation::Load() {
        std::string json_buffer;
        translation_map_t tmp_translation;
        const auto ec = glz::read_file_json(tmp_translation, (util::vfs::GetTranslationDir() / "default.json").string(), json_buffer);
        if (ec) {
            return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, json_buffer));
        }

        auto res = Merge(tmp_translation);
        if (res.error()) {
            LOG_ERROR(res);
        }

        return res;
    }

    Status Translation::Save() {
        const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(loaded_translation_, (util::vfs::GetTranslationDir() / "default.json").string(), std::string{});
        if (ec) {
            return MakeFailure<ResultCode::kIO_ERROR>(magic_enum::enum_name(ec.ec).data());
        }

        return {};
    }

    Status Translation::Merge(translation_map_t tmp_translation) {
        auto merged = glz::merge{loaded_translation_, tmp_translation};

        std::string tmp_buff;
        glz::write_json(merged, tmp_buff);
        const auto ec = glz::read_json(loaded_translation_, tmp_buff);
        if (ec) {
            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to merge default and loaded translation files");
        }

        return {};
    }

    Manager::Manager() {
        auto status = translation_.Load();
        if (status.error()) {
            LOG_FATAL("Failed to load translation from disk: {}", status.error());
        }

        kMANAGER = this;
    }

    Manager::~Manager() {
        kMANAGER = nullptr;

        auto status = translation_.Save();
        if (status.error()) {
            LOG_ERROR("Failed to save active translation to disk: {}", status.error());
        }
    }
}
