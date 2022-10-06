#include "range.hpp"
#include "pattern.hpp"

namespace gta_base::memory::scanner {
      Range::Range(Handle base, std::size_t size) : base_(base), size_(size) {}

      Handle Range::begin() {
        return base_;
      }

      Handle Range::end() {
        return base_.add(size_);
      }

      std::size_t Range::size() {
        return size_;
      }

      bool Range::contains(Handle h) {
        return h.as<std::uintptr_t>() >= begin().as<std::uintptr_t>() && h.as<std::uintptr_t>() <= end().as<std::uintptr_t>();
      }

      static bool pattern_matches(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length) {
        for (std::size_t i = 0; i < length; ++i) {
          if (sig[i] && *sig[i] != target[i])
            return false;
        }

        return true;
      };

      Handle Range::scan(Pattern const &sig) {
        auto data = sig.bytes_.data();
        auto length = sig.bytes_.size();
        for (std::uintptr_t i = 0; i < size_ - length; ++i) {
          if (pattern_matches(base_.add(i).as<std::uint8_t*>(), data, length)) {
            return base_.add(i);
          }
        }

        return nullptr;
      }

      Handle Range::scan(char* sig) {
        return scan(Pattern(sig));
      }

      std::vector<Handle> Range::scan_all(Pattern const &sig) {
        std::vector<Handle> result;

        auto data = sig.bytes_.data();
        auto length = sig.bytes_.size();
        for (std::uintptr_t i = 0; i < size_ - length; ++i) {
          if (pattern_matches(base_.add(i).as<std::uint8_t*>(), data, length)) {
            result.push_back(base_.add(i));
          }
        }

        return std::move(result);
      }

      std::vector<Handle> Range::scan_all(char* sig) {
        return scan_all(Pattern(sig));
      }
    }