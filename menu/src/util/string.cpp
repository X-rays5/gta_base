//
// Created by X-ray on 04/08/2024.
//

#include "string.hpp"

namespace base::util::string {
  std::string ReplaceAll(std::string str, const std::string& target_replace, const std::string& replace_val) {
    size_t pos = 0;
    while ((pos = str.find(target_replace, pos)) != std::string::npos) {
      str.replace(pos, target_replace.length(), replace_val);
      pos += replace_val.length();
    }

    return str;
  }
}
