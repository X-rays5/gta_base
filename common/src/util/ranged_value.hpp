//
// Created by X-ray on 31/05/2025.
//

#ifndef RANGED_VALUE_HPP_05131809
#define RANGED_VALUE_HPP_05131809
#include <algorithm>
#include <limits>
#include <type_traits>
#include <glaze/glaze.hpp>

namespace base::common::util {
  template <typename T, T min_val = std::numeric_limits<T>::lowest(), T max_val = std::numeric_limits<T>::max()> requires std::is_arithmetic_v<T>
  class RangedValue {
  public:
    RangedValue() : value_(min_val) {}
    RangedValue(T val) : value_(Clamp(val)) {}

    operator T() const {
      return get();
    }

    T get() const {
      return value_;
    }

    void set(const T& val) {
      value_ = Clamp(val);
    }

    bool operator==(const T& val) const {
      return value_ == val;
    }

    auto operator<=>(const T& val) const {
      return value_ <=> val;
    }

    bool operator==(const RangedValue& other) const {
      return value_ == other.value_;
    }

    auto operator<=>(const RangedValue& other) const {
      return value_ <=> other.value_;
    }

    RangedValue& operator=(const T& val) {
      set(val);
      return *this;
    }

  private:
    T value_;

    T Clamp(const T& val) const {
      return std::clamp(val, min_val, max_val);
    }
  };
}

namespace glz {
  template <typename RangedValType, RangedValType min_val, RangedValType max_val>
  struct meta<base::common::util::RangedValue<RangedValType, min_val, max_val>> {
    using T = base::common::util::RangedValue<RangedValType, min_val, max_val>;
    static constexpr auto value = custom<&T::set, &T::get>;
  };
}

#endif //RANGED_VALUE_HPP_05131809
