//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

// The menu's headers are written against its precompiled header and name things like LOG_ERROR
// without including them, so the base-common headers go first, as the other tests here do.
#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>

// base_option.hpp reaches the option UI components, which are written against the precompiled header's
// icon font. A test is not that translation unit, so the font is brought in here rather than there.
#include <imfont/IconsFontAwesome6.hpp>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <random>
#include <string>

#include "../../src/as/bindings/as_option.hpp"
#include "../../src/as/script/as_script_manager.hpp"
#include "../../src/as/script/as_script_manifest.hpp"
#include "../../src/default.hpp"
#include "../../src/options/option_registry.hpp"
#include "../../src/script/game_task_executor.hpp"
#include "../../src/util/glaze/generic_tree.hpp"

// A script's Option is a handle on the menu's own option registry, so what these tests hold the two
// ends of that crossing to is that an option a script registers can be found by name and run with a
// command line read the way its own declaration says - and that nothing of it is left behind
// afterwards, neither the option in the registry nor the handle the run was called with.
namespace {
  using base::menu::as::script::ScriptManager;
  using base::menu::as::script::ScriptManifest;
  using base::menu::options::OptionRegistry;
  using base::menu::script::GameTaskExecutor;
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
      dir_ = std::filesystem::temp_directory_path() / ("as_option_" + UniqueName("scripts"));
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

    /// The manifest of a script named `name` over a main.as holding `source`, as the manager reads one.
    /// `short_name` is written into it as the script's option prefix when it is given, which is the
    /// only thing the manifest has to say about one.
    [[nodiscard]] ScriptManifest WriteScript(const std::string& name, const std::string& source,
                                             const std::optional<std::string>& short_name = std::nullopt) const {
      const auto script_dir = dir_ / name;
      std::filesystem::create_directories(script_dir);

      std::ofstream manifest(script_dir / "manifest.toml", std::ios::binary | std::ios::trunc);
      manifest << "name = \"" << name << "\"\n";
      if (short_name.has_value()) {
        manifest << "short_name = \"" << *short_name << "\"\n";
      }
      manifest << "main_file = \"main.as\"\n";
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

  /// The directory a script of that name saves into, removed with the test. The menu puts it under its
  /// working directory, which for a test is the build tree, so leaving it behind would leave one
  /// directory per test there.
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

    [[nodiscard]] const std::filesystem::path& Path() const {
      return dir_;
    }

    /// The file `name` of that directory, read as the document the script wrote, so that the assertions
    /// below are on the bytes that reached the disk rather than on anything the script reported.
    [[nodiscard]] GenericView Read(const std::string& name, const ValueFormat format = ValueFormat::kJSON) const {
      auto tree = std::make_shared<GenericTree>(dir_);
      GenericView root(tree, {});
      EXPECT_TRUE(root.Load(name, format)) << "the script did not write " << name;
      return root;
    }

  private:
    std::filesystem::path dir_;
  };

  /// The menu a script's options need to exist at all, built in the order each part assumes and
  /// destroyed in the reverse of it.
  ///
  /// The order is the whole of what this fixture is, and each of the four is here for a reason the
  /// script layer cannot do without:
  ///
  /// - the profiles come first, since the registry registers the menu's own options out of them;
  /// - the registry comes second, because it is what `Options::Register` registers into;
  /// - the game task queue comes third, and is what a run is handed to: with one, `Options::Run` queues
  ///   its callback the way it does in the game rather than running it nested inside the caller;
  /// - the script manager comes last, and goes first, so that every script is destroyed - and has its
  ///   options unregistered - while the registry is still there to unregister them from.
  class Menu {
  public:
    Menu() = default;

    ScriptManager& Scripts() {
      return scripts_;
    }

    OptionRegistry& Options() {
      return options_;
    }

    /// One pass of the game thread: the scripts first, then the queue of work they handed over. In the
    /// game the queue is a host of its own and its turn may come before the scripts', which is one
    /// tick of latency and nothing else; here it follows them, so that a run asked for from GameInit
    /// has run its callback by the time this returns.
    void Pass() {
      scripts_.TickScripts();
      game_tasks_.Tick();
    }

  private:
    base::menu::Default defaults_;
    OptionRegistry options_;
    GameTaskExecutor game_tasks_;
    ScriptManager scripts_;
  };

  /// Load a script and make one pass, which is what runs the GameInit these tests put their work in.
  void RunGameInit(Menu& menu, const ScriptManifest& manifest) {
    ASSERT_FALSE(menu.Scripts().LoadScript(manifest).has_error());
    menu.Pass();
  }
}

// The whole path a script takes to have an option of its own: declare it, give it a callback,
// register it, and run it by name with a command line - which is parsed against the declaration the
// script made, and which the callback then reads the values out of.
TEST(as_option, a_script_registers_an_option_that_is_run_by_name_with_a_command_line) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("registrar");
  const ScriptData data(name);

  RunGameInit(menu, dir.WriteScript(name, R"AS(
// The callback is handed the option itself, which is where the arguments of this run are read from.
void OnLevel(Option@ &in self) {
  serialization::Value results;
  results["level"] = self.GetInt("--level");
  results["level_undashed"] = self.GetInt("level");
  results["text"] = self.GetString("--level");
  results["number"] = self.GetDouble("--level");
  results["used"] = self.WasUsed("--level");
  results["unused"] = self.WasUsed("--other");
  results["name"] = self.GetName();
  results.Save("callback.json");
}

void GameInit() {
  Option@ opt = Option("script_level", "Sets the level.");
  opt.AddArg("--level", "i", "The level to apply.");
  opt.SetCallback(@OnLevel);

  serialization::Value results;
  results["registered"] = Options::Register(opt);
  results["duplicate"] = Options::Register(opt);
  results["declared"] = opt.GetName();
  results["run"] = Options::Run("script_level", "--level 3");
  results["unknown_run"] = Options::Run("no_such_option", "");
  results["bad_run"] = Options::Run("script_level", "--level abc");
  results.Save("init.json");
}
)AS"));

  const auto init = data.Read("init.json");
  EXPECT_TRUE(init.Key("registered").ReadBool());
  EXPECT_FALSE(init.Key("duplicate").ReadBool()) << "a name that is taken must not be registered over";

  // The name the script wrote is not the name the option has: the script's own prefix goes in front of
  // it, so that what the menu holds it under is one a script of its own could not have taken.
  EXPECT_EQ(init.Key("declared").ReadString(), name + ".script_level");
  EXPECT_TRUE(init.Key("run").ReadBool());
  EXPECT_FALSE(init.Key("unknown_run").ReadBool()) << "running a name no option has must be refused";
  EXPECT_FALSE(init.Key("bad_run").ReadBool()) << "a command line that does not parse must run nothing";

  // The run was asked for from GameInit and the callback ran in the queue that follows it, so the run
  // was handed over rather than carried out inside the script that asked for it.
  const auto callback = data.Read("callback.json");
  EXPECT_EQ(callback.Key("level").ReadInt(), 3);
  EXPECT_EQ(callback.Key("level_undashed").ReadInt(), 3) << "an argument should be readable under either spelling";
  EXPECT_EQ(callback.Key("text").ReadString(), "3") << "an argument declared as an integer should read as its text";
  EXPECT_DOUBLE_EQ(callback.Key("number").ReadDouble(), 3.0);
  EXPECT_TRUE(callback.Key("used").ReadBool());
  EXPECT_FALSE(callback.Key("unused").ReadBool()) << "an argument the option does not declare was reported as used";
  EXPECT_EQ(callback.Key("name").ReadString(), name + ".script_level");
}

// The reason the prefix is there at all: two scripts each registering an option called the same thing,
// which is the collision every script would otherwise have over the one flat set of names. Each lands
// under its own script's prefix, and a run of the bare name from inside a script runs that script's
// own - the name it wrote, rather than anything the other script registered.
TEST(as_option, two_scripts_register_the_same_option_name_without_colliding) {
  Menu menu;
  const ScriptDir dir;
  const auto first = UniqueName("first");
  const auto second = UniqueName("second");
  const ScriptData first_data(first);
  const ScriptData second_data(second);

  constexpr const char* kSource = R"AS(
void OnVolume(Option@ &in self) {
  serialization::Value results;
  results["name"] = self.GetName();
  results["level"] = self.GetInt("--level");
  results.Save("callback.json");
}

void GameInit() {
  Option@ opt = Option("volume", "Sets the volume.");
  opt.AddArg("--level", "i", "The level to set.");
  opt.SetCallback(@OnVolume);
  Options::Register(opt);

  serialization::Value results;
  results["declared"] = opt.GetName();
  results["run"] = Options::Run("volume", "--level 2");
  results.Save("init.json");
}
)AS";

  RunGameInit(menu, dir.WriteScript(first, kSource));
  RunGameInit(menu, dir.WriteScript(second, kSource));

  // The name as the scripts wrote it is registered by neither: that is what the prefix is for, and it
  // is also why the menu's own options stay reachable - none of them carries one.
  EXPECT_EQ(menu.Options().GetOption("volume"), nullptr);
  ASSERT_NE(menu.Options().GetOption(first + ".volume"), nullptr);
  ASSERT_NE(menu.Options().GetOption(second + ".volume"), nullptr);

  const auto init = first_data.Read("init.json");
  EXPECT_EQ(init.Key("declared").ReadString(), first + ".volume");
  EXPECT_TRUE(init.Key("run").ReadBool());

  // Both were run by the same bare name and each ran its own, which is what the second reading of a
  // name is: the one the script registered, not the one registered first.
  const auto first_callback = first_data.Read("callback.json");
  EXPECT_EQ(first_callback.Key("name").ReadString(), first + ".volume");
  EXPECT_EQ(first_callback.Key("level").ReadInt(), 2);

  const auto second_callback = second_data.Read("callback.json");
  EXPECT_EQ(second_callback.Key("name").ReadString(), second + ".volume");
  EXPECT_EQ(second_callback.Key("level").ReadInt(), 2);
}

// The manifest's half of it: a script whose name is long says once, in one line, what its options are
// prefixed with - and what it reads back is that name, from the option itself and from its help, so
// what a script is told is what it can run.
TEST(as_option, a_short_name_in_the_manifest_is_the_prefix_instead_of_the_script_name) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("Example Option Registry");
  const ScriptData data(name);

  RunGameInit(menu, dir.WriteScript(name, R"AS(
void OnVolume(Option@ &in self) {
}

void GameInit() {
  Option@ opt = Option("volume", "Sets the volume.");
  opt.AddArg("--level", "i", "The level to set.");
  opt.SetCallback(@OnVolume);

  serialization::Value results;
  results["registered"] = Options::Register(opt);
  results["declared"] = opt.GetName();
  results["own"] = Options::Find("volume") !is null;
  results["run"] = Options::Run("volume", "--level 2");
  results["help"] = Options::Find("volume").GetHelp();

  // An option of the menu's own, which has no prefix to be read through and is therefore found as
  // written by a script that has one.
  results["menu"] = Options::Find("god_mode") !is null;
  results.Save("init.json");
}
)AS", "optreg"));

  const auto init = data.Read("init.json");
  EXPECT_TRUE(init.Key("registered").ReadBool());
  EXPECT_EQ(init.Key("declared").ReadString(), "optreg.volume");

  // Nothing is registered under the script's own name: the short name is what replaces it rather than
  // being a second name the option answers to.
  EXPECT_EQ(menu.Options().GetOption(name + ".volume"), nullptr);
  ASSERT_NE(menu.Options().GetOption("optreg.volume"), nullptr);

  EXPECT_TRUE(init.Key("own").ReadBool()) << "a script must be able to name its own option the way it declared it";
  EXPECT_TRUE(init.Key("run").ReadBool());
  EXPECT_TRUE(init.Key("menu").ReadBool()) << "a script's prefix must not keep it from the menu's own options";

  const auto help = init.Key("help").ReadString();
  EXPECT_NE(help.find("optreg.volume"), std::string::npos) << "the help names an option by a name it cannot be run by";
  EXPECT_NE(help.find("Sets the volume."), std::string::npos);
  EXPECT_NE(help.find("--level <int>"), std::string::npos);
}

// What a script can find out about the options that are already there, which is the half of this that
// is about running the menu's own rather than registering your own: find one by name, ask it what it
// is and what it takes, and list every option there is.
TEST(as_option, a_script_finds_and_lists_options_and_reads_their_help) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("finding");
  const ScriptData data(name);

  // Taken before the script is loaded, so that the baseline is the count with nothing of its making
  // alive - the test's own assertions are the only thing that has to be constant across it.
  const auto baseline = base::menu::as::bindings::option::ScriptOptionHandle::LiveInstances();

  RunGameInit(menu, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value results;

  Option@ god = Options::Find("god_mode");
  results["found"] = god !is null;
  results["name"] = god.GetName();
  results["help_has_name"] = god.GetHelp().find("god_mode") >= 0;

  Option@ missing = Options::Find("no_such_option");
  results["missing"] = missing is null;

  std::array<Option@>@ all = Options::All();
  results["count"] = int(all.length());
  bool listed = false;
  for (uint i = 0; i < all.length(); i++) {
    if (all[i].GetName() == "god_mode") {
      listed = true;
    }
  }

  results["listed"] = listed;

  // An option of the script's own, which is never registered: what its help says is what it was
  // declared with, which is how a script reads back what it has made so far.
  Option@ own = Option("script_help", "What the option is for.");
  own.AddArg("--count", "i", "How many.");
  own.AddArg("--flag", "b", "Whether.");
  results["help"] = own.GetHelp();

  results.Save("find.json");
}
)AS"));

  const auto out = data.Read("find.json");
  ASSERT_TRUE(out.Key("found").ReadBool()) << "the menu's own option should be findable by name";
  EXPECT_EQ(out.Key("name").ReadString(), "god_mode");
  EXPECT_TRUE(out.Key("help_has_name").ReadBool());
  EXPECT_TRUE(out.Key("missing").ReadBool()) << "a name no option has should be null rather than an option";
  EXPECT_TRUE(out.Key("listed").ReadBool());

  // The menu's own options are all in the list, which is what makes it a list a script can choose
  // from - nine of them, and the script's own is not one until it is registered.
  EXPECT_GE(out.Key("count").ReadInt(), 9);

  // One line per declared argument, naming the type it is read as: that, and not the description, is
  // what a script needs to be able to run an option it did not write. The name the help opens with is
  // the one it is registered under, which for an option a script made is the qualified one - so a
  // script that reads a name out of a help text can run it.
  const auto help = out.Key("help").ReadString();
  EXPECT_NE(help.find(name + ".script_help"), std::string::npos);
  EXPECT_NE(help.find("What the option is for."), std::string::npos);
  EXPECT_NE(help.find("--count <int>"), std::string::npos);
  EXPECT_NE(help.find("How many."), std::string::npos);
  EXPECT_NE(help.find("--flag <bool>"), std::string::npos);

  // Every handle of them was the engine's to release, including the ones the array was built from.
  EXPECT_EQ(base::menu::as::bindings::option::ScriptOptionHandle::LiveInstances(), baseline)
      << "an option handle outlived the game init that made it";
}

// The other end of an option's life: one a script registered is unregistered when that script
// unloads, and that has to be the script going away rather than anything the script does - the
// registry holds the option strongly, so it is the only thing that would otherwise keep it.
TEST(as_option, an_option_is_unregistered_with_the_script_that_registered_it) {
  Menu menu;
  const ScriptDir dir;
  const auto owner = UniqueName("owner");
  const auto checker = UniqueName("checker");
  const auto owner_data = ScriptData(owner);
  const ScriptData checker_data(checker);

  RunGameInit(menu, dir.WriteScript(owner, R"AS(
void OnN(Option@ &in self) {
}

void GameInit() {
  Option@ opt = Option("owned_option", "Belongs to a script.");
  opt.AddArg("--n", "i", "A number.");
  opt.SetCallback(@OnN);
  Options::Register(opt);

  serialization::Value results;
  results["registered"] = Options::Find("owned_option") !is null;
  results["help_has_arg"] = Options::Find("owned_option").GetHelp().find("--n") >= 0;
  results.Save("registered.json");
}
)AS"));

  const auto registered = owner_data.Read("registered.json");
  EXPECT_TRUE(registered.Key("registered").ReadBool());
  EXPECT_TRUE(registered.Key("help_has_arg").ReadBool());

  // Registered under the qualified name, which is what the unload below has to take back out.
  ASSERT_NE(menu.Options().GetOption(owner + ".owned_option"), nullptr);

  ASSERT_FALSE(menu.Scripts().UnloadScript(owner).has_error());

  // The pass after the unload is the one that releases the script, and releasing it is what takes its
  // options out of the registry.
  menu.Scripts().TickScripts();
  EXPECT_EQ(menu.Options().GetOption(owner + ".owned_option"), nullptr);

  // The same thing asked from a script, which is the half of it a script can act on: an option that
  // is gone cannot be found, and cannot be run either.
  RunGameInit(menu, dir.WriteScript(checker, R"AS(
void GameInit() {
  serialization::Value results;
  results["found"] = Options::Find("owned_option") !is null;
  results["run"] = Options::Run("owned_option", "--n 1");
  results.Save("after.json");
}
)AS"));

  const auto after = checker_data.Read("after.json");
  EXPECT_FALSE(after.Key("found").ReadBool()) << "the unloaded script's option is still in the registry";
  EXPECT_FALSE(after.Key("run").ReadBool());
}

// The handle a run is called with is made by the run and released by it, which is the whole of the
// reason a callback is declared with `Option@ &in self` rather than `Option@ self`: the engine
// reference-counts the argument of a handle parameter it is handed, and only gives that reference
// back when the call ends in an exception. A callback that returns and one that throws therefore have
// to leave the same count behind, and the count has to be the one from before the run.
TEST(as_option, a_run_leaves_no_handle_behind_whether_its_callback_returns_or_throws) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("counted");
  const ScriptData data(name);

  const auto baseline = base::menu::as::bindings::option::ScriptOptionHandle::LiveInstances();

  RunGameInit(menu, dir.WriteScript(name, R"AS(
void OnFine(Option@ &in self) {
  serialization::Value results;
  results["level"] = self.GetInt("--level");
  results.Save("fine.json");
}

// Saved before the fault, so that the file is evidence the callback ran rather than evidence it did
// not get that far.
void OnBoom(Option@ &in self) {
  serialization::Value results;
  results["ran"] = true;
  results.Save("boom.json");

  Option@ nothing = null;
  nothing.GetName();
}

void GameInit() {
  Option@ fine = Option("script_fine", "Runs to the end.");
  fine.AddArg("--level", "i", "The level to apply.");
  fine.SetCallback(@OnFine);
  Options::Register(fine);

  Option@ boom = Option("script_boom", "Throws.");
  boom.SetCallback(@OnBoom);
  Options::Register(boom);

  serialization::Value results;
  results["fine"] = Options::Run("script_fine", "--level 3");
  results["boom"] = Options::Run("script_boom", "");
  results.Save("init.json");
}
)AS"));

  const auto init = data.Read("init.json");
  EXPECT_TRUE(init.Key("fine").ReadBool());
  EXPECT_TRUE(init.Key("boom").ReadBool());

  EXPECT_EQ(data.Read("fine.json").Key("level").ReadInt(), 3);
  EXPECT_TRUE(data.Read("boom.json").Key("ran").ReadBool());

  // Both runs are over, and neither the handle the callback was given nor the one the script made its
  // option with is still counted.
  EXPECT_EQ(base::menu::as::bindings::option::ScriptOptionHandle::LiveInstances(), baseline)
      << "a run left a handle of its own behind";
}
