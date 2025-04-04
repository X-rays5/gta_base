//
// Created by X-ray on 11/01/2024.
//

#ifndef TYPE_TRAITS_HPP_01233709
#define TYPE_TRAITS_HPP_01233709
#include <type_traits>

namespace base::menu::util {
  template <typename T>
  struct is_char_array : std::false_type {};

  template <size_t N>
  struct is_char_array<char[N]> : std::true_type {};

  template <>
  struct is_char_array<const char*> : std::true_type {};

  template <>
  struct is_char_array<char*> : std::true_type {};

  template <typename T>
  inline constexpr bool is_char_array_v = is_char_array<T>::value;

  template <typename T>
  concept char_array = is_char_array_v<T>;

  template <class, template <class...> class>
  inline constexpr bool is_specialization_v = false;

  template <template <class...> class T, class... Args>
  inline constexpr bool is_specialization_v<T<Args...>, T> = true;
}
#endif //TYPE_TRAITS_HPP_01233709
