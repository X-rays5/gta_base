//
// Created by X-ray on 02/17/23.
//

#pragma once
#ifndef GTA_BASE_COOLDOWN_INPUT_D4B8CE1CF13F4939B035EE1314A737A6_HPP
#define GTA_BASE_COOLDOWN_INPUT_D4B8CE1CF13F4939B035EE1314A737A6_HPP
namespace gta_base::key_input {
  class CooldownInput {
  public:
    CooldownInput(std::uint32_t key_bind_id, std::uint64_t timeout) : key_bind_id_(key_bind_id), timeout_(timeout) {}

    bool Get();

  private:
    std::uint32_t key_bind_id_;
    std::uint64_t last_input_ = 0;
    std::uint64_t timeout_;
  };
}
#endif //GTA_BASE_COOLDOWN_INPUT_D4B8CE1CF13F4939B035EE1314A737A6_HPP
