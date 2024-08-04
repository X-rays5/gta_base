//
// Created by X-ray on 04/08/2024.
//

#ifndef STRING_HPP_08145245
#define STRING_HPP_08145245
#include <string>

namespace base::util::string {
  /**
   * Replace all occurrences of targetReplace with replaceWith in str
   * @param str String to replace in
   * @param target_replace Str to match and replace
   * @param replace_val Str to replace matches with
   * @return New string with all occurrences of targetReplace replaced with replaceWith
   */
  std::string ReplaceAll(std::string str, const std::string& target_replace, const std::string& replace_val);
}

#endif //STRING_HPP_08145245
