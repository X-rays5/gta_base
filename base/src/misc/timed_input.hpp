//
// Created by X-ray on 07/02/22.
//

#pragma once
#ifndef GTA_BASE_TIMED_INPUT_HPP
#define GTA_BASE_TIMED_INPUT_HPP

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

  class ModifierTimedInput {
  public:
    ModifierTimedInput(int modifier, int input, std::uint64_t timeout) : modifier_(modifier), input_(input), timeout_(timeout) {}

    bool Get();

  private:

    int modifier_;
    int input_;
    std::uint64_t last_input_ = NULL;
    std::uint64_t timeout_;
  };
}
#endif //GTA_BASE_TIMED_INPUT_HPP