#pragma once
#include <vector>
#include "pattern.hpp"
#include "handle.hpp"

namespace base::memory::scanner {
  class Range {
  public:
    Handle begin();
    Handle end();
    std::size_t size();

    bool contains(Handle h);

    absl::StatusOr<Handle> scan(Pattern const& sig);
    std::vector<Handle> scan_all(Pattern const& sig);
  protected:
    Handle base_;
    std::size_t size_;

  protected:
    Range(Handle base, std::size_t size);
  };
}