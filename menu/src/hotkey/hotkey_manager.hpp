//
// Created by X-ray on 28/03/2026.
//

#pragma once
#include <optional>
#include <ankerl/unordered_dense.h>
#include "../util/key_input/key_event_listener.hpp"

namespace base::menu::options {
  class BaseOption;
}

namespace base::menu::hotkey {
  union Hotkey {
    struct {
      std::uint32_t vk_key;
      util::KeyEventListener::ModifierKey modifier;
    };
    std::uint64_t combined;

    Hotkey() : combined(NULL) {}
    Hotkey(const std::uint32_t vk, const util::KeyEventListener::ModifierKey mod) : vk_key(vk), modifier(mod) {}
    explicit Hotkey(const std::uint32_t vk) : vk_key(vk), modifier(util::KeyEventListener::ModifierKey::kNONE) {}
    explicit Hotkey(const std::uint64_t combined) : combined(combined) {}

    std::string AsString() const noexcept {
      const std::string key_str = win32::VkIdToString(vk_key);
      const std::string mod = modifier == util::KeyEventListener::ModifierKey::kSHIFT ? "Shift + " :
                        modifier == util::KeyEventListener::ModifierKey::kCTRL ? "Ctrl + " : "";

      return mod + key_str;
    }

    bool operator==(const Hotkey& that) const {
      return combined == that.combined;
    }
  };
}

template <>
  struct std::hash<base::menu::hotkey::Hotkey> {
  std::size_t operator()(const base::menu::hotkey::Hotkey& s) const noexcept {
    return std::hash<std::uint64_t>{}(s.combined);
  }
};

namespace base::menu::hotkey {
  class HotkeyManager : public util::KeyEventListener {
  public:
    HotkeyManager();
    ~HotkeyManager();

    void AddNewHotkey(std::shared_ptr<options::BaseOption> option);
    void RemoveHotkey(const Hotkey& hotkey);

    std::vector<std::pair<Hotkey, std::shared_ptr<options::BaseOption>>> GetAllHotkeys() const;

    /**
     * The key `option_name` is currently bound to, or nothing when it is bound to none.
     *
     * The map is keyed the other way round, so this is a scan of it. That is deliberate rather than a
     * second map kept in step: the only callers are the ones drawing a frame, and the scan is over one
     * entry per key the player has actually bound, which is a handful at worst - a second map would be
     * state to keep consistent for a lookup nobody does often enough to notice.
     */
    [[nodiscard]] std::optional<Hotkey> GetHotkeyForOption(const std::string& option_name) const;

    virtual void KeyDown(std::uint32_t vk_key, ModifierKey modifier) override;
    virtual void KeyUp(std::uint32_t vk_key, ModifierKey modifier) override;

  private:
    const std::size_t key_event_listener_id_;
    ankerl::unordered_dense::map<Hotkey, std::shared_ptr<options::BaseOption>> key_opt_map_;
    std::atomic<bool> is_adding_hotkey_ = false;
    mutable common::concurrency::RecursiveSpinlock add_hotkey_lock_;
    win32::Signal new_hotkey_signal_;
    Hotkey new_hotkey_{};

  private:
    void KeyForRegistration(std::uint32_t vk_key, ModifierKey modifier);

    void Save();
    void Load();
  };

  inline HotkeyManager* kHOTKEY_MANAGER{};
}
