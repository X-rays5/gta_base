//
// Created by X-ray on 15/09/2026.
//

#pragma once
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>

namespace base::menu::game {
  /**
   * A game handle - a Ped, a Vehicle, a Blip - and the whole of it is the 32 bit value.
   *
   * The class exists because the handle is what an operation belongs to: a ped can teleport itself, a
   * vehicle can be repaired, a blip can be asked where it is. What it is not is a wrapper owning the
   * value - the value *is* the handle, so one of these passes exactly as the int32_t it replaces and
   * the generated natives keep the signatures they already have.
   *
   * That sameness is a contract rather than a coincidence, and the party it is owed to is AngelScript:
   * the bindings declare every handle as `int` and call the native function directly, so a handle has to
   * be a value a script can pass as one. A second member, or a virtual, or anything else that stopped
   * this being a register-sized trivial value would make the compiler pass one by reference, and the
   * script would be handing the native a reference where it reads a value. Inside C++ the layout is
   * already taken out of the picture - the invoker pushes the identity off a handle (Invoker::NativeCall)
   * - so the assert below is what holds up the script side of the promise.
   */
  class ScrHandle {
  public:
    constexpr ScrHandle() = default;
    constexpr ScrHandle(const std::int32_t value) : value_(value) {}

    /// Implicit on purpose: comparisons against 0, raw native calls and the game's own bookkeeping are
    /// all written in terms of the raw integer.
    constexpr operator std::int32_t() const {
      return value_;
    }

    [[nodiscard]] constexpr std::int32_t Get() const {
      return value_;
    }

    /// Whether a handle was handed out at all. What it names may be long gone by now - that is the
    /// question Exists() puts to the game.
    [[nodiscard]] constexpr bool IsValid() const {
      return value_ != 0;
    }

  private:
    std::int32_t value_{};
  };

  static_assert(sizeof(ScrHandle) == sizeof(std::int32_t) && std::is_standard_layout_v<ScrHandle> &&
                  std::is_trivially_copyable_v<ScrHandle>,
                "A handle has to stay a 32 bit trivial value at offset 0 - that is the ABI the natives and the script bindings are written against.");

  /// Handles with nothing of their own to do yet. They are here because the database says an operation
  /// can take one - a native asking for a Cam should not be handed a Ped - and each earns a file of its
  /// own the moment it has an operation to carry.
  class Cam : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;
  };
  static_assert(sizeof(Cam) == sizeof(ScrHandle));

  class Pickup : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;
  };
  static_assert(sizeof(Pickup) == sizeof(ScrHandle));

  class Interior : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;
  };
  static_assert(sizeof(Interior) == sizeof(ScrHandle));

  class FireId : public ScrHandle {
  public:
    using ScrHandle::ScrHandle;
  };
  static_assert(sizeof(FireId) == sizeof(ScrHandle));

  /// A handle reads as the number it is. fmt does not go through conversion operators, so without this
  /// a handle in a log line or a notification would not compile - and the number is the useful half.
  inline std::int32_t format_as(const ScrHandle& handle) {
    return handle.Get();
  }
}

/// A handle keys a map as the int it is, which is what the int it replaced did.
template <typename T>
  requires std::derived_from<T, ::base::menu::game::ScrHandle>
struct std::hash<T> {
  std::size_t operator()(const T& handle) const noexcept {
    return std::hash<std::int32_t>{}(handle.Get());
  }
};
