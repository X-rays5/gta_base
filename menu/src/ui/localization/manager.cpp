//
// Created by X-ray on 29/12/2023.
//

#include "manager.hpp"
#include <glaze/glaze.hpp>
#include "../../util/vfs.hpp"

namespace base::ui::localization {
    Status Translation::Load() {
        std::string json_buffer;
        const auto ec = glz::read_file_json(loaded_translation_, (util::vfs::GetTranslationDir() / "default.json").string(), json_buffer);
        if (ec) {
            return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, json_buffer));
        }

        return {};
    }

    Status Translation::Save() {
        const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(loaded_translation_, (util::vfs::GetTranslationDir() / "default.json").string(), std::string{});
        if (ec) {
            return MakeFailure<ResultCode::kIO_ERROR>(magic_enum::enum_name(ec.ec).data());
        }

        return {};
    }

    void Translation::Merge() {}

    Manager::Manager() {
        kMANAGER = this;
    }

    Manager::~Manager() {
        kMANAGER = nullptr;
    }
}
