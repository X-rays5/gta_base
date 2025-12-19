//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include "native_call_context.hpp"

namespace rage::script {
  class CustomCallContext : public NativeCallContext {
  public:
    constexpr CustomCallContext()
    {
      return_val_ = &return_stack_[0];
      args_ = &arg_stack_[0];
    }

  private:
    uint64_t return_stack_[10];
    uint64_t arg_stack_[40];
  };
}