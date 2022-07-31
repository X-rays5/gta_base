//
// Created by X-ray on 07/31/22.
//

#pragma once
#ifndef GTA_BASE_VMT_HPP
#define GTA_BASE_VMT_HPP
#include <memory>

namespace gta_base {
  namespace hooking {
    class VmtHook {
    public:
      explicit VmtHook(void* obj, std::size_t num_funcs);

      VmtHook(VmtHook&& that) = delete;
      VmtHook& operator=(VmtHook&& that) = delete;
      VmtHook(VmtHook const&) = delete;
      VmtHook& operator=(VmtHook const&) = delete;

      void Hook(std::size_t index, void* func);
      void Unhook(std::size_t index);

      template <typename T>
      T GetOriginal(std::size_t index) {
        return static_cast<T>(og_table_[index]);
      }

      void Enable();
      void Disable();
    private:
      void*** object_;
      std::size_t num_funcs_;

      void** og_table_;
      std::unique_ptr<void*[]> new_table_;
    };
  }
}
#endif //GTA_BASE_VMT_HPP
