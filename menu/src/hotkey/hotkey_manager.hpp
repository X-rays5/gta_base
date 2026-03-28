//
// Created by X-ray on 28/03/2026.
//

#pragma once
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
    virtual void KeyDown(std::uint32_t vk_key, ModifierKey modifier) override;
    virtual void KeyUp(std::uint32_t vk_key, ModifierKey modifier) override;

  private:
    const std::size_t key_event_listener_id_;
    ankerl::unordered_dense::map<Hotkey, std::shared_ptr<options::BaseOption>> key_opt_map_;
    std::atomic<bool> is_adding_hotkey_ = false;
    common::concurrency::RecursiveSpinlock add_hotkey_lock_;
    win32::Signal new_hotkey_signal_;
    Hotkey new_hotkey_{};

  private:
    void KeyForRegistration(std::uint32_t vk_key, ModifierKey modifier);

    void Save();
    void Load();
  };

  inline HotkeyManager* kHOTKEY_MANAGER{};
}
