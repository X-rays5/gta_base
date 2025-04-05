//
// Created by X-ray on 09/04/2024.
//

#ifndef PATTERN_HPP
#define PATTERN_HPP
#include <base-common/memory/address.hpp>

namespace base::menu::memory::signature {
  class Pattern {
  public:
    explicit Pattern(const std::string& signature, const std::string& module_name = "") : signature_(signature), module_name_(module_name) {}

    StatusOr<common::memory::Address> Scan();

    std::string GetModule() {
      return module_name_;
    }

  private:
    std::string signature_;
    std::string module_name_;
  };
}
#endif //PATTERN_HPP
