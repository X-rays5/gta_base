//
// Created by X-ray on 07/31/22.
//

#include "vmt.hpp"

namespace gta_base {
  namespace hooking {
    VmtHook::VmtHook(void* obj, std::size_t num_funcs) : object_(static_cast<void***>(obj)), num_funcs_(num_funcs + 1), og_table_(*object_), new_table_(std::make_unique<void*[]>(num_funcs_)) {
      std::copy_n(og_table_ - 1, num_funcs_, new_table_.get());
    }

    void VmtHook::Hook(std::size_t index, void* func) {
      new_table_[index + 1] = func;
    }

    void VmtHook::Unhook(std::size_t index) {
      new_table_[index + 1] = og_table_[index];
    }

    void VmtHook::Enable() {
      *object_ = new_table_.get() + 1;
    }

    void VmtHook::Disable() {
      *object_ = og_table_;
    }
  }
}