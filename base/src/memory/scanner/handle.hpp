#pragma once
#include <cstddef>
#include <type_traits>

namespace gta_base::memory::scanner {
      class Handle {
      public:
        Handle(void* ptr = nullptr);
        explicit Handle(std::uintptr_t ptr);

        template <typename T>
        std::enable_if_t<std::is_pointer_v<T>, T> as();

        template <typename T>
        std::enable_if_t<std::is_lvalue_reference_v<T>, T> as();

        template <typename T>
        std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> as();

        template <typename T>
        Handle add(T offset);

        template <typename T>
        Handle sub(T offset);

        Handle rip();

        explicit operator bool();

        friend bool operator==(Handle a, Handle b);
        friend bool operator!=(Handle a, Handle b);
      private:
        void* ptr_;
      };

      inline Handle::Handle(void* ptr) : ptr_(ptr) {}

      inline Handle::Handle(std::uintptr_t ptr) : ptr_(reinterpret_cast<void*>(ptr)) {}

      template <typename T>
      inline std::enable_if_t<std::is_pointer_v<T>, T> Handle::as() {
        return static_cast<T>(ptr_);
      }

      template <typename T>
      inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Handle::as() {
        return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(ptr_);
      }

      template <typename T>
      inline std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> Handle::as() {
        return reinterpret_cast<std::uintptr_t>(ptr_);
      }

      template <typename T>
      inline Handle Handle::add(T offset) {
        return Handle(as<std::uintptr_t>() + offset);
      }

      template <typename T>
      inline Handle Handle::sub(T offset) {
        return Handle(as<std::uintptr_t>() - offset);
      }

      inline Handle Handle::rip() {
        return add(as<std::int32_t&>()).add(4);
      }

      inline bool operator==(Handle a, Handle b) {
        return a.ptr_ == b.ptr_;
      }

      inline bool operator!=(Handle a, Handle b) {
        return a.ptr_ != b.ptr_;
      }

      inline Handle::operator bool() {
        return ptr_ != nullptr;
      }
    }