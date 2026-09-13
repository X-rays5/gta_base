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
#include <memory>
#include <random>
#include <string>

#include "../../src/as/script/as_script_manager.hpp"
#include "../../src/as/script/as_script_manifest.hpp"
#include "../../src/util/glaze/generic_tree.hpp"

// The `notify` namespace, which lives outside `gui` because a notification is not a page: a script
// that only wants to tell a player something has no business holding a component to do it with.
//
// What a test here can see is the half of it that does not need a menu. The manager that holds the
// notifications cannot be built outside the game - its constructor registers a render callback - so
// the positive path, that a notification a script raises is drawn, is in-game, and what is pinned
// here is everything up to it: that the namespace registers at all, that every overload binds as
// declared, that each returns what it says rather than throwing when there is no menu, and that the
// enum a script names the kind by has the same values as the C++ one.
namespace {
  using base::menu::as::script::ScriptManager;
  using base::menu::as::script::ScriptManifest;
  using base::menu::util::GenericTree;
  using base::menu::util::GenericView;
  using base::menu::util::ValueFormat;

  /// A name unique across processes, so that two tests running at once cannot write each other's
  /// documents: ctest discovers one entry per test and is free to run those in parallel.
  std::string UniqueName(const std::string& base) {
    static std::atomic<int> counter{0};
    return base + "_" + std::to_string(std::random_device{}()) + "_" + std::to_string(counter.fetch_add(1));
  }

  /// A scratch directory of scripts, removed with the test that made it.
  class ScriptDir {
  public:
    ScriptDir() {
      dir_ = std::filesystem::temp_directory_path() / ("as_notify_" + UniqueName("scripts"));
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
      std::filesystem::create_directories(dir_);
    }

    ~ScriptDir() {
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
    }

    ScriptDir(const ScriptDir&) = delete;
    ScriptDir(ScriptDir&&) = delete;
    ScriptDir& operator=(const ScriptDir&) = delete;
    ScriptDir& operator=(ScriptDir&&) = delete;

    [[nodiscard]] ScriptManifest WriteScript(const std::string& name, const std::string& source) const {
      const auto script_dir = dir_ / name;
      std::filesystem::create_directories(script_dir);

      std::ofstream manifest(script_dir / "manifest.toml", std::ios::binary | std::ios::trunc);
      manifest << "name = \"" << name << "\"\nmain_file = \"main.as\"\n";
      manifest.close();

      std::ofstream file(script_dir / "main.as", std::ios::binary | std::ios::trunc);
      file << source;
      file.close();

      ScriptManifest read(script_dir);
      const auto status = read.ReadMetaData();
      EXPECT_FALSE(status.has_error()) << "the test manifest did not read back: " << status.error().GetResultMessage();
      return read;
    }

  private:
    std::filesystem::path dir_;
  };

  /// The directory a script of that name saves into, removed with the test.
  class ScriptData {
  public:
    explicit ScriptData(const std::string& script_name) : dir_(base::common::fs::vfs::GetScriptDataDir(script_name)) {}

    ~ScriptData() {
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
    }

    ScriptData(const ScriptData&) = delete;
    ScriptData(ScriptData&&) = delete;
    ScriptData& operator=(const ScriptData&) = delete;
    ScriptData& operator=(ScriptData&&) = delete;

    [[nodiscard]] GenericView Read(const std::string& name, const ValueFormat format = ValueFormat::kJSON) const {
      auto tree = std::make_shared<GenericTree>(dir_);
      GenericView root(tree, {});
      EXPECT_TRUE(root.Load(name, format)) << "the script did not write " << name;
      return root;
    }

  private:
    std::filesystem::path dir_;
  };

  /// The script every test below runs: one call per overload there is, and the three enum values, all
  /// of them recorded so that the assertions are on what the calls returned rather than on nothing
  /// having crashed.
  constexpr const char* kNotifyScript = R"AS(
void GameInit() {
  serialization::Value results;

  // show, the one that takes the kind: what the nine below are.
  results["show"] = notify::show(notify::Type::Info, "a title", "a message", 1234);

  // Three arities for each of the three kinds, so that each name's overload set is bound in full
  // rather than only its first member.
  results["info_full"] = notify::info("a title", "a message", 1234);
  results["info_message"] = notify::info("a title", "a message");
  results["info_title"] = notify::info("a title");

  results["warning_full"] = notify::warning("a title", "a message", 1234);
  results["warning_message"] = notify::warning("a title", "a message");
  results["warning_title"] = notify::warning("a title");

  results["error_full"] = notify::error("a title", "a message", 1234);
  results["error_message"] = notify::error("a title", "a message");
  results["error_title"] = notify::error("a title");

  // The kind read back as the number the C++ enum has for it. Type is registered by hand with its
  // three values listed one at a time - its leaf name collides with script::ScriptBase::Type, so the
  // deriving overload cannot be used for it - which is exactly the kind of registration that can
  // list the wrong values and still compile.
  results["info_kind"] = int(notify::Type::Info);
  results["warning_kind"] = int(notify::Type::Warning);
  results["error_kind"] = int(notify::Type::Error);

  results.Save("notify.json");
}
)AS";

  /// A menu with no notification manager and no localization manager, which is what a harness has:
  /// both are constructed by the game and neither can be built here.
  void RunNotifyScript(ScriptManager& manager, const ScriptDir& dir, const std::string& name) {
    const auto loaded = manager.LoadScript(dir.WriteScript(name, kNotifyScript));
    ASSERT_FALSE(loaded.has_error())
      << "the notify namespace did not register, or one of its declarations does not bind: "
      << loaded.error().GetResultMessage();
    manager.TickScripts();
  }
}

// Every shape of every kind binds as declared, and every one of them answers rather than throwing
// when there is nothing to show the notification with.
TEST(as_notify, every_overload_answers_false_without_a_notification_manager) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("notifier");
  const ScriptData data(name);

  RunNotifyScript(manager, dir, name);

  const auto results = data.Read("notify.json");

  constexpr const char* kAll[] = {"show",          "info_full",      "info_message",      "info_title",
                                  "warning_full",  "warning_message", "warning_title",    "error_full",
                                  "error_message", "error_title"};

  for (const char* const key : kAll) {
    EXPECT_FALSE(results.Key(key).ReadBool())
      << key << " should report that it was not shown, since there is no menu to show it with";
  }
}

// The values a script names the kind by, which are the C++ enum's. Registered one value at a time
// with an explicit name, so a value that was listed wrong would be a notification drawn in the wrong
// colour rather than a declaration the engine refuses.
TEST(as_notify, the_kind_enum_has_the_same_values_as_the_cpp_one) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("kinds");
  const ScriptData data(name);

  RunNotifyScript(manager, dir, name);

  const auto results = data.Read("notify.json");
  EXPECT_EQ(results.Key("info_kind").ReadInt(), 0) << "Type::Info is the first value of the C++ enum";
  EXPECT_EQ(results.Key("warning_kind").ReadInt(), 1);
  EXPECT_EQ(results.Key("error_kind").ReadInt(), 2);
}

// The namespace being registered under its own name rather than in `gui`: a script that only wants to
// say something calls `notify::info` and never names a page, a component or a submenu to do it.
TEST(as_notify, the_functions_are_in_the_notify_namespace_and_not_in_gui) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("namespace");

  // Deliberately unbuildable: there is no `gui::info`, so if the notify registration had landed in
  // the GUI's namespace after all, this module would build and this load would succeed.
  const auto loaded = manager.LoadScript(dir.WriteScript(name, R"AS(
void GameInit() {
  gui::info("a title");
}
)AS"));

  EXPECT_TRUE(loaded.has_error()) << "gui::info does not exist, so a script calling it must not build";
}
