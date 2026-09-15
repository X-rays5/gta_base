//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

// The menu's headers are written against its precompiled header and name things like LOG_ERROR
// without including them, so the base-common headers go first, as the other tests here do.
#include <base-common/logging/logging_macro.hpp>

#include "../../src/ui/script_gui/submenu_id.hpp"

#include <algorithm>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

// The ids a script's pages are known by, which is the one thing about them that has to be true for
// everything else to be: an id is never handed out twice, so nothing can be keyed by one and outlive
// what it names. Numbering starts above every id the renderer's own submenus have, which is what lets
// the renderer tell a script's page from one of its own by the key alone.
namespace {
  using base::menu::ui::script_gui::ScriptSubmenuIdAllocator;

  TEST(script_submenu_id, the_first_id_is_the_first_script_id) {
    ScriptSubmenuIdAllocator ids;

    // Not merely "some number": 1'000'000'000 is above every SubmenuIDs value there is, and a first id
    // below that would be a script page registering itself over one of the menu's own.
    EXPECT_EQ(ids.Acquire(), ScriptSubmenuIdAllocator::kFIRST_ID);
    EXPECT_EQ(ScriptSubmenuIdAllocator::kFIRST_ID, 1'000'000'000ULL);
  }

  TEST(script_submenu_id, ids_are_monotonic_and_a_released_one_is_never_handed_out_again) {
    ScriptSubmenuIdAllocator ids;

    const std::uint64_t first = ids.Acquire();
    const std::uint64_t second = ids.Acquire();
    const std::uint64_t third = ids.Acquire();

    EXPECT_LT(first, second);
    EXPECT_LT(second, third);

    EXPECT_TRUE(ids.IsOutstanding(second));
    ids.Release(second);
    EXPECT_FALSE(ids.IsOutstanding(second)) << "a released id should no longer be outstanding";

    // The point of the whole class: the id of a page that is gone must never name the next page. A
    // script that kept an id across a reload would otherwise be looking at somebody else's page.
    const std::uint64_t fourth = ids.Acquire();
    EXPECT_EQ(fourth, third + 1) << "the allocator should keep counting rather than fill the hole";
    EXPECT_NE(fourth, second);
    EXPECT_FALSE(ids.IsOutstanding(second));
  }

  TEST(script_submenu_id, outstanding_counts_only_what_was_not_released) {
    ScriptSubmenuIdAllocator ids;
    EXPECT_EQ(ids.Outstanding(), 0U);

    const std::uint64_t first = ids.Acquire();
    const std::uint64_t second = ids.Acquire();
    EXPECT_EQ(ids.Outstanding(), 2U);

    ids.Release(first);
    EXPECT_EQ(ids.Outstanding(), 1U);

    // Releasing what was never handed out, or releasing twice, is not a way to go below zero: an id
    // that is not outstanding is a no-op rather than a second decrement.
    ids.Release(first);
    ids.Release(ScriptSubmenuIdAllocator::kFIRST_ID - 1);
    EXPECT_EQ(ids.Outstanding(), 1U);

    ids.Release(second);
    EXPECT_EQ(ids.Outstanding(), 0U);
  }

  TEST(script_submenu_id, an_id_is_not_outstanding_before_it_is_handed_out) {
    ScriptSubmenuIdAllocator ids;

    EXPECT_FALSE(ids.IsOutstanding(ScriptSubmenuIdAllocator::kFIRST_ID));
    EXPECT_FALSE(ids.IsOutstanding(0));

    EXPECT_EQ(ids.Acquire(), ScriptSubmenuIdAllocator::kFIRST_ID);
    EXPECT_TRUE(ids.IsOutstanding(ScriptSubmenuIdAllocator::kFIRST_ID));
  }

  TEST(script_submenu_id, the_renderer_key_is_distinct_per_id_and_namespaced) {
    const std::string first = ScriptSubmenuIdAllocator::ToRendererKey(1'000'000'000);
    const std::string second = ScriptSubmenuIdAllocator::ToRendererKey(1'000'000'001);

    EXPECT_NE(first, second) << "two ids must not produce one key";
    EXPECT_EQ(ScriptSubmenuIdAllocator::ToRendererKey(1'000'000'000), first) << "the key must be stable";
    EXPECT_EQ(first, "script/1000000000") << "the key is the id under a namespace of its own";
    EXPECT_EQ(first.rfind("script/", 0), 0U) << "the key must be namespaced, so that no enchantum spelling "
                                                "of a SubmenuIDs value can produce it";
  }

  TEST(script_submenu_id, two_threads_acquiring_at_once_never_share_an_id) {
    ScriptSubmenuIdAllocator ids;
    std::vector<std::uint64_t> taken(64);
    std::vector<std::thread> threads;

    for (std::size_t i = 0; i < taken.size(); ++i) {
      threads.emplace_back([&ids, &taken, i] { taken[i] = ids.Acquire(); });
    }
    for (auto& thread : threads) {
      thread.join();
    }

    // A page is made from wherever a script runs, so two scripts loading at once is an ordinary case
    // rather than a race to be documented away. Sorted, the ids are the first sixty-four in order.
    std::ranges::sort(taken);
    for (std::size_t i = 0; i < taken.size(); ++i) {
      EXPECT_EQ(taken[i], ScriptSubmenuIdAllocator::kFIRST_ID + i);
    }
  }
}
