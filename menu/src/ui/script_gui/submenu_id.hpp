//
// Created by X-ray on 12/09/2026.
//

#pragma once

#include <base-common/concurrency/spinlock.hpp>

#include <cstdint>
#include <set>
#include <string>

namespace base::menu::ui::script_gui {
  /**
   * Hands out the ids by which the renderer knows a script's submenus.
   *
   * A submenu a script makes is not one of the renderer's own - it has no SubmenuIDs value and never
   * will - so it needs a key of its own, and the key has to be one no built-in enchantum name can
   * collide with. Numbering from a billion is that: every id is unique for the life of the process,
   * and the renderer's map can hold script keys and enum names side by side without either being able
   * to name the other.
   *
   * Ids only go up. A reloaded script gets a new one rather than the one it had, which is the whole
   * reason the number is handed out here instead of being derived from anything the script holds: an
   * id is not something a script may keep, and a script that forgets one and asks again is asking for
   * a page it does not have.
   */
  class ScriptSubmenuIdAllocator {
  public:
    /// Where numbering starts. High enough that no SubmenuIDs value and no id of anything else here
    /// will ever reach it, so an id alone says which kind of submenu it names.
    static constexpr std::uint64_t kFIRST_ID = 1'000'000'000;

    /**
     * The next id, which is never one that was handed out before - not even one that has since been
     * released. Reuse would make a stale handle name a live page, and a script holding an id across
     * a reload is exactly the case this is built to make impossible.
     */
    [[nodiscard]] std::uint64_t Acquire();

    /**
     * Gives an id back. Recorded as no longer outstanding, so IsOutstanding answers for a page that
     * is gone, but never re-handed-out.
     */
    void Release(std::uint64_t id);

    /// Whether `id` was handed out by this allocator and has not been released since.
    [[nodiscard]] bool IsOutstanding(std::uint64_t id) const;

    /// How many ids are currently outstanding. Diagnostics and tests.
    [[nodiscard]] std::size_t Outstanding() const;

    /**
     * The renderer's key for `id`.
     *
     * Namespaced because the renderer's other keys are enchantum names of SubmenuIDs, and an id on
     * its own would be a key that could collide with one an enum spelling happens to produce. The
     * prefix is this class's business so that every caller - the registry that registers the page,
     * the component that navigates to it, the code that removes it - spells it the same way.
     */
    [[nodiscard]] static std::string ToRendererKey(std::uint64_t id);

  private:
    mutable common::concurrency::Spinlock lock_;
    std::uint64_t next_ = kFIRST_ID;
    std::set<std::uint64_t> outstanding_;
  };
}
