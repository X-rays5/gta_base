#pragma once

#include <cstddef>
#include <expected>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace base::tools::native_gen::db {
  struct NativeParam {
    std::string type;
    std::string name;
  };

  struct Native {
    std::string hash;
    std::string name;
    std::string comment;
    std::string return_type;
    std::vector<NativeParam> params;
  };

  struct NativeGroup {
    std::string name;
    std::vector<Native> natives;
  };

  // Groups and, within a group, natives stay in JSON document order. That order is
  // significant: the committed generated files depend on it.
  using NativeDb = std::vector<NativeGroup>;

  struct Crossmap {
    std::vector<std::pair<std::string, std::string>> entries;
    std::unordered_map<std::string, std::size_t> hash_to_index;

    [[nodiscard]] bool empty() const { return entries.empty(); }

    // Mirrors the generator's `crossmap and native_hash in hash_to_index` test.
    [[nodiscard]] bool Has(const std::string& hash) const {
      return !entries.empty() && hash_to_index.contains(hash);
    }

    [[nodiscard]] std::size_t IndexOf(const std::string& hash) const { return hash_to_index.at(hash); }
  };

  [[nodiscard]] std::expected<NativeDb, std::string> ParseNativeDb(std::string_view json);

  // Blank lines and '#' comments are skipped; a line needs at least two comma separated
  // fields or it is ignored. Both hashes map to the line's index, later lines winning.
  // Malformed input is simply skipped, so this cannot fail.
  [[nodiscard]] Crossmap ParseCrossmap(std::string_view text);
}
