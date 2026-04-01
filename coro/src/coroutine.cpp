//
// Created by X-ray on 01/04/2026.
//

#include "coroutine.hpp"

#include <cassert>
#include <exception>
#include <stdexcept>
#include <thread>
#include <base-coro/minicoro_config.h>
#define MINICORO_IMPL
#include <minicoro/minicoro.h>

namespace minicoropp {
  namespace detail {
    void CoreExecWrapper(mco_coro* co);

    class CoroutineDetail {
    public:
      explicit CoroutineDetail(void* func_ptr, void (*invoke_fn)(void*), void (*destroy_fn)(void*), void* user_data, const std::size_t stack_size)
        : func_ptr_(func_ptr), invoke_fn_(invoke_fn), destroy_fn_(destroy_fn), user_data(user_data), yield_till(std::chrono::high_resolution_clock::now()) {
        desc_ = mco_desc_init(CoreExecWrapper, stack_size);
        desc_.user_data = this;

        const mco_result res = mco_create(&co, &desc_);
        if (res != MCO_SUCCESS) {
          throw std::runtime_error("CoroutineDetail constructor failed: mco_create returned " + std::to_string(res));
        }
      }

      ~CoroutineDetail() {
        if (co != nullptr) {
          const auto state = mco_status(co);
          assert(state == MCO_DEAD || state == MCO_SUSPENDED);
          mco_destroy(co);
        }
        if (destroy_fn_ && func_ptr_) {
          destroy_fn_(func_ptr_);
        }
      }

      CoroutineDetail(const CoroutineDetail&) = delete;
      CoroutineDetail(CoroutineDetail&&) = delete;
      CoroutineDetail& operator=(const CoroutineDetail&) = delete;
      CoroutineDetail& operator=(CoroutineDetail&&) = delete;

      CoroResult Resume() const {
        if (co == nullptr) {
          return CoroResult::kINVALID_COROUTINE;
        }

        if (exception_) {
          return CoroResult::kEXCEPTION;
        }

        if (yield_till > std::chrono::high_resolution_clock::now()) {
          return CoroResult::kYIELDING;
        }

        const mco_result res = mco_resume(co);

        if (exception_) {
          return CoroResult::kEXCEPTION;
        }

        return static_cast<CoroResult>(res);
      }

      void* GetUserData() const {
        return this->user_data;
      }

      mco_coro* GetCoroutine() const {
        return co;
      }

      void Run() {
        if (invoke_fn_ && func_ptr_) {
          invoke_fn_(func_ptr_);
        }
      }

      CoroState GetState() const {
        return static_cast<CoroState>(mco_status(co));
      }

      void SetExceptionThrown(const std::exception_ptr& exception_ptr) {
        exception_ = exception_ptr;
      }

      std::exception_ptr GetExceptionThrown() {
        return exception_;
      }

      void SetYieldTill(const std::chrono::time_point<std::chrono::high_resolution_clock> till) {
        yield_till = till;
      }

    private:
      void* func_ptr_;
      void (*invoke_fn_)(void*);
      void (*destroy_fn_)(void*);
      mco_desc desc_;
      mco_coro* co = nullptr;
      void* user_data = nullptr;
      std::exception_ptr exception_;
      std::chrono::time_point<std::chrono::high_resolution_clock> yield_till;
    };

    inline void CoreExecWrapper(mco_coro* co) {
      try {
        auto* coro = static_cast<CoroutineDetail*>(mco_get_user_data(co));
        if (coro) {
          coro->Run();
        }
      } catch (...) {
        auto* coro = static_cast<CoroutineDetail*>(mco_get_user_data(co));
        if (coro) {
          coro->SetExceptionThrown(std::current_exception());
        }
      }
    }

    namespace this_coro {
      mco_coro* GetCurrentCoro() {
        return mco_running();
      }

      CoroutineDetail* GetCurrentCoroutineDetail() {
        auto* co = GetCurrentCoro();
        if (!co) {
          return nullptr;
        }

        return static_cast<CoroutineDetail*>(mco_get_user_data(co));
      }
    }
  }

  void this_coro::yield() {
    const auto* coro = minicoropp::detail::this_coro::GetCurrentCoroutineDetail();
    if (!coro) {
      std::this_thread::yield();
      return;
    }

    mco_yield(coro->GetCoroutine());
  }


  void* this_coro::get_data() {
    const auto* coro = minicoropp::detail::this_coro::GetCurrentCoroutineDetail();
    if (!coro) {
      return nullptr;
    }

    return coro->GetUserData();
  }

  void* this_coro::get_current_handle() {
    return minicoropp::detail::this_coro::GetCurrentCoroutineDetail();
  }

  void this_coro::suspend_indefinitely() {
    auto* coro = minicoropp::detail::this_coro::GetCurrentCoroutineDetail();
    if (!coro) {
      return; // Or yield thread depending on context
    }

    coro->SetYieldTill(std::chrono::time_point<std::chrono::high_resolution_clock>::max());
    mco_yield(coro->GetCoroutine());
  }

  void this_coro::wake_handle(void* handle) {
    if (!handle) return;
    auto* coro = static_cast<minicoropp::detail::CoroutineDetail*>(handle);
    coro->SetYieldTill(std::chrono::time_point<std::chrono::high_resolution_clock>::min());
  }

  void Coroutine::Init(void* func_ptr, void (*invoke_fn)(void*), void (*destroy_fn)(void*), void* user_data, const std::size_t stack_size) {
    detail_ = new detail::CoroutineDetail(func_ptr, invoke_fn, destroy_fn, user_data, stack_size);
  }

  Coroutine::~Coroutine() {
    delete detail_;
  }

  Coroutine::Coroutine(Coroutine&& other) noexcept : detail_(other.detail_) {
    other.detail_ = nullptr;
  }

  Coroutine& Coroutine::operator=(Coroutine&& other) noexcept {
    if (this != &other) {
      delete detail_;
      detail_ = other.detail_;
      other.detail_ = nullptr;
    }
    return *this;
  }

  CoroResult Coroutine::resume() const {
    if (!detail_) return CoroResult::kINVALID_COROUTINE;
    return detail_->Resume();
  }

  CoroState Coroutine::state() const {
    if (!detail_) return CoroState::kDEAD;
    return detail_->GetState();
  }

  std::exception_ptr Coroutine::exception() const {
    if (!detail_) return nullptr;
    return detail_->GetExceptionThrown();
  }

  namespace this_coro::detail {
    void sleep_for_impl(const std::chrono::high_resolution_clock::duration& sleep_duration) {
      auto* coro = minicoropp::detail::this_coro::GetCurrentCoroutineDetail();
      if (!coro) {
        std::this_thread::sleep_for(sleep_duration);
        return;
      }

      const auto now = std::chrono::high_resolution_clock::now();
      coro->SetYieldTill(now + sleep_duration);
      mco_yield(coro->GetCoroutine());
    }

    void sleep_until_impl(const std::chrono::high_resolution_clock::time_point& sleep_time) {
      auto* coro = minicoropp::detail::this_coro::GetCurrentCoroutineDetail();
      if (!coro) {
        std::this_thread::sleep_until(sleep_time);
        return;
      }

      // Set the exact time point for when this coroutine should resume
      coro->SetYieldTill(sleep_time);
      mco_yield(coro->GetCoroutine());
    }
  }
}
