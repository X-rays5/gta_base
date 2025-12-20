//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <rage/script/custom_call_context.hpp>
#include "crossmap.hpp"

namespace base::menu::natives {
  class Invoker {
  public:
    class NativeCall {
    public:

      template<typename T>
      void PushArg(T&& value) {
        context_.PushArg(std::forward<T>(value));
      }

      void EndCall(std::uint64_t native_idx);

      template<typename T>
      T& GetReturnValue()
      {
        return *context_.GetReturnValue<T>();
      }

    private:
      ::rage::script::CustomCallContext context_{};
    };

  public:
    Invoker();
    ~Invoker();

    template<typename Ret, std::uint64_t native_idx, typename... Args>
    static Ret Invoke(Args... args) {
      NativeCall call;
      (call.PushArg(std::forward<Args>(args)), ...);
      call.EndCall(native_idx);

      if constexpr (!std::is_same_v<Ret, void>) {
        return call.GetReturnValue<Ret>();
      } else {
        return;
      }
    }

  private:
    std::array<rage::script::NativeHandler, NATIVE_CROSSMAP_SIZE> native_handlers_{};
  };
  inline Invoker* kINVOKER{};
}