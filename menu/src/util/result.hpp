//
// Created by X-ray on 11/01/2024.
//

#ifndef RESULT_HPP_01224051
#define RESULT_HPP_01224051
#include <expected>
#include <magic_enum.hpp>

namespace base::util::result {
  enum class ResultCode {
    kSUCCESS,
    kINVALID_ARGUMENT,
    kINVALID_HANDLE,
    kNOT_FOUND,
    kIO_ERROR,
    kINTERNAL_ERROR
  };

  class StatusErr {
  public:
    StatusErr() : result_code_(ResultCode::kSUCCESS) {}
    StatusErr(const ResultCode code, const std::string& msg = "") : result_code_(code), msg_(msg) {}

    [[nodiscard]] bool Ok() const {
      return result_code_ == ResultCode::kSUCCESS;
    }

    [[nodiscard]] ResultCode GetResult() const {
      return result_code_;
    }

    [[nodiscard]] std::string GetResultString() const {
      return magic_enum::enum_name(result_code_).data();
    }

    [[nodiscard]] std::string GetResultMessage() const {
      return msg_;
    }

    [[nodiscard]] std::unexpected<StatusErr> Forward() {
      return std::unexpected(*this);
    }

    operator bool() const {
      return !Ok();
    }

  private:
    const ResultCode result_code_;
    const std::string msg_;
  };

  using Status = std::expected<void, StatusErr>;
  template <typename T>
  using StatusOr = std::expected<T, StatusErr>;

  template <ResultCode code>
  std::unexpected<StatusErr> MakeFailure(const std::string& msg = "") {
    return std::unexpected(StatusErr(code, msg));
  }
}

namespace base {
  using util::result::ResultCode;
  using util::result::Status;
  using util::result::StatusOr;
  using util::result::MakeFailure;
}

template <typename T>
struct fmt::formatter<base::StatusOr<T>> {
  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename Context>
  auto format(const base::StatusOr<T>& status, Context& ctx) {
    return fmt::format_to(ctx.out(), "{} \"{}\")", status.error().GetResultString(), status.error().GetResultMessage());
  }
};
#endif //RESULT_HPP_01224051
