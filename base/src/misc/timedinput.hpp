//
// Created by X-ray on 07/02/22.
//

#pragma once
#ifndef GTA_BASE_TIMEDINPUT_HPP
#define GTA_BASE_TIMEDINPUT_HPP
#include <cstdint>

namespace gta_base::util {
    class TimedInput {
    public:
      TimedInput(int input, std::uint64_t timeout) : input_(input), timeout_(timeout) {}

      bool Get();

    private:

      int input_;
      std::uint64_t last_input_ = NULL;
      std::uint64_t timeout_;
    };
  }
#endif //GTA_BASE_TIMEDINPUT_HPP