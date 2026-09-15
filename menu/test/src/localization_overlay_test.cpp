//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

// The menu's headers are written against its precompiled header and name things like LOG_ERROR
// without including them, so the base-common headers go first, as the other tests here do.
#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <random>
#include <string>

#include "../../src/ui/localization/manager.hpp"

// The strings a script registers, which are kept structurally apart from the translation the menu
// loads off disk - so far apart that no writer has to remember not to write them. That separation is
// the whole point of the overlay, and it is what the last test here pins: whatever a script registers,
// the file Save writes does not contain it.
namespace {
  using base::menu::ui::localization::Manager;

  /// A name unique across processes, so that two tests running at once cannot write each other's
  /// files: ctest discovers one entry per test and is free to run those in parallel.
  std::string UniqueName(const std::string& base) {
    static std::atomic<int> counter{0};
    return base + "_" + std::to_string(std::random_device{}()) + "_" + std::to_string(counter.fetch_add(1));
  }

  /// One of the keys the menu ships, taken from the default translation the manager loads: a script
  /// asking for this one is asking for a key that is already the menu's.
  constexpr const char* kBuiltinKey = "label/ok";

  /// A translation file of the working directory, removed with the test that made it. A script's
  /// strings are never in one, which is what the last test asserts against these bytes.
  class TranslationFile {
  public:
    explicit TranslationFile(const std::string& name)
        : path_(std::filesystem::path(base::common::fs::vfs::GetTranslationDir()) / (name + ".json")) {}

    ~TranslationFile() {
      std::error_code ec;
      std::filesystem::remove(path_, ec);
    }

    TranslationFile(const TranslationFile&) = delete;
    TranslationFile(TranslationFile&&) = delete;
    TranslationFile& operator=(const TranslationFile&) = delete;
    TranslationFile& operator=(TranslationFile&&) = delete;

    [[nodiscard]] const std::filesystem::path& Path() const {
      return path_;
    }

    /// The file as it reached the disk - the bytes rather than anything the manager reported, since
    /// what is being asserted is precisely that the two can disagree.
    [[nodiscard]] std::string Read() const {
      std::ifstream file(path_, std::ios::binary);
      return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }

  private:
    std::filesystem::path path_;
  };
}

// A script's string is found under the key it registered, which is the whole of what the overlay is
// for: a key that resolved to itself before the script loaded resolves to the script's text after.
TEST(localization_overlay, a_registered_key_resolves_to_the_scripts_text) {
  Manager manager;
  const auto key = UniqueName("overlaytest/key");

  EXPECT_EQ(manager.Localize(key), key) << "an unregistered key should show as written";

  ASSERT_TRUE(manager.RegisterScriptTranslation(key, "the script's text", "a script"));
  EXPECT_EQ(manager.Localize(key), "the script's text");
}

// The menu's own keys are not a script's to take. A script that overwrote one would be rewriting the
// menu's text from outside, and the check is here rather than in the binding because it is the one
// place that knows which keys the built-in translation holds.
TEST(localization_overlay, a_key_the_menu_ships_is_refused) {
  Manager manager;

  EXPECT_FALSE(manager.RegisterScriptTranslation(kBuiltinKey, "not the menu's text", "a script"))
    << "a key the loaded translation holds must not be a script's to take";
  EXPECT_NE(manager.Localize(kBuiltinKey), "not the menu's text");
}

// One script cannot take another's key either, and cannot drop it: the first script to register a key
// keeps it, which is the only answer that does not have two scripts silently reading each other's text.
TEST(localization_overlay, another_scripts_key_is_refused) {
  Manager manager;
  const auto key = UniqueName("overlaytest/shared");

  ASSERT_TRUE(manager.RegisterScriptTranslation(key, "the first script's text", "first"));
  EXPECT_FALSE(manager.RegisterScriptTranslation(key, "the second script's text", "second"));
  EXPECT_EQ(manager.Localize(key), "the first script's text");

  EXPECT_FALSE(manager.RemoveScriptTranslation(key, "second")) << "a key is not another script's to remove";
  EXPECT_EQ(manager.Localize(key), "the first script's text");

  EXPECT_TRUE(manager.RemoveScriptTranslation(key, "first"));
  EXPECT_EQ(manager.Localize(key), key) << "a removed key should show as written again";
}

// A script registering its own key again - after a reload, or twice in one init - is asking for the
// new text, since that is the only thing it can mean.
TEST(localization_overlay, a_script_may_register_its_own_key_again_with_a_new_value) {
  Manager manager;
  const auto key = UniqueName("overlaytest/again");

  ASSERT_TRUE(manager.RegisterScriptTranslation(key, "the first text", "a script"));
  ASSERT_TRUE(manager.RegisterScriptTranslation(key, "the second text", "a script"));
  EXPECT_EQ(manager.Localize(key), "the second text");
  EXPECT_EQ(manager.ScriptTranslationCount(), 1U) << "registering again should not leave a second entry";
}

// What a script's unload does: exactly its own strings go, and nothing else does.
TEST(localization_overlay, remove_owned_by_drops_exactly_that_scripts_keys) {
  Manager manager;
  const auto first = UniqueName("overlaytest/first");
  const auto second = UniqueName("overlaytest/second");

  ASSERT_TRUE(manager.RegisterScriptTranslation(first, "the first script's text", "first"));
  ASSERT_TRUE(manager.RegisterScriptTranslation(second, "the second script's text", "second"));
  EXPECT_EQ(manager.ScriptTranslationCount(), 2U);

  manager.RemoveScriptTranslationsOwnedBy("first");

  EXPECT_EQ(manager.Localize(first), first) << "the unloaded script's key should be gone";
  EXPECT_EQ(manager.Localize(second), "the second script's text") << "the other script's key must survive";
  EXPECT_EQ(manager.ScriptTranslationCount(), 1U);

  // Idempotent, so that an unload that runs twice - a reload, a shutdown - is not a second change.
  manager.RemoveScriptTranslationsOwnedBy("first");
  EXPECT_EQ(manager.ScriptTranslationCount(), 1U);
}

// The property the whole split exists for: a script's strings are in memory and nowhere else. This is
// asserted against the file rather than against anything the manager reports, because a manager that
// merged the overlay in and then claimed not to would pass the weaker test.
TEST(localization_overlay, a_script_key_is_not_in_the_file_save_writes) {
  const auto translation = UniqueName("overlaytest_translation");
  const TranslationFile file(translation);
  const auto key = UniqueName("overlaytest/unsaved");
  const std::string value = "a text that must never reach the disk";

  {
    Manager manager;

    ASSERT_TRUE(manager.RegisterScriptTranslation(key, value, "a script"));
    EXPECT_EQ(manager.Localize(key), value) << "the string should be live while the menu is up";

    // Save is reached through the active translation, which is what the menu switches when a player
    // picks one: the file written is the loaded translation, and the overlay is not part of it.
    ASSERT_FALSE(manager.SetActiveTranslation(translation).has_error());
  }

  const std::string written = file.Read();
  ASSERT_FALSE(written.empty()) << "the translation file was not written, so there is nothing to assert against";
  EXPECT_EQ(written.find(key), std::string::npos) << "a script's key reached the disk";
  EXPECT_EQ(written.find(value), std::string::npos) << "a script's text reached the disk";
  EXPECT_NE(written.find(kBuiltinKey), std::string::npos) << "the menu's own strings are what the file is for";
}
