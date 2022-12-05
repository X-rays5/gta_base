#include "pattern.hpp"

namespace gta_base::memory::scanner {
  Pattern::Pattern(std::string_view ida_sig) {

    for (std::size_t i = 0; i < ida_sig.size(); ++i) {
      if (ida_sig[i] == ' ')
        continue;
      bool last = (i == ida_sig.size() - 1);
      if (ida_sig[i] != '?') {
        if (!last) {
          auto c1 = common::HexToInt(ida_sig[i]);
          auto c2 = common::HexToInt(ida_sig[i + 1]);
          if (c1 && c2) {
            bytes_.emplace_back(static_cast<std::uint8_t>((*c1 * 0x10) + *c2));
          }
        }
      } else {
        bytes_.emplace_back(std::nullopt);
      }
    }
  }

  Pattern::Pattern(const void *bytes, std::string_view mask) {
    for (std::size_t i = 0; i < mask.size(); ++i) {
      if (mask[i] != '?')
        bytes_.emplace_back(static_cast<const std::uint8_t*>(bytes)[i]);
      else
        bytes_.emplace_back(std::nullopt);
    }
  }
}