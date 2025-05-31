//
// Created by X-ray on 31/05/2025.
//

#ifndef RANGED_VALUE_HPP_05131809
#define RANGED_VALUE_HPP_05131809
#include <algorithm>
#include <limits>
#include <type_traits>

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

#endif //RANGED_VALUE_HPP_05131809
