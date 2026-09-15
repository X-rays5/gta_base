//
// Created by X-ray on 19/12/2025.
//

#pragma once
#include <cstddef>
#include "native_call_context.hpp"

namespace rage::script {
  class CustomCallContext : public NativeCallContext {
  public:
    /// The argument stack the game is handed, and the whole of the bound on a call: PushArg writes where
    /// it is told and checks nothing. Anything sizing itself by how many arguments a call can carry has
    /// to read the number from here rather than restate it.
    static constexpr std::size_t kMAX_ARGS = 40;

    constexpr CustomCallContext()
    {
      reset();
      return_val_ = &return_stack_[0];
      args_ = &arg_stack_[0];
    }

  private:
    uint64_t return_stack_[10];
    uint64_t arg_stack_[kMAX_ARGS];
  };
}