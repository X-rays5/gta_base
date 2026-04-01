//
// Created by X-ray on 01/04/2026.
//

#pragma once

#include <chrono>
#include <cstdint>
#include <exception>

namespace minicoropp {
  /**
   * @brief Lightweight coroutine wrapper around minicoro library
   *
   * Thread-safety: NOT thread-safe. Do not call resume() from multiple threads.
   *
   * User Data Ownership: The wrapper does not own user_data. The caller is responsible for:
   * - Allocating user_data if needed
   * - Ensuring user_data lifetime extends until coroutine completion
   * - Deallocating user_data after coroutine is destroyed
   *
   * Exception Handling: Exceptions thrown in the coroutine function are captured.
   * Call exception() to check if an exception occurred, or rethrow() to propagate it.
   */

  namespace detail {
    class CoroutineDetail;

    namespace this_coro {
      CoroutineDetail* GetCurrentCoroutineDetail();
    }
  }

  namespace this_coro {
    /**
     * @brief Yield execution from the current coroutine
     * @note If called outside a coroutine context, this will call std::this_thread::yield()
     */
    void yield();

    // Non-template helper functions that will be defined in cpp
    namespace detail {
      void sleep_for_impl(const std::chrono::high_resolution_clock::duration& sleep_duration);
      void sleep_until_impl(const std::chrono::high_resolution_clock::time_point& sleep_time);
    }

    /**
     * @brief Sleep for a specified duration
     * @param sleep_duration How long to sleep
     * @note If called outside a coroutine context, this will call std::this_thread::sleep_for()
     */
    template<class Rep, class Period>
    void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration) {
      detail::sleep_for_impl(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(sleep_duration));
    }

    /**
     * @brief Sleep until a specified time point
     * @param sleep_time The time point to wake up at
     * @note Supports any Clock type; internally converted to high_resolution_clock
     * @note If called outside a coroutine context, this will call std::this_thread::sleep_until()
     */
    template< class Clock, class Duration >
    void sleep_until(const std::chrono::time_point<Clock, Duration>& sleep_time) {
      if constexpr (std::is_same_v<Clock, std::chrono::high_resolution_clock>) {
        detail::sleep_until_impl(std::chrono::time_point_cast<std::chrono::high_resolution_clock::duration>(sleep_time));
      } else {
        const auto remaining = sleep_time - Clock::now();
        detail::sleep_for_impl(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(remaining));
      }
    }

    /**
     * @brief Get user data for the current coroutine
     * @return Pointer to user data passed during coroutine creation, or nullptr if outside coroutine context
     */
    void* get_data();

    /**
     * @brief Get an opaque handle to the current coroutine.
     * @return Handle pointer, or nullptr if not in a coroutine.
     */
    void* get_current_handle();

    /**
     * @brief Suspend the current coroutine indefinitely until woken up manually.
     */
    void suspend_indefinitely();

    /**
     * @brief Wake a specific coroutine from indefinite suspension.
     * @param handle The opaque handle of the coroutine to wake
     */
    void wake_handle(void* handle);
  }

  /**
   * @brief State of a coroutine (from minicoro mco_status)
   */
  enum class CoroState : std::uint8_t {
    /* The coroutine has finished normally or was uninitialized before finishing. */
    kDEAD = 0,
    /* The coroutine is active but not running (that is, it has resumed another coroutine). */
    kNORMAL,
    /* The coroutine is active and running. */
    kRUNNING,
    /* The coroutine is suspended (in a call to yield, or it has not started running yet). */
    kSUSPENDED,
  };

  /**
   * @brief Result of resuming a coroutine
   * Values 0-12 are from minicoro mco_result.
   * Values >= 200 are custom results from this wrapper.
   */
  enum class CoroResult : std::uint8_t {
    kSUCCESS = 0,              // Coroutine resumed successfully
    kGENERIC_ERROR,            // Generic minicoro error
    kINVALID_POINTER,          // Invalid pointer passed to minicoro
    kINVALID_COROUTINE,        // Invalid coroutine handle
    kNOT_SUSPENDED,            // Attempted to resume non-suspended coroutine
    kNOT_RUNNING,              // Attempted operation on non-running coroutine
    kMAKE_CONTEXT_ERROR,       // Failed to create context
    kSWITCH_CONTEXT_ERROR,     // Failed to switch context
    kNOT_ENOUGH_SPACE,         // Stack space insufficient
    kOUT_OF_MEMORY,            // Out of memory
    kINVALID_ARGUMENTS,        // Invalid arguments
    kINVALID_OPERATION,        // Invalid operation
    kSTACK_OVERFLOW,           // Stack overflow
    kYIELDING = 200,           // Coroutine is still yielding (sleep not finished)
    kEXCEPTION                 // Exception was thrown in coroutine
  };

  /**
   * @brief Coroutine wrapper class providing safe RAII semantics
   *
   * Usage:
   *   Coroutine coro([](){
   *     this_coro::sleep_for(std::chrono::milliseconds(100));
   *     this_coro::yield();
   *   });
   *
   *   while(coro.resume() == CoroResult::kSUCCESS) {
   *     // Coroutine executing...
   *   }
   *
   *   if (coro.exception()) {
   *     std::rethrow_exception(coro.exception());
   *   }
   */
  class Coroutine {
  public:
    /**
     * @brief Create a new coroutine
     * @param func The function to execute in the coroutine
     * @param user_data Optional user data accessible via this_coro::get_data()
     * @param stack_size Stack size in bytes (default 64KB)
     * @throws std::runtime_error if coroutine creation fails
     */
    template <typename F>
    explicit Coroutine(F&& func, void* user_data = nullptr, std::size_t stack_size = 64000) {
      using DecayedF = std::decay_t<F>;
      auto* callable = new DecayedF(std::forward<F>(func));

      void (*invoke_fn)(void*) = [](void* ptr) {
        (*static_cast<DecayedF*>(ptr))();
      };

      void (*destroy_fn)(void*) = [](void* ptr) {
        delete static_cast<DecayedF*>(ptr);
      };

      Init(callable, invoke_fn, destroy_fn, user_data, stack_size);
    }
    ~Coroutine();

    Coroutine(Coroutine&&) noexcept;
    Coroutine& operator=(Coroutine&&) noexcept;
    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;

    /**
     * @brief Resume coroutine execution
     * @return CoroResult indicating success, error, or special states (kYIELDING, kEXCEPTION)
     * @note NOT thread-safe. Must be called from the same thread.
     */
    CoroResult resume() const;

    /**
     * @brief Get the current coroutine state
     * @return CoroState (kDEAD, kNORMAL, kRUNNING, or kSUSPENDED)
     * @note NOT thread-safe.
     */
    CoroState state() const;

    /**
     * @brief Get any exception thrown in the coroutine
     * @return std::exception_ptr containing the exception, or null if no exception
     * @note Use std::rethrow_exception() to re-throw the exception if needed
     */
    std::exception_ptr exception() const;

  private:
    void Init(void* func_ptr, void (*invoke_fn)(void*), void (*destroy_fn)(void*), void* user_data, std::size_t stack_size);

    detail::CoroutineDetail* detail_ = nullptr;
  };
}