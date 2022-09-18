//
// Created by X-ray on 09/18/22.
//

#include "global.hpp"
#include "../../memory/pointers.hpp"

namespace rage {
  namespace script {
    Global::Global(std::size_t idx) : idx_(idx) {}
    Global::Global(GlobalIdx idx) : Global(static_cast<std::size_t>(idx)) {}

    Global Global::At(std::ptrdiff_t idx) const {
      return Global(idx_ + idx);
    }

    Global Global::At(GlobalIdx idx) const {
      return At(static_cast<std::ptrdiff_t>(idx));
    }

    Global Global::At(std::ptrdiff_t idx, std::size_t arr_size) const {
      return Global(idx_ + 1 + (idx * arr_size));
    }

    Global Global::At(GlobalIdx idx, GlobalIdx arr_size) const {
      return At(static_cast<std::ptrdiff_t>(idx), static_cast<std::size_t>(arr_size));
    }

    void* Global::Get() const {
      return gta_base::memory::kPOINTERS->script_globals_[idx_ >> 0x12 & 0x3F] + (idx_ & 0x3FFFF);
    }
  }
}