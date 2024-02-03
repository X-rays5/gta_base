#include "range.hpp"
#include "pattern.hpp"

namespace base::memory::scanner {
  Range::Range(Handle base, std::size_t size)
    :
    base_(base), size_(size) {}

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
    return h.as<std::uintptr_t>() >= begin().as<std::uintptr_t>() && h.as<std::uintptr_t>() <= end().as<
      std::uintptr_t>();
  }

  // https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore%E2%80%93Horspool_algorithm
  // https://www.youtube.com/watch?v=AuZUeshhy-s
  StatusOr<Handle> scan_pattern(const std::optional<std::uint8_t>* sig, std::size_t length, Handle begin,
                                std::size_t module_size) {
    std::size_t maxShift = length;
    const std::size_t max_idx = length - 1;

    //Get wildcard index, and store max shiftable byte count
    std::size_t wild_card_idx{static_cast<size_t>(-1)};
    for (int i{static_cast<int>(max_idx - 1)}; i >= 0; --i) {
      if (!sig[i]) {
        maxShift = max_idx - i;
        wild_card_idx = i;
        break;
      }
    }

    //Store max shiftable bytes for non wildcards.
    std::size_t shift_table[UINT8_MAX + 1]{};
    for (std::size_t i{}; i <= UINT8_MAX; ++i) {
      shift_table[i] = maxShift;
    }

    //Fill shift table with sig bytes
    for (std::size_t i{wild_card_idx + 1}; i != max_idx; ++i) {
      shift_table[*sig[i]] = max_idx - i;
    }

    //Loop data
    for (std::size_t current_idx{}; current_idx <= module_size - length;) {
      for (std::size_t sig_idx{max_idx}; true; --sig_idx) {
        if (sig[sig_idx] && *begin.add(current_idx + sig_idx).as<uint8_t*>() != *sig[sig_idx]) {
          current_idx += shift_table[*begin.add(current_idx + max_idx).as<uint8_t*>()];
          break;
        } else if (sig_idx == NULL) {
          return begin.add(current_idx);
        }
      }
    }

    return MakeFailure<ResultCode::kNOT_FOUND>("Failed to find pattern");
  }

  StatusOr<Handle> Range::scan(Pattern const& sig) {
    const auto data = sig.bytes_.data();
    const auto length = sig.bytes_.size();

    return scan_pattern(data, length, base_, size_);
  }

  bool pattern_matches(const std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length) {
    for (std::size_t i{}; i != length; ++i) {
      if (sig[i] && *sig[i] != target[i]) {
        return false;
      }
    }

    return true;
  }

  std::vector<Handle> Range::scan_all(Pattern const& sig) {
    std::vector<Handle> result{};
    const auto data = sig.bytes_.data();
    const auto length = sig.bytes_.size();

    const auto search_end = size_ - length;
    for (std::uintptr_t i = 0; i < search_end; ++i) {
      if (pattern_matches(base_.add(i).as<std::uint8_t*>(), data, length)) {
        result.push_back(base_.add(i));
      }
    }

    return std::move(result);
  }
}
