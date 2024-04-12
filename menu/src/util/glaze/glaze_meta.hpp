//
// Created by X-ray on 10/01/2024.
//

#ifndef GLAZE_META_HPP_01223842
#define GLAZE_META_HPP_01223842
#include <glaze/glaze.hpp>
#include <ankerl/unordered_dense.h>

namespace glz {
  template <class T>
  concept unordered_dense_map = is_specialization_v<T, ankerl::unordered_dense::map>;

  template <unordered_dense_map T>
  struct meta<T> {
    using Key = typename T::key_type;
    using V = typename T::mapped_type;
    static constexpr std::string_view name = join_v<chars<"ankerl::unordered_dense::map<">, name_v<Key>, chars<",">, name_v<V>, chars<">">>;
  };
}
#endif //GLAZE_META_HPP_01223842
