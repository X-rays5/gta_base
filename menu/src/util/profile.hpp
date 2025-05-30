//
// Created by X-ray on 04/08/2024.
//

#ifndef PROFILE_HPP_08034048
#define PROFILE_HPP_08034048

#include <filesystem>
#include <base-common/fs/vfs.hpp>
#include <base-common/util/time.hpp>
#include <glaze/trace/trace.hpp>

#ifndef NDEBUG
#define PROFILER_SAVE(profiler) if (const Status _prof_res = profiler.WriteToDisk(::base::common::util::string::ReplaceAll(__FUNCTION__, "::", "_")); _prof_res.error()) { LOG_ERROR("{}", _prof_res.error()); }
#else
#define PROFILER_SAVE(profiler) (void)0
#endif

namespace base::menu::util {
  class Profile {
  public:
    void Disabled(const bool is_disabled) {
      trace_.disabled = is_disabled;
    }

    void Begin(const std::string& name) {
      trace_.begin(name);
    }

    void End(const std::string& name) {
      trace_.end(name);
    }

    [[nodiscard]] Status WriteToDisk(const std::string& folder) {
      if (!trace_.disabled) {
        const auto target_folder = common::fs::vfs::GetTimeTraceDir() / folder;
        const auto target_file = target_folder / fmt::format("{}.trace", common::util::time::GetTimeStamp());
        try {
          std::filesystem::create_directories(target_folder);
        } catch (std::filesystem::filesystem_error& ex) {
          LOG_ERROR("{}", ex.what());
        }

        if (const auto ec = glz::write_file_json(trace_, target_file.string(), std::string{}); ec.ec != glz::error_code::none) {
          auto error_name = magic_enum::enum_name(ec.ec);
          return MakeFailure<ResultCode::kIO_ERROR>("Error writing trace file: {}", error_name);
        }
      }

      return {};
    }

  private:
    glz::trace trace_{};
  };
}

#endif //PROFILE_HPP_08034048
