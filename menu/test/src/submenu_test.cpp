//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

// submenu.hpp is written against the menu precompiled header: it names LOG_ERROR, and through
// GetName it names the localization manager, which is itself written against the pch's Status.
// A test is not that translation unit, so the three names are brought in here instead.
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>
#include "../../src/ui/localization/manager.hpp"

#include "../../src/ui/submenu.hpp"

#include <utility>

// Submenu holds its two callbacks type-erased, and both are optional. The three claims worth
// pinning are that the open callback is what runs on OnOpened, that its absence is not a call into
// nothing, and that it survives the move into the renderer: every submenu reaches its stored form
// through AddSubmenu, so a callback the move constructor forgets is a callback the menu never has.
namespace {
  using base::menu::ui::Submenu;

  TEST(submenu, the_open_callback_runs_each_time_the_submenu_is_opened) {
    int opened = 0;
    Submenu submenu("ui/sub/test", [](Submenu*) {}, [&opened](Submenu* sub) {
      EXPECT_NE(sub, nullptr) << "The callback should be handed the submenu that opened.";
      ++opened;
    });

    EXPECT_EQ(opened, 0) << "Constructing a submenu does not open it.";

    submenu.OnOpened();
    submenu.OnOpened();

    EXPECT_EQ(opened, 2) << "A submenu opened twice should run its callback twice.";
  }

  TEST(submenu, a_submenu_without_an_open_callback_is_still_openable) {
    // Not merely "does not throw": an empty Callback invoked directly is undefined, so the guard in
    // OnOpened is what makes the common case of no open callback well defined at all.
    Submenu submenu("ui/sub/test", [](Submenu*) {});

    submenu.OnOpened();
  }

  TEST(submenu, moving_a_submenu_carries_both_callbacks_over) {
    int opened = 0;
    int updated = 0;
    Submenu source("ui/sub/test", [&updated](Submenu*) { ++updated; },
                   [&opened](Submenu*) { ++opened; });

    Submenu moved(std::move(source));

    moved.OnOpened();
    EXPECT_EQ(opened, 1) << "The open callback should have moved with the submenu.";

    moved.UpdateComponents();
    EXPECT_EQ(updated, 1) << "The update callback should have moved with the submenu.";
  }
}
