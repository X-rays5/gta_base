//
// Created by X-ray on 09/04/2024.
//

#ifndef ADDRESS_HPP
#define ADDRESS_HPP
#include <Windows.h>
#include "../util/result.hpp"

namespace base::common::memory {
  class Address {
  public:
    explicit Address(void* addr) : addr_(reinterpret_cast<std::uintptr_t>(addr)) {}
    explicit Address(const std::uintptr_t addr) : addr_(addr) {}

    template <typename T> requires std::is_pointer_v<T>
    T As() {
      return static_cast<T>(static_cast<void*>(*this));
    }

    template <typename T> requires std::is_lvalue_reference_v<T>
    T As() {
      return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(static_cast<void*>(*this));
    }

    template <typename T> requires std::is_same_v<T, std::uintptr_t>
    T As() {
      return addr_;
    }

    Address Sub(const std::uintptr_t offset) const {
      return Address(addr_ - offset);
    }

    Address Add(const std::uintptr_t offset) const {
      return Address(addr_ + offset);
    }

    Address Rip() {
      return Add(As<std::int32_t&>()).Add(4);
    }

    StatusOr<MEMORY_BASIC_INFORMATION> GetMemoryInformation() {
      MEMORY_BASIC_INFORMATION memory_info;

      const SIZE_T res = VirtualQuery(static_cast<void*>(*this), &memory_info, sizeof(memory_info));
      if (res != sizeof(memory_info)) {
        return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to get memory info for addr: 0x{:X}", addr_);
      }
      return memory_info;
    }

    explicit operator void*() const {
      return reinterpret_cast<void*>(addr_);
    }

    explicit operator std::uintptr_t() const {
      return addr_;
    }

  private:
    std::uintptr_t addr_;
  };
}
#endif //ADDRESS_HPP
