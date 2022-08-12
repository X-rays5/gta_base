//
// Created by X-ray on 08/12/22.
//

#include "script.hpp"
#include <cassert>

namespace gta_base {
  namespace fiber {

    Script::Script(Script::func_t func) : func_(func) {
      script_fiber_ = CreateFiber(0, [](PVOID param){
        auto this_ = static_cast<Script*>(param);
        this_->FiberFunc();
      }, this);
    }

    Script::~Script() {
      if (script_fiber_)
        DeleteFiber(script_fiber_);
    }

    void Script::Tick() {
      og_fiber_ = GetCurrentFiber();
      if (!wake_at_.has_value() || wake_at_.value() <= common::GetEpoch()) {
        SwitchToFiber(script_fiber_);
      }
    }

    void Script::Yield(std::uint64_t amount) {
      if (amount != 0) {
        wake_at_ = common::GetEpoch() + amount;
      } else {
        wake_at_ = std::nullopt;
      }

      SwitchToFiber(og_fiber_);
    }

    Script* Script::GetCur() {
      return static_cast<Script*>(GetFiberData());
    }

    void Script::ScriptExceptHandler(PEXCEPTION_POINTERS e) {
      HMODULE mod{};
      DWORD64 offset{};
      char buffer[MAX_PATH]{};
      if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)e->ExceptionRecord->ExceptionAddress, &mod) == TRUE && mod != nullptr)
      {
        offset = ((DWORD64)e->ExceptionRecord->ExceptionAddress - (DWORD64)mod);
        GetModuleFileNameA(mod, buffer, MAX_PATH - 1);
      }
      LOG_FATAL << "Exception Code: 0x" << e->ExceptionRecord->ExceptionCode << " Exception Offset: 0x" << offset << " Fault Module Name: " << buffer;
    }

    [[noreturn]] void Script::FiberFunc() {
      __try {
        func_();
      } __except (ScriptExceptHandler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) { }

      []() {
        LOG_INFO << "Script finished!";
      }();

      while(true) {
        Yield();
      }
    }
  }
}