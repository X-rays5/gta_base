//
// Created by X-ray on 08/22/22.
//

#pragma once
#ifndef GTA_BASE_INVOKER_HPP
#define GTA_BASE_INVOKER_HPP
#include <robin_hood.h>
#include <script/scrNativeHandler.hpp>

namespace rage {
  class NativeCallContext : public rage::scrNativeCallContext {
  public:
    NativeCallContext();
  private:
    std::uint64_t return_stack_[10];
    std::uint64_t arg_stack_[100];
  };

  class Invoker {
  public:
    void CacheHandlers();
    void BeginCall();
    void EndCall(rage::scrNativeHash hash);

    template<typename T>
    void PushArg(T&& val) {
      call_context_.push_arg(std::forward<T>(val));
    }

    template<typename T>
    T& GetRetVal() {
      return *call_context_.get_return_value<T>();
    }

  private:
    NativeCallContext call_context_;
    robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> handler_cache_;
  };

  inline Invoker kINVOKER;
}

template<typename Ret, typename ...Args>
__forceinline Ret invoke(rage::scrNativeHash hash, Args&& ...args) {
  using namespace rage;

  kINVOKER.BeginCall();
  (kINVOKER.PushArg(std::forward<Args>(args)), ...);
  kINVOKER.EndCall(hash);

  if constexpr (!std::is_same_v<Ret, void>) {
    return kINVOKER.GetRetVal<Ret>();
  }
}

#endif //GTA_BASE_INVOKER_HPP
