//
// Created by X-ray on 14/09/2026.
//

#include <gtest/gtest.h>

#include "../../src/ui/text_input/popup.hpp"

namespace {
  using base::menu::ui::text_input::Popup;

  /// What is pinned here is what the rest of the menu deals with - whether the popup is up, which of
  /// its callbacks ran and with what. The window itself, and the keys that close it, are drawn on a
  /// frame the tests have not got a renderer for.
  TEST(text_input_popup, a_popup_that_was_never_opened_is_closed) {
    Popup popup;
    EXPECT_FALSE(popup.IsOpen());
  }

  TEST(text_input_popup, opening_a_popup_puts_it_up) {
    Popup popup;
    popup.Open("title", "text", [](const std::string&) {});
    EXPECT_TRUE(popup.IsOpen());
  }

  TEST(text_input_popup, submit_hands_the_text_to_the_submit_callback) {
    Popup popup;
    std::string submitted;
    popup.Open("title", "hello", [&submitted](const std::string& text) { submitted = text; });

    popup.Submit();

    EXPECT_EQ(submitted, "hello");
  }

  TEST(text_input_popup, what_closes_the_popup_is_also_what_leaves_it_down_for_the_callback) {
    Popup popup;
    bool open_when_called = true;
    popup.Open("title", "hello", [&popup, &open_when_called](const std::string&) { open_when_called = popup.IsOpen(); });

    popup.Submit();

    EXPECT_FALSE(popup.IsOpen());
    EXPECT_FALSE(open_when_called);
  }

  TEST(text_input_popup, cancel_runs_the_cancel_callback_and_not_the_submit_one) {
    Popup popup;
    bool submitted = false;
    bool cancelled = false;
    popup.Open("title", "hello", [&submitted](const std::string&) { submitted = true; }, [&cancelled] { cancelled = true; });

    popup.Cancel();

    EXPECT_TRUE(cancelled);
    EXPECT_FALSE(submitted);
    EXPECT_FALSE(popup.IsOpen());
  }

  TEST(text_input_popup, closing_runs_neither_callback) {
    Popup popup;
    bool called = false;
    popup.Open("title", "hello", [&called](const std::string&) { called = true; }, [&called] { called = true; });

    popup.Close();

    EXPECT_FALSE(called);
    EXPECT_FALSE(popup.IsOpen());
  }

  TEST(text_input_popup, submitting_while_nothing_is_open_does_nothing) {
    Popup popup;
    popup.Submit();
    popup.Cancel();

    EXPECT_FALSE(popup.IsOpen());
  }

  TEST(text_input_popup, a_popup_opened_over_another_replaces_it_without_cancelling_it) {
    Popup popup;
    bool first_cancelled = false;
    std::string submitted;
    popup.Open("title", "first", [](const std::string&) {}, [&first_cancelled] { first_cancelled = true; });

    popup.Open("title", "second", [&submitted](const std::string& text) { submitted = text; });
    popup.Submit();

    EXPECT_FALSE(first_cancelled);
    EXPECT_EQ(submitted, "second");
  }

  TEST(text_input_popup, a_popup_a_callback_opened_is_the_one_left_open) {
    Popup popup;
    bool second_cancelled = false;
    popup.Open("title", "first", [](const std::string&) {}, [&popup, &second_cancelled] {
      popup.Open("title", "second", [](const std::string&) {}, [&second_cancelled] { second_cancelled = true; });
    });

    popup.Cancel();

    EXPECT_TRUE(popup.IsOpen());
    popup.Cancel();
    EXPECT_TRUE(second_cancelled);
    EXPECT_FALSE(popup.IsOpen());
  }

  TEST(text_input_popup, text_longer_than_the_field_holds_is_cut_off) {
    Popup popup;
    std::string submitted;
    popup.Open("title", std::string(Popup::kMaxTextLength + 10, 'a'), [&submitted](const std::string& text) { submitted = text; });

    popup.Submit();

    EXPECT_EQ(submitted.size(), Popup::kMaxTextLength);
  }
}
