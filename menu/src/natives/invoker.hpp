//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <rage/script/custom_call_context.hpp>
#include "../game/handles.hpp"
#include "crossmap.hpp"

namespace base::menu::natives {
  namespace detail {
    /// A handle as a value. Only its identity reaches the game.
    template <typename T>
    concept HandleValue = std::derived_from<std::remove_cvref_t<T>, game::ScrHandle>;

    /// A pointer to a handle, which is the script API's `*` spelling: the game reads the handle through
    /// it and writes what came out back into it, so one is in, out or both, and never just an address.
    /// A pointer to a const handle is the API saying it is only read, so there is nothing to write back.
    template <typename T>
    concept HandlePointer = std::is_pointer_v<std::remove_cvref_t<T>> &&
                            std::derived_from<std::remove_pointer_t<std::remove_cvref_t<T>>, game::ScrHandle> &&
                            !std::is_const_v<std::remove_pointer_t<std::remove_cvref_t<T>>>;
  }

  class Invoker {
  public:
    class NativeCall {
    public:

      /**
       * Every handle that crosses here crosses as the identity it is, so nothing about a native call
       * depends on what a handle class happens to contain - a handle is free to grow and the calls the
       * game is given keep the shape they had.
       *
       * A pointer to a handle is a caller asking the game for one (DELETE_PED, REMOVE_BLIP and the other
       * `*` parameters), so the game is handed a slot of ours to write into rather than the caller's
       * object, and the object is built from what lands in that slot once the call returns.
       */
      template<typename T>
      void PushArg(T&& value) {
        if constexpr (detail::HandlePointer<T>) {
          PushHandleOut(value);
        } else if constexpr (detail::HandleValue<T>) {
          context_.PushArg(value.Get());
        } else {
          context_.PushArg(std::forward<T>(value));
        }
      }

      void EndCall(std::uint64_t native_idx);

      template<typename T>
      T& GetReturnValue()
      {
        // Special handling for Vector3 - convert from script::Vector to rage::Vector3
        if constexpr (std::is_same_v<T, ::rage::Vector3>) {
          auto* script_vec = static_cast<::rage::script::Vector*>(context_.GetReturnValue<void>());
          static thread_local ::rage::Vector3 converted;
          converted = ::rage::Vector3(script_vec->x, script_vec->y, script_vec->z);
          return *reinterpret_cast<T*>(&converted);
        } else if constexpr (detail::HandleValue<T>) {
          // The mirror of the above: the game leaves the identity behind, and the object is built on it.
          static thread_local T converted;
          converted = T{*context_.GetReturnValue<std::int32_t>()};
          return converted;
        } else {
          return *context_.GetReturnValue<T>();
        }
      }

    private:
      /// One of the `*` parameters of the call being made.
      struct HandleOut {
        void* target;
        void (*write)(void* target, std::int32_t identity);
        /// A `*` in the script API is a pointer to a 64 bit slot - the dumper's own tables spell these
        /// `uint64_t*` - and a handle is 32 bits of it. Sizing this by the handle instead would leave the
        /// game writing four bytes past it.
        std::uint64_t slot;
      };

      /// One slot per out handle, exactly as many as the arguments themselves can be, so the table cannot
      /// fill before the argument stack has already run off its own end. That is what retires the idea of
      /// an overflow branch: there is no fuller table to guard against, and the only fallback available -
      /// handing the game the caller's own four byte object to write a slot into - would be a corruption.
      static constexpr std::size_t kMAX_HANDLE_OUTS = ::rage::script::CustomCallContext::kMAX_ARGS;

      template <typename T>
      void PushHandleOut(T* const target) {
        // The database lets these be null where the result is not wanted - GET_ENTITY_SCRIPT documents
        // its second parameter as unused - and then there is no object to rebuild.
        if (target == nullptr) {
          context_.PushArg(target);
          return;
        }

        auto& out = handle_outs_[handle_out_count_++];
        out.target = target;
        out.write = [](void* const target, const std::int32_t identity) {
          *static_cast<T*>(target) = std::remove_cv_t<T>{identity};
        };
        // Zero extended: the game may read all eight bytes of the slot, and the identity is what it has
        // to find there.
        out.slot = static_cast<std::uint32_t>(target->Get());
        context_.PushArg(&out.slot);
      }

      /// The handles the call left behind, built into the objects they were asked for.
      void ApplyHandleOuts() {
        for (std::size_t i = 0; i < handle_out_count_; i++) {
          auto& out = handle_outs_[i];
          out.write(out.target, static_cast<std::int32_t>(static_cast<std::uint32_t>(out.slot)));
        }
        handle_out_count_ = 0;
      }

      ::rage::script::CustomCallContext context_{};
      std::array<HandleOut, kMAX_HANDLE_OUTS> handle_outs_{};
      std::size_t handle_out_count_{};
    };

  public:
    Invoker();
    ~Invoker();

    template<typename Ret, std::uint64_t native_idx, typename... Args>
    static Ret Invoke(Args&&... args) {
      NativeCall call;
      (call.PushArg(std::forward<Args>(args)), ...);
      call.EndCall(native_idx);

      if constexpr (!std::is_same_v<Ret, void>) {
        return call.GetReturnValue<Ret>();
      } else {
        return;
      }
    }

    // Getter for native handlers (used for dumping)
    const auto& GetNativeHandlers() const {
      return native_handlers_;
    }

  private:
    std::array<rage::script::NativeHandler, NATIVE_CROSSMAP_SIZE> native_handlers_{};
  };
  inline Invoker* kINVOKER{};
}
