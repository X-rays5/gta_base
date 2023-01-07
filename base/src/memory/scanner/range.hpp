#pragma once
#include <vector>
#include "pattern.hpp"
#include "handle.hpp"

namespace gta_base::memory::scanner {
class Range {
 public:
  Range(Handle base, std::size_t size);

  Handle begin();
  Handle end();
  std::size_t size();

  bool contains(Handle h);

  Handle scan(Pattern const &sig);
  Handle scan(char *sig);
  Handle bruteforce_scan(Pattern const &sig);
  Handle bruteforce_scan(char *sig);
  std::vector<Handle> scan_all(Pattern const &sig);
  std::vector<Handle> scan_all(char *sig);
 protected:
  Handle base_;
  std::size_t size_;
};
}