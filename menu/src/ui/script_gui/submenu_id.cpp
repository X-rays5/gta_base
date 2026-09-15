//
// Created by X-ray on 12/09/2026.
//

#include "submenu_id.hpp"

#include <fmt/format.h>

namespace base::menu::ui::script_gui {
  std::uint64_t ScriptSubmenuIdAllocator::Acquire() {
    common::concurrency::ScopedSpinlock lock(lock_);
    const std::uint64_t id = next_++;
    outstanding_.insert(id);
    return id;
  }

  void ScriptSubmenuIdAllocator::Release(const std::uint64_t id) {
    common::concurrency::ScopedSpinlock lock(lock_);
    // Deliberately not stepping next_ back: see the class comment. A released id is spent.
    outstanding_.erase(id);
  }

  bool ScriptSubmenuIdAllocator::IsOutstanding(const std::uint64_t id) const {
    common::concurrency::ScopedSpinlock lock(lock_);
    return outstanding_.contains(id);
  }

  std::size_t ScriptSubmenuIdAllocator::Outstanding() const {
    common::concurrency::ScopedSpinlock lock(lock_);
    return outstanding_.size();
  }

  std::string ScriptSubmenuIdAllocator::ToRendererKey(const std::uint64_t id) {
    return fmt::format("script/{}", id);
  }
}
