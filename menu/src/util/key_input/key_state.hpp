//
// Created by X-ray on 01/06/2025.
//

#ifndef KEY_STATE_HPP_06162834
#define KEY_STATE_HPP_06162834
#include <chrono>
#include <ankerl/unordered_dense.h>
#include "key_event_listener.hpp"

namespace base::menu::util {
  /**
   * A class that registers the state of a set of keys and provides methods to check if a key was pressed or is currently down.
   */
  class KeyState final : public KeyEventListener {
  public:
    struct KeyStateData {
      bool is_down = false;
      bool was_pressed = false;
      std::chrono::steady_clock::time_point last_check_time{};
    };

  public:
    /**
     * 
     * @param keys
     * @param cooldown_ms
     */
    KeyState(const std::vector<std::uint32_t>& keys, std::chrono::milliseconds cooldown_ms);
    ~KeyState() override;

    /**
     * Checks if a key was pressed since the last check and the cooldown period has passed.
     * @param key_code Virtual key code of the key to check.
     * @return True if the key was pressed since the last check, and the cooldown period has passed.
     */
    bool WasKeyPressed(std::uint32_t key_code);

    /**
     * Checks if a key is currently down and the cooldown period has passed.
     * @param key_code Virtual key code of the key to check.
     * @return True if the key is currently down and the cooldown period has passed.
     */
    bool IsKeyDown(std::uint32_t key_code);

    /**
     * Checks if the specified key is currently down and the cooldown period has passed.
     * @param vk_key Virtual key code of the key to check.
     * @param modifier Modifier key that was pressed along with the key, if any.
     */
    void KeyDown(std::uint32_t vk_key, ModifierKey modifier) override;

    /**
     * Checks if the specified key was released, and the cooldown period has passed.
     * @param vk_key Virtual key code of the key to check.
     * @param modifier Modifier key that was pressed along with the key, if any.
     */
    void KeyUp(std::uint32_t vk_key, ModifierKey modifier) override;

  private:
    common::concurrency::Spinlock spinlock_;
    std::size_t key_event_handler_id_{};
    std::chrono::milliseconds cooldown_ms_{};
    ankerl::unordered_dense::map<std::uint32_t, KeyStateData> key_registry_;
  };
}

#endif //KEY_STATE_HPP_06162834
