#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace base::tools::native_gen::util {
  // Mirrors Python str.strip(), which strips ' ', '\t', '\n', '\r', '\v' and '\f'.
  [[nodiscard]] std::string Strip(std::string_view value);

  [[nodiscard]] std::string ReplaceAll(std::string_view value, std::string_view from, std::string_view to);

  [[nodiscard]] std::string ToLowerAscii(std::string_view value);

  // Mirrors the Python generator's snake_case -> PascalCase conversion, which is
  // str.capitalize() applied per '_'-separated component: first character upper,
  // remainder lower. "APP_SET_APP" therefore becomes "AppSetApp".
  [[nodiscard]] std::string PascalCase(std::string_view value);

  // The generator's only type normalisation: every occurrence of "BOOL" becomes "bool".
  [[nodiscard]] std::string NormalizeType(std::string_view type);

  // Mirrors the dumper's TYPE_MAPPING table plus its pointer and const fallbacks,
  // including branch order and the case-sensitive "char" substring test.
  [[nodiscard]] std::string MapTypeToPrimitive(std::string_view type);

  // Reads `path` verbatim. Returns an error message on failure.
  [[nodiscard]] std::optional<std::string> ReadFileText(const std::filesystem::path& path, std::string& out);

  // Writes `content` (built with '\n') to `path`, translating line endings to CRLF when
  // `crlf` is set. Creates parent directories. Returns an error message on failure.
  [[nodiscard]] std::optional<std::string> WriteTextFile(const std::filesystem::path& path,
                                                         std::string_view content,
                                                         bool crlf,
                                                         bool atomic);
}
