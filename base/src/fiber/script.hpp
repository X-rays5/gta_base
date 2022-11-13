//
// Created by X-ray on 08/12/22.
//

#pragma once
#ifndef GTA_BASE_SCRIPT_HPP
#define GTA_BASE_SCRIPT_HPP
#include <optional>
#undef Yield

namespace gta_base::fiber {
    class Script {
    public:
      using func_t = void(*)();

    public:
      explicit Script(func_t func);
      ~Script();

      void Tick();
      void Yield(std::uint64_t amount = 0);
      static Script* GetCurr();
      static void ScriptExceptHandler(PEXCEPTION_POINTERS e);

    private:
      LPVOID og_fiber_ = nullptr;
      LPVOID script_fiber_;
      func_t func_;
      std::optional<std::int64_t> wake_at_;

    private:
      [[noreturn]] void FiberFunc();
    };
  }
#endif //GTA_BASE_SCRIPT_HPP
