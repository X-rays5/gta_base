#pragma once
#include <optional>
#include <string_view>
#include <vector>
#include "fwdec.hpp"
#include "handle.hpp"

namespace base::memory::scanner {
  class Pattern {
    friend Batch;
    friend Range;
    public:
    explicit Pattern(std::string_view ida_sig);
    explicit Pattern(const void* bytes, std::string_view mask);

    inline explicit Pattern(const std::string& ida_sig) : Pattern(ida_sig.c_str()) {}

    inline explicit Pattern(char* ida_sig) : Pattern(std::string_view(ida_sig)) {}

    inline explicit Pattern(const char* ida_sig) : Pattern(std::string_view(ida_sig)) {}

    public:
    std::vector<std::optional<std::uint8_t>> bytes_;
  };
}