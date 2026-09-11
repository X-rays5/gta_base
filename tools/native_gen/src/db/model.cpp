#include "model.hpp"

#include <optional>

#include <glaze/glaze.hpp>

#include "../util/util.hpp"

namespace base::tools::native_gen::db {
  namespace {
    // The database drives everything from these four keys; anything else is ignored.
    std::string JsonString(const auto& object, const std::string_view key, const std::string_view fallback) {
      const auto it = object.find(key);
      if (it == object.end() || !it->second.is_string()) {
        return std::string(fallback);
      }
      return std::string(it->second.get_string());
    }

    // Python's str.split(','). The generator keeps empty fields, so this does too.
    std::vector<std::string> SplitCommas(const std::string& line) {
      std::vector<std::string> parts;
      std::size_t position = 0;
      while (true) {
        const auto comma = line.find(',', position);
        if (comma == std::string::npos) {
          parts.push_back(line.substr(position));
          return parts;
        }
        parts.push_back(line.substr(position, comma - position));
        position = comma + 1;
      }
    }
  }

  std::expected<NativeDb, std::string> ParseNativeDb(const std::string_view json) {
    glz::generic document{};
    if (const auto error = glz::read_json(document, json)) {
      return std::unexpected("cannot parse the native database: " + glz::format_error(error, json));
    }

    if (!document.is_object()) {
      return std::unexpected("cannot parse the native database: expected an object at the root");
    }

    NativeDb database;
    for (const auto& [group_name, group_value] : document.get_object()) {
      if (!group_value.is_object()) {
        continue;
      }

      NativeGroup group;
      group.name = std::string(group_name);
      for (const auto& [hash, native_value] : group_value.get_object()) {
        if (!native_value.is_object()) {
          continue;
        }

        const auto& object = native_value.get_object();
        Native native;
        native.hash = std::string(hash);
        native.name = JsonString(object, "name", "UNKNOWN");
        native.comment = JsonString(object, "comment", "");
        native.return_type = JsonString(object, "return_type", "void");

        if (const auto params = object.find("params"); params != object.end() && params->second.is_array()) {
          for (const auto& param : params->second.get_array()) {
            if (!param.is_object()) {
              continue;
            }
            const auto& fields = param.get_object();
            native.params.push_back(NativeParam{
                .type = JsonString(fields, "type", ""),
                .name = JsonString(fields, "name", ""),
            });
          }
        }

        group.natives.push_back(std::move(native));
      }

      database.push_back(std::move(group));
    }

    return database;
  }

  Crossmap ParseCrossmap(const std::string_view text) {
    Crossmap crossmap;

    std::size_t start = 0;
    while (start <= text.size()) {
      const auto newline = text.find('\n', start);
      const auto end = newline == std::string_view::npos ? text.size() : newline;
      const std::string line = util::Strip(text.substr(start, end - start));

      if (!line.empty() && !line.starts_with('#')) {
        const auto parts = SplitCommas(line);
        if (parts.size() >= 2) {
          const std::string hash1 = util::Strip(parts[0]);
          const std::string hash2 = util::Strip(parts[1]);
          const std::size_t index = crossmap.entries.size();
          crossmap.entries.emplace_back(hash1, hash2);
          crossmap.hash_to_index[hash1] = index;
          crossmap.hash_to_index[hash2] = index;
        }
      }

      if (newline == std::string_view::npos) {
        break;
      }
      start = newline + 1;
    }

    return crossmap;
  }
}
