//
// Created by X-ray on 11/01/2024.
//

#ifndef RESULT_HPP_01224051
#define RESULT_HPP_01224051
#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>
#include <result/result.hpp>

namespace base::common::util::result {
  enum class ResultCode {
    kSUCCESS,
    kINVALID_ARGUMENT,
    kINVALID_HANDLE,
    kNOT_FOUND,
    kIO_ERROR,
    kINTERNAL_ERROR,
    kALLOCATION_ERROR
  };

  class StatusErr {
  public:
    StatusErr() :
      result_code_(ResultCode::kSUCCESS) {}

    StatusErr(const ResultCode code, std::string msg = "") :
      result_code_(code), msg_(std::move(msg)) {}

    /**
     * \brief Check if the status is ok
     * \return True if the status is ok, false otherwise
     */
    [[nodiscard]] bool Ok() const {
      return result_code_ == ResultCode::kSUCCESS;
    }

    /**
     * \brief Get the result code
     * \return The result code
     */
    [[nodiscard]] ResultCode GetResult() const {
      return result_code_;
    }

    /**
     * \brief Get the result code as a string
     * \return The result code as a string
     */
    [[nodiscard]] std::string GetResultString() const {
      return std::string(magic_enum::enum_name(result_code_));
    }

    /**
     * \brief Get the result message
     * \return The result message
     */
    [[nodiscard]] std::string GetResultMessage() const {
      return msg_;
    }

    /**
     * \brief Forward the error
     * @return The error
     */
    [[nodiscard]] cpp::failure<StatusErr> Forward() const {
      return cpp::fail(*this);
    }

    operator bool() const {
      return !Ok();
    }

  private:
    const ResultCode result_code_;
    const std::string msg_;
  };

  using Status = cpp::result<void, StatusErr>;
  template <typename T>
  using StatusOr = cpp::result<T, StatusErr>;

  /**
   * \brief Make a failure status
   * \param format The result message
   * \param format_args arguments to format message
   * \return An unexpected status
   */
  template <ResultCode code, typename... Args>
  cpp::failure<StatusErr> MakeFailure(const std::string& format = "", Args&&... format_args) {
    std::string message;
    if constexpr (sizeof...(format_args) > 0) {
      // Create lvalue copies to satisfy fmt::make_format_args
      auto args_tuple = std::make_tuple(std::decay_t<Args>(std::forward<Args>(format_args))...);

      message = std::apply([format](const auto&... args) {
        return fmt::vformat(format, fmt::make_format_args(args...));
      }, args_tuple);
    } else {
      message = format;
    }
    return cpp::fail(StatusErr(code, std::move(message)));
  }

  /**
   * Make a success status
   * @return a cpp::failure instance containing kSUCCESS
   */
  [[nodiscard]] inline cpp::failure<StatusErr> MakeSuccess() {
    return MakeFailure<ResultCode::kSUCCESS>();
  }
}

namespace base {
  using common::util::result::ResultCode;
  using common::util::result::Status;
  using common::util::result::StatusOr;
  using common::util::result::MakeFailure;
}

template <>
struct fmt::formatter<base::common::util::result::StatusErr> : formatter<std::string> {
  template <typename FormatContext>
  auto format(const base::common::util::result::StatusErr& status, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}: {}", status.GetResultString(), status.GetResultMessage());
  }
};

template <typename SuccessType, typename ErrorType>
struct fmt::formatter<cpp::result<SuccessType, ErrorType>> : formatter<std::string> {
  template <typename FormatContext>
  auto format(const cpp::result<SuccessType, ErrorType>& result, FormatContext& ctx) const {
    if (result.has_value()) {
      return fmt::format_to(ctx.out(), "[SUCCESS]");
    }

    return fmt::format_to(ctx.out(), "[FAILURE]: {}", result.error());
  }
};

#endif //RESULT_HPP_01224051
