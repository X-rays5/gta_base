//
// Created by X-ray on 02/16/23.
//

#pragma once
#ifndef GTA_BASE_KEYBOARD_C511E2207E9C42858F7669CC7C131947_HPP
#define GTA_BASE_KEYBOARD_C511E2207E9C42858F7669CC7C131947_HPP
#include <robin_hood.h>
#include "../misc/spinlock.hpp"

#undef DELETE

namespace gta_base::key_input {
  class Keyboard {
  public:
    struct KeyState {
      bool down = false;
      std::uint64_t released_at = 0;
    };

    using vk_key_t = std::uint32_t;

    enum class INPUT : std::uint32_t {
      LEFT_MOUSE = VK_LBUTTON,
      RIGHT_MOUSE = VK_RBUTTON,
      MIDDLE_MOUSE = VK_MBUTTON,
      PAUSE_BREAK = VK_CANCEL,
      FORWARD_MOUSE = VK_XBUTTON1,
      BACKWARD_MOUSE = VK_XBUTTON2,
      BACKSPACE = VK_BACK,
      TAB = VK_TAB,
      RETURN = VK_RETURN,
      SHIFT = VK_SHIFT,
      LEFT_SHIFT = VK_LSHIFT,
      RIGHT_SHIFT = VK_RSHIFT,
      CONTROL = VK_CONTROL,
      LEFT_CONTROL = VK_LCONTROL,
      RIGHT_CONTROL = VK_RCONTROL,
      ALT = VK_MENU,
      LEFT_ALT = VK_LMENU,
      RIGHT_ALT = VK_RMENU,
      PAUSE = VK_PAUSE,
      CAPS_LOCK = VK_CAPITAL,
      ESCAPE = VK_ESCAPE,
      SPACE = VK_SPACE,
      PAGE_UP = VK_PRIOR,
      PAGE_DOWN = VK_NEXT,
      END = VK_END,
      HOME = VK_HOME,
      LEFT_ARROW = VK_LEFT,
      UP_ARROW = VK_UP,
      RIGHT_ARROW = VK_RIGHT,
      DOWN_ARROW = VK_DOWN,
      SELECT = VK_SELECT,
      SNAPSHOT = VK_SNAPSHOT,
      INSERT = VK_INSERT,
      DELETE = VK_DELETE,
      KEY_0 = 0x30,
      KEY_1 = 0x31,
      KEY_2 = 0x32,
      KEY_3 = 0x33,
      KEY_4 = 0x34,
      KEY_5 = 0x35,
      KEY_6 = 0x36,
      KEY_7 = 0x37,
      KEY_8 = 0x38,
      KEY_9 = 0x39,
      KEY_A = 0x41,
      KEY_B = 0x42,
      KEY_C = 0x43,
      KEY_D = 0x44,
      KEY_E = 0x45,
      KEY_F = 0x46,
      KEY_G = 0x47,
      KEY_H = 0x48,
      KEY_I = 0x49,
      KEY_J = 0x4A,
      KEY_K = 0x4B,
      KEY_L = 0x4C,
      KEY_M = 0x4D,
      KEY_N = 0x4E,
      KEY_O = 0x4F,
      KEY_P = 0x50,
      KEY_Q = 0x51,
      KEY_R = 0x52,
      KEY_S = 0x53,
      KEY_T = 0x54,
      KEY_U = 0x55,
      KEY_V = 0x56,
      KEY_W = 0x57,
      KEY_X = 0x58,
      KEY_Y = 0x59,
      KEY_Z = 0x5A,
      NUMPAD_0 = VK_NUMPAD0,
      NUMPAD_1 = VK_NUMPAD1,
      NUMPAD_2 = VK_NUMPAD2,
      NUMPAD_3 = VK_NUMPAD3,
      NUMPAD_4 = VK_NUMPAD4,
      NUMPAD_5 = VK_NUMPAD5,
      NUMPAD_6 = VK_NUMPAD6,
      NUMPAD_7 = VK_NUMPAD7,
      NUMPAD_8 = VK_NUMPAD8,
      NUMPAD_9 = VK_NUMPAD9,
      MULTIPLY = VK_MULTIPLY,
      ADD = VK_ADD,
      SEPARATOR = VK_SEPARATOR,
      SUBTRACT = VK_SUBTRACT,
      DECIMAL = VK_DECIMAL,
      DIVIDE = VK_DIVIDE,
      F1 = VK_F1,
      F2 = VK_F2,
      F3 = VK_F3,
      F4 = VK_F4,
      F5 = VK_F5,
      F6 = VK_F6,
      F7 = VK_F7,
      F8 = VK_F8,
      F9 = VK_F9,
      F10 = VK_F10,
      F11 = VK_F11,
      F12 = VK_F12,
      F13 = VK_F13,
      F14 = VK_F14,
      F15 = VK_F15,
      F16 = VK_F16,
      F17 = VK_F17,
      F18 = VK_F18,
      F19 = VK_F19,
      F20 = VK_F20,
      F21 = VK_F21,
      F22 = VK_F22,
      F23 = VK_F23,
      F24 = VK_F24,
    };

  public:
    Keyboard();
    ~Keyboard();

    bool IsKeyDown(INPUT key);
    bool KeyJustReleased(INPUT key, std::uint64_t time_released_ms = 100);

    constexpr static inline bool IsKeyboardInput(std::uint32_t key_id) {
      return magic_enum::enum_contains<INPUT>(key_id);
    }

  private:
    misc::Spinlock lock_;
    robin_hood::unordered_map<INPUT, KeyState> key_state_;

  private:
    void SetKeyState(vk_key_t key, bool state);

    static void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  };

  inline Keyboard* kKEYBOARD{};
}
#endif //GTA_BASE_KEYBOARD_C511E2207E9C42858F7669CC7C131947_HPP
