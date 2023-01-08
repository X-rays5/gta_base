#pragma once
#include <string_view>
#include "range.hpp"

namespace gta_base::memory::scanner {
  class Module : public Range {
  public:
    explicit Module(HMODULE mod);
    explicit Module(std::nullptr_t);
    explicit Module(std::string_view name);
    explicit Module(std::wstring_view name);

    [[maybe_unused]] Handle get_export(std::string_view symbol_name);
  };
}