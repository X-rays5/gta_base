//
// Created by X-ray on 02/16/23.
//

#pragma once
#ifndef GTA_BASE_CONTROLLER_5E36818BF3744455BAE8BFD234D389E8_HPP
#define GTA_BASE_CONTROLLER_5E36818BF3744455BAE8BFD234D389E8_HPP
#include <Xinput.h>
#include <robin_hood.h>
#include "../misc/spinlock.hpp"

namespace gta_base::key_input {
  class Controller {
  public:
    enum class INPUT : std::uint32_t {
      NONE = 0,
      A = VK_PAD_A,
      B = VK_PAD_B,
      X = VK_PAD_X,
      Y = VK_PAD_Y,
      RIGHT_SHOULDER = VK_PAD_RSHOULDER,
      LEFT_SHOULDER = VK_PAD_LSHOULDER,
      RIGHT_TRIGGER = VK_PAD_RTRIGGER,
      LEFT_TRIGGER = VK_PAD_LTRIGGER,
      DPAD_UP = VK_PAD_DPAD_UP,
      DPAD_DOWN = VK_PAD_DPAD_DOWN,
      DPAD_LEFT = VK_PAD_DPAD_LEFT,
      DPAD_RIGHT = VK_PAD_DPAD_RIGHT,
      START = VK_PAD_START,
      BACK = VK_PAD_BACK,
      LEFT_THUMB = VK_PAD_LTHUMB_PRESS,
      RIGHT_THUMB = VK_PAD_RTHUMB_PRESS,
    };

    struct KeyState {
      bool down = false;
      std::uint64_t released_at = 0;
    };

  public:
    Controller();
    ~Controller();

    bool IsKeyDown(INPUT key);
    bool KeyJustReleased(INPUT key, std::uint64_t time_released_ms = 100);

    constexpr static inline bool IsControllerInput(std::uint32_t key_id) {
      return magic_enum::enum_contains<INPUT>(key_id);
    }

  private:
    misc::Spinlock lock_;
    std::atomic<bool> running_ = true;
    std::thread controller_state_update_thread_;
    robin_hood::unordered_map<INPUT, KeyState> key_state_;

  private:
    static void UpdateState(Controller* that);
  };
}
#endif //GTA_BASE_CONTROLLER_5E36818BF3744455BAE8BFD234D389E8_HPP
