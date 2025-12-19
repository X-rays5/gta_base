//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <rage/script/custom_call_context.hpp>

namespace base::menu::natives {
  class Invoker {
  public:
    class NativeCall {
    public:

      template<typename T>
      void PushArg(T&& value) {
        context_.PushArg(std::forward<T>(value));
      }

#pragma warning(push)
#pragma warning(disable: 4100)
      void EndCall(const std::uint64_t native_hash)
      {
          context_.FixVectors();
      }
#pragma warning(pop)

      template<typename T>
      T& GetReturnValue()
      {
        return *context_.GetReturnValue<T>();
      }

    private:
      ::rage::script::CustomCallContext context_{};
    };

  public:

    template<typename Ret, std::uint64_t native_hash, typename... Args>
    static Ret Invoke(Args... args) {
      NativeCall call;
      (call.PushArg(std::forward<Args>(args)), ...);
      call.EndCall(native_hash);

      if constexpr (!std::is_same_v<Ret, void>) {
        return call.GetReturnValue<Ret>();
      } else {
        return;
      }
    }
  };
}