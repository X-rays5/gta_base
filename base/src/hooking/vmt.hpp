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
      explicit VmtHook(void* object, std::size_t num_funcs) noexcept : object_(reinterpret_cast<std::uintptr_t**>(object)), num_funcs_(num_funcs), og_(*object_), new_(std::make_unique<std::uintptr_t[]>(num_funcs_ + 1)) {
        std::copy_n(og_ - 1, num_funcs_ + 1, new_.get());
      }

      ~VmtHook() noexcept = default;
      VmtHook(VmtHook const&) noexcept = delete;
      VmtHook(VmtHook&&) noexcept = delete;
      VmtHook& operator=(VmtHook const&) noexcept = delete;
      VmtHook& operator=(VmtHook&&) noexcept = delete;


      void Hook(std::size_t index, void* func) noexcept {
        new_[index + 1] = reinterpret_cast<std::uintptr_t>(func);
      }

      void Unhook(std::size_t index) noexcept {
        new_[index + 1] = og_[index];
      }

      void Enable() noexcept {
        *object_ = &new_[1];
      }

      void Disable() noexcept {
        *object_ = og_;
      }

      template <typename T>
      T GetOriginal(std::size_t index) noexcept {
        return reinterpret_cast<T>(og_[index]);
      }
    private:
      std::uintptr_t** object_;
      std::size_t num_funcs_;
      std::uintptr_t* og_;
      std::unique_ptr<std::uintptr_t[]> new_;

    };
  }
}
#endif //GTA_BASE_VMT_HPP
