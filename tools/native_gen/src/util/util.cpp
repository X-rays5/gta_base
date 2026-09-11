#include "util.hpp"

#include <cstdio>
#include <system_error>
#include <unordered_map>

namespace base::tools::native_gen::util {
  namespace {
    constexpr bool IsPySpace(const char c) {
      return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
    }

    constexpr char ToUpperAscii(const char c) { return c >= 'a' && c <= 'z' ? static_cast<char>(c - 32) : c; }

    constexpr char ToLowerAsciiChar(const char c) { return c >= 'A' && c <= 'Z' ? static_cast<char>(c + 32) : c; }

    [[nodiscard]] std::optional<std::string> WriteRaw(const std::filesystem::path& path, std::string_view bytes) {
      std::error_code ec;
      if (const auto parent = path.parent_path(); !parent.empty()) {
        std::filesystem::create_directories(parent, ec);
        if (ec) {
          return "cannot create directory " + parent.string() + ": " + ec.message();
        }
      }

      std::FILE* file = std::fopen(path.string().c_str(), "wb");
      if (file == nullptr) {
        return "cannot open " + path.string() + " for writing";
      }

      const auto written = std::fwrite(bytes.data(), 1, bytes.size(), file);
      const auto close_err = std::fclose(file);
      if (written != bytes.size()) {
        return "short write to " + path.string();
      }
      if (close_err != 0) {
        return "cannot flush " + path.string();
      }
      return std::nullopt;
    }
  }

  std::optional<std::string> ReadFileText(const std::filesystem::path& path, std::string& out) {
    std::FILE* file = std::fopen(path.string().c_str(), "rb");
    if (file == nullptr) {
      return "cannot open " + path.string() + " for reading";
    }

    out.clear();
    char buffer[65536];
    while (true) {
      const size_t read = std::fread(buffer, 1, sizeof(buffer), file);
      out.append(buffer, read);
      if (read < sizeof(buffer)) {
        break;
      }
    }

    const bool failed = std::ferror(file) != 0;
    std::fclose(file);
    return failed ? std::optional<std::string>{"read error in " + path.string()} : std::nullopt;
  }

  std::string Strip(std::string_view value) {
    size_t begin = 0;
    size_t end = value.size();
    while (begin < end && IsPySpace(value[begin])) {
      ++begin;
    }
    while (end > begin && IsPySpace(value[end - 1])) {
      --end;
    }
    return std::string(value.substr(begin, end - begin));
  }

  std::string ReplaceAll(std::string_view value, std::string_view from, std::string_view to) {
    if (from.empty()) {
      return std::string(value);
    }

    std::string result;
    result.reserve(value.size());
    size_t position = 0;
    while (true) {
      const auto found = value.find(from, position);
      if (found == std::string_view::npos) {
        result.append(value.substr(position));
        return result;
      }
      result.append(value.substr(position, found - position));
      result.append(to);
      position = found + from.size();
    }
  }

  std::string ToLowerAscii(std::string_view value) {
    std::string result(value);
    for (char& c : result) {
      c = ToLowerAsciiChar(c);
    }
    return result;
  }

  std::string PascalCase(std::string_view value) {
    std::string result;
    size_t position = 0;
    while (true) {
      const auto found = value.find('_', position);
      const auto component = found == std::string_view::npos
                               ? value.substr(position)
                               : value.substr(position, found - position);
      if (!component.empty()) {
        result.push_back(ToUpperAscii(component.front()));
        for (size_t i = 1; i < component.size(); ++i) {
          result.push_back(ToLowerAsciiChar(component[i]));
        }
      }
      if (found == std::string_view::npos) {
        return result;
      }
      position = found + 1;
    }
  }

  std::string NormalizeType(std::string_view type) { return ReplaceAll(type, "BOOL", "bool"); }

  std::string MapTypeToPrimitive(std::string_view type) {
    static const std::unordered_map<std::string_view, std::string_view> kTypeMapping{
        {"bool", "bool"},
        {"int", "int"},
        {"float", "float"},
        {"void", "void"},
        {"char", "char"},
        {"unsigned", "uint32_t"},
        {"uint", "uint32_t"},
        {"uint64_t", "uint64_t"},
        {"Ped", "int"},
        {"Entity", "int"},
        {"Vehicle", "int"},
        {"Hash", "uint32_t"},
        {"Any", "int32_t"},
        {"const char*", "const char*"},
        {"char*", "char*"},
        {"void*", "uint64_t*"},
    };

    const std::string trimmed = Strip(type);
    if (const auto it = kTypeMapping.find(trimmed); it != kTypeMapping.end()) {
      return std::string(it->second);
    }

    if (!trimmed.empty() && trimmed.back() == '*') {
      const std::string base = Strip(std::string_view(trimmed).substr(0, trimmed.size() - 1));
      return base.find("char") != std::string::npos ? "const char*" : "uint64_t*";
    }

    if (trimmed.starts_with("const ")) {
      const std::string inner = Strip(std::string_view(trimmed).substr(6));
      if (!inner.empty() && inner.back() == '*') {
        const std::string base = Strip(std::string_view(inner).substr(0, inner.size() - 1));
        return base.find("char") != std::string::npos ? "const char*" : "uint64_t*";
      }
      const std::string mapped = MapTypeToPrimitive(inner);
      return mapped.starts_with("const") ? mapped : "const " + mapped;
    }

    return "int";
  }

  std::optional<std::string> WriteTextFile(const std::filesystem::path& path,
                                           std::string_view content,
                                           const bool crlf,
                                           const bool atomic) {
    std::string bytes;
    if (crlf) {
      bytes.reserve(content.size() + content.size() / 16);
      for (const char c : content) {
        if (c == '\n') {
          bytes += "\r\n";
        } else {
          bytes.push_back(c);
        }
      }
    } else {
      bytes.assign(content);
    }

    if (!atomic) {
      return WriteRaw(path, bytes);
    }

    auto temp = path;
    temp += ".tmp";
    if (auto error = WriteRaw(temp, bytes)) {
      return error;
    }

    std::error_code ec;
    std::filesystem::rename(temp, path, ec);
    if (ec) {
      std::filesystem::remove(temp, ec);
      return "cannot replace " + path.string();
    }
    return std::nullopt;
  }
}
