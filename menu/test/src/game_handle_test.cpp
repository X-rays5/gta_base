//
// Created by X-ray on 16/09/2026.
//

#include <gtest/gtest.h>

// The menu's headers are written against its precompiled header, so the base-common headers go first,
// as the other tests here do.
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../../src/game/blip.hpp"
#include "../../src/game/entity.hpp"
#include "../../src/game/handles.hpp"
#include "../../src/game/native_types.hpp"
#include "../../src/game/ped.hpp"
#include "../../src/game/player.hpp"
#include "../../src/game/vehicle.hpp"

namespace game = base::menu::game;

// A handle is a 32 bit value and nothing else - asserted in handles.hpp too, and here because it is the
// one thing about these types that must not change quietly: the generated natives are declared with
// them, and the script bindings spell every handle `int`.
TEST(game_handle, a_handle_is_the_int_the_natives_are_declared_with) {
  static_assert(sizeof(game::ScrHandle) == sizeof(std::int32_t));
  static_assert(sizeof(game::Entity) == sizeof(std::int32_t));
  static_assert(sizeof(game::Ped) == sizeof(std::int32_t));
  static_assert(sizeof(game::Vehicle) == sizeof(std::int32_t));
  static_assert(sizeof(game::Object) == sizeof(std::int32_t));
  static_assert(sizeof(game::Player) == sizeof(std::int32_t));
  static_assert(sizeof(game::Blip) == sizeof(std::int32_t));
  static_assert(sizeof(game::Cam) == sizeof(std::int32_t));
  static_assert(sizeof(game::Pickup) == sizeof(std::int32_t));
  static_assert(sizeof(game::Interior) == sizeof(std::int32_t));
  static_assert(sizeof(game::FireId) == sizeof(std::int32_t));

  static_assert(std::is_trivially_copyable_v<game::ScrHandle>);
  static_assert(std::is_trivially_copyable_v<game::Ped>);
  static_assert(std::is_trivially_copyable_v<game::Blip>);
  static_assert(std::is_standard_layout_v<game::Ped>);
  static_assert(std::is_standard_layout_v<game::Vehicle>);
}

TEST(game_handle, a_handle_holds_the_identity_it_was_given) {
  const game::Ped ped{42};
  EXPECT_EQ(ped.Get(), 42);
  EXPECT_TRUE(ped.IsValid());
  EXPECT_EQ(static_cast<std::int32_t>(ped), 42);

  const game::Ped none{};
  EXPECT_EQ(none.Get(), 0);
  EXPECT_FALSE(none.IsValid());
}

TEST(game_handle, a_ped_is_an_entity_and_anything_is_a_script_handle) {
  const game::Ped ped{7};

  // What a native asking for an Entity, or for a plain script handle, is handed.
  const game::Entity entity = ped;
  const game::ScrHandle handle = ped;
  EXPECT_EQ(entity.Get(), 7);
  EXPECT_EQ(handle.Get(), 7);

  static_assert(std::is_base_of_v<game::Entity, game::Ped>);
  static_assert(std::is_base_of_v<game::Entity, game::Vehicle>);
  static_assert(std::is_base_of_v<game::ScrHandle, game::Player>);
  static_assert(std::is_base_of_v<game::ScrHandle, game::Blip>);
  static_assert(std::is_base_of_v<game::ScrHandle, game::Ped>);
  static_assert(!std::is_base_of_v<game::Vehicle, game::Ped>);
  static_assert(!std::is_base_of_v<game::Ped, game::Entity>);
}

TEST(game_handle, a_handle_reads_as_the_number_it_is) {
  EXPECT_EQ(fmt::format("{}", game::Ped{42}), "42");
  EXPECT_EQ(fmt::format("ped {}", game::Entity{7}), "ped 7");
  EXPECT_EQ(fmt::format("{}", game::Vehicle{0}), "0");
  EXPECT_EQ(fmt::format("{}", game::Blip{123456}), "123456");
}

TEST(game_handle, a_handle_compares_and_sorts_as_the_number_it_is) {
  // No comparison operator is declared - the conversion to the identity is the only candidate, so
  // these are the built-in ones at work. Declaring a defaulted operator== next to it is what would
  // turn the same expression into an ambiguity.
  EXPECT_TRUE(game::Ped{1} == game::Ped{1});
  EXPECT_TRUE(game::Ped{1} != game::Ped{2});
  EXPECT_TRUE(game::Ped{1} == 1);
  EXPECT_TRUE(game::Ped{1} < game::Ped{2});

  const std::map<game::Ped, std::string> names{{game::Ped{2}, "two"}, {game::Ped{1}, "one"}};
  EXPECT_EQ(names.begin()->second, "one");
  EXPECT_EQ(names.rbegin()->second, "two");
}

TEST(game_handle, a_handle_takes_a_format_specification) {
  EXPECT_EQ(fmt::format("{:d}", game::Ped{42}), "42");
  EXPECT_EQ(fmt::format("{:06d}", game::Ped{42}), "000042");
}

TEST(game_handle, a_handle_can_key_a_hash_map) {
  std::unordered_map<game::Ped, std::string> names;
  names[game::Ped{1}] = "one";
  names[game::Ped{2}] = "two";

  EXPECT_EQ(names.size(), 2U);
  EXPECT_EQ(names[game::Ped{1}], "one");
  EXPECT_EQ(names.at(game::Ped{2}), "two");
  EXPECT_EQ(names.count(game::Ped{3}), 0U);
}
