//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

namespace base::common::util {
  // Wraps one callable, of exactly that type. Able to forward arguments and constness
  // to the target, since nothing is erased.
  template <typename F>
  class Callback {
  public:
    explicit constexpr Callback(F f)
        : callable_(std::move(f)) {}

    // Explicit object parameter ("deducing this"). The constraint must trail the
    // declarator: an explicit object parameter is not in scope in a template-head
    // requires-clause.
    template <typename Self, typename... Args>
    constexpr decltype(auto) operator()(this Self&& self, Args&&... args)
    requires std::invocable<decltype((std::forward<Self>(self).callable_)), Args...> {
        return std::invoke(
            std::forward<Self>(self).callable_,
            std::forward<Args>(args)...
        );
    }

  private:
    F callable_;
  };

  template <typename F>
  Callback(F) -> Callback<F>;

  namespace detail {
    // Whether a target can be stored in a Callback with the given signature.
    // kConstInvocable is what the two erased signatures differ in.
    template <typename R, bool kConstInvocable, typename F, typename... Args>
    concept StorableTarget = std::is_invocable_r_v<R, std::conditional_t<kConstInvocable, const F&, F&>, Args...>;

    // The storage and vtable shared by Callback<R(Args...)> and Callback<R(Args...) const>.
    // A target that does not fit, is over-aligned, or cannot be moved without throwing is
    // rejected at compile time rather than falling back to the heap.
    template <typename R, bool kConstInvocable, typename... Args>
    class CallbackState {
    public:
      CallbackState() = default;

      template <typename F>
      requires (!std::is_same_v<std::decay_t<F>, CallbackState>)
            && StorableTarget<R, kConstInvocable, std::decay_t<F>, Args...>
      CallbackState(F f) {
        using DF = std::decay_t<F>;
        using StoredPtr = std::conditional_t<kConstInvocable, const DF*, DF*>;

        static_assert(sizeof(DF) <= kBufSize, "callable too large for the inline storage");
        static_assert(alignof(DF) <= alignof(std::max_align_t), "callable is over-aligned for the inline storage");
        static_assert(std::is_nothrow_move_constructible_v<DF>, "callable must be move constructible without throwing, as Callback's moves are noexcept");

        ::new (static_cast<void*>(storage_)) DF(std::move(f));

        // The storage's declared type is unsigned char, not DF, so every pointer
        // recovered from it is laundered before use. A no-op at runtime; it keeps the
        // byte buffer from being treated as a fresh, unrelated object.
        invoke_ = [](TargetPtr self, Args... args) -> R {
          return std::invoke(*std::launder(static_cast<StoredPtr>(self)), std::forward<Args>(args)...);
        };
        // Called only from reset(), which always runs on a non-const Callback.
        destroy_ = [](void* self) {
          std::launder(static_cast<DF*>(self))->~DF();
        };
        // Deliberately leaves the source alive: reset() destroys it. Destroying it here
        // as well would run the destructor twice on the same object.
        move_ = [](void* src, void* dst) {
          ::new (dst) DF(std::move(*std::launder(static_cast<DF*>(src))));
        };
      }

      CallbackState(const CallbackState&) = delete;
      CallbackState& operator=(const CallbackState&) = delete;

      CallbackState(CallbackState&& other) noexcept {
        if (other.invoke_) {
          other.move_(other.storage_, storage_);
          invoke_ = other.invoke_;
          destroy_ = other.destroy_;
          move_ = other.move_;
          other.reset();
        }
      }

      CallbackState& operator=(CallbackState&& other) noexcept {
        if (this != &other) {
          reset();
          if (other.invoke_) {
            other.move_(other.storage_, storage_);
            invoke_ = other.invoke_;
            destroy_ = other.destroy_;
            move_ = other.move_;
            other.reset();
          }
        }
        return *this;
      }

      ~CallbackState() { reset(); }

      // Deducing this again: the pointer handed to the target follows the constness of
      // the Callback, so a mutable target is unreachable through a const Callback. The
      // constraint keeps the const path from existing at all for the signature whose
      // targets are only non-const-invocable.
      template <typename Self>
      R invoke(this Self&& self, Args... args)
      requires (kConstInvocable || !std::is_const_v<std::remove_reference_t<Self>>) {
        return std::forward<Self>(self).invoke_(std::forward<Self>(self).storage_, std::forward<Args>(args)...);
      }

      explicit operator bool() const noexcept { return invoke_ != nullptr; }

    private:
      // The pointer the target is reached through, which is the whole difference between
      // the two signatures this state backs.
      using TargetPtr = std::conditional_t<kConstInvocable, const void*, void*>;

      // Must precede storage_, whose array bound is not a complete-class context.
      static constexpr std::size_t kBufSize = 32;

      void reset() noexcept {
        if (destroy_) {
          destroy_(storage_);
        }
        invoke_ = nullptr;
        destroy_ = nullptr;
        move_ = nullptr;
      }

      alignas(std::max_align_t) unsigned char storage_[kBufSize];

      R (*invoke_)(TargetPtr, Args...) = nullptr;
      void (*destroy_)(void*) = nullptr;
      void (*move_)(void*, void*) = nullptr;
    };
  }

  // Type-erased callback. Callback<R(Args...)> stores any callable matching that
  // signature in inline storage, so callables of different concrete types can share one
  // container. The signature has to be spelled out: CTAD cannot recover R and Args...
  // from a callable, so Callback{lambda} still selects the wrapper above.
  //
  // Targets are invoked through a non-const pointer, so a mutable target is allowed but
  // only a non-const Callback can call it.
  //
  // Unlike std::function, invoking an empty Callback is undefined: callers are expected
  // to test operator bool first.
  template <typename R, typename... Args>
  class Callback<R(Args...)> {
  public:
    Callback() = default;

    // StorableTarget is repeated here rather than left to CallbackState's own
    // constraint: is_constructible has to be told the truth, or generic code that asks
    // would select this constructor and only then fail on the member initializer. MSVC
    // reports true for such a target without this clause, since it does not instantiate
    // the member initializer to answer the trait.
    template <typename F>
    requires (!std::is_same_v<std::decay_t<F>, Callback>)
          && detail::StorableTarget<R, false, std::decay_t<F>, Args...>
    Callback(F f)
        : state_(std::move(f)) {}

    R operator()(Args... args) {
      return state_.invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept { return static_cast<bool>(state_); }

  private:
    detail::CallbackState<R, false, Args...> state_;
  };

  // Type-erased callback whose targets are const-invocable, and which is therefore
  // callable through a const Callback. This is the spelling to reach for by default: an
  // ordinary lambda is const-invocable, and const access is what most call sites want.
  // The plain Callback<R(Args...)> is for targets that need to be told apart from those,
  // i.e. mutable ones.
  template <typename R, typename... Args>
  class Callback<R(Args...) const> {
  public:
    Callback() = default;

    template <typename F>
    requires (!std::is_same_v<std::decay_t<F>, Callback>)
          && detail::StorableTarget<R, true, std::decay_t<F>, Args...>
    Callback(F f)
        : state_(std::move(f)) {}

    R operator()(Args... args) const {
      return state_.invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept { return static_cast<bool>(state_); }

  private:
    detail::CallbackState<R, true, Args...> state_;
  };
}
