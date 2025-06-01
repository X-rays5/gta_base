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
      return value_;
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
      value_ = Clamp(val);
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
  template <typename T, T min_val, T max_val>
  struct meta<base::common::util::RangedValue<T, min_val, max_val>> {
    using Value = base::common::util::RangedValue<T, min_val, max_val>;

    // Tell glaze to treat RangedValue as its underlying type
    static constexpr auto value = T{};

    // Conversion functions
    static constexpr auto to_value = [](auto&& self) -> T {
      return static_cast<T>(self);
    };

    // Change the parameter name from "value" to "val" to avoid the conflict
    static constexpr auto from_value = [](Value& self, T&& val) {
      self = val;
      return true;
    };
  };
}

#endif //RANGED_VALUE_HPP_05131809
