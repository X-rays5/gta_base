//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

// The menu's headers are written against its precompiled header and name things like LOG_ERROR
// without including them, so the base-common headers go first, as the other tests here do.
#include <base-common/fs/vfs.hpp>
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>

// The components are written against the precompiled header's icon font. A test is not that
// translation unit, so the font is brought in here rather than there.
#include <imfont/IconsFontAwesome6.hpp>

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <random>
#include <string>

#include "../../src/as/script/as_script_manager.hpp"
#include "../../src/as/script/as_script_manifest.hpp"
#include "../../src/default.hpp"
#include "../../src/options/option_registry.hpp"
#include "../../src/script/game_task_executor.hpp"
#include "../../src/script/general_task_executor.hpp"
#include "../../src/ui/components/execute_component.hpp"
#include "../../src/ui/components/label_component.hpp"
#include "../../src/ui/components/list_component.hpp"
#include "../../src/ui/components/number_range_component.hpp"
#include "../../src/ui/components/sub_link_component.hpp"
#include "../../src/ui/components/toggle_component.hpp"
#include "../../src/ui/components/toggle_list_component.hpp"
#include "../../src/ui/components/toggle_number_range_component.hpp"
#include "../../src/ui/localization/manager.hpp"
#include "../../src/ui/script_gui/script_submenu.hpp"
#include "../../src/ui/script_gui/script_submenu_registry.hpp"
#include "../../src/ui/submenu.hpp"
#include "../../src/util/glaze/generic_tree.hpp"

// The GUI a script draws, driven without a renderer.
//
// MenuRenderer cannot be built here - its constructor registers a render callback - so nothing below
// goes through it. What it goes through is everything the renderer would: the page's own BuildInto,
// reached the way the renderer reaches it, and the frame those components land in. The renderer is what
// the harness stands in for, and what only it can show - that the page is drawn, and that its rows
// answer to a key press - is in-game.
namespace {
  using base::menu::as::script::ScriptManager;
  using base::menu::as::script::ScriptManifest;
  using base::menu::ui::Submenu;
  using base::menu::ui::components::ExecuteComponent;
  using base::menu::ui::components::LabelComponent;
  using base::menu::ui::components::ListComponent;
  using base::menu::ui::components::NumberRangeComponent;
  using base::menu::ui::components::SubLinkComponent;
  using base::menu::ui::components::ToggleComponent;
  using base::menu::ui::components::ToggleListComponent;
  using base::menu::ui::components::ToggleNumberRangeComponent;
  using base::menu::ui::script_gui::ScriptSubmenu;
  using base::menu::ui::script_gui::ScriptSubmenuRegistry;
  using base::menu::util::GenericTree;
  using base::menu::util::GenericView;
  using base::menu::util::ValueFormat;

  /// A name unique across processes, so that two tests running at once cannot write each other's files:
  /// ctest discovers one entry per test and is free to run those in parallel.
  std::string UniqueName(const std::string& base) {
    static std::atomic<int> counter{0};
    return base + "_" + std::to_string(std::random_device{}()) + "_" + std::to_string(counter.fetch_add(1));
  }

  /// A scratch directory of scripts, removed with the test that made it.
  class ScriptDir {
  public:
    ScriptDir() {
      dir_ = std::filesystem::temp_directory_path() / ("as_gui_" + UniqueName("scripts"));
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

  /// The document a script saved, read back as the values it recorded rather than as anything the
  /// script reported to anyone. Removed with the test.
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

  /// The menu a script's pages need to exist at all, built in the order each part assumes and destroyed
  /// in the reverse of it.
  ///
  /// The order is the whole of what this fixture is, and each part is here for a reason the script layer
  /// cannot do without:
  ///
  /// - the profiles and the option registry, which is where `Options::Register` puts a script's option;
  /// - the two task queues: the game one, which is what a script's own work is handed to, and the general
  ///   one, which is where `init()` runs - the pages and options below are all made there;

  /// - the localization manager, which every component's text goes through: a component whose text is
  ///   read at all - which is every assertion below - dereferences `kMANAGER`, so it has to be there;
  /// - the submenu registry, which is what a `gui::Submenu` is registered into and what `~Script` calls
  ///   to take a dying script's pages away - so it outlives every script;
  /// - the script manager last, and so destroyed first, so that every script is destroyed - and has its
  ///   pages and its strings unregistered - while the two registries are still there to unregister them
  ///   from.
  class Menu {
  public:
    ScriptManager& Scripts() {
      return scripts_;
    }

    ScriptSubmenuRegistry& Submenus() {
      return submenus_;
    }

    /// One pass of the game thread: the scripts first, then the general queue - which is where the init
    /// the scripts just queued runs - and then the game queue. In the game the general queue is a host
    /// with a thread of its own, and a script's init() may outlive this pass; here it is ticked in the
    /// same pass it was queued in, which is what a test needs to see a page registered.
    ///
    /// What it does not do is give a script its second pass, so no page below is built from a GameTick:
    /// these tests are about what init() made.
    void Pass() {
      scripts_.TickScripts();
      general_tasks_.Tick();
      game_tasks_.Tick();
    }

  private:
    base::menu::Default defaults_;
    base::menu::options::OptionRegistry options_;
    base::menu::script::GameTaskExecutor game_tasks_;
    base::menu::script::GeneralTaskExecutor general_tasks_{nullptr};
    base::menu::ui::localization::Manager localization_;
    ScriptSubmenuRegistry submenus_;
    ScriptManager scripts_;
  };

  /// The page a script made under `name_key`. The renderer reaches a page through the key it was
  /// registered under, which is its id rather than its name; a harness has neither, so this looks the
  /// registry's list over by the name the script gave.
  std::shared_ptr<ScriptSubmenu> Page(Menu& menu, const std::string& name_key) {
    for (const auto& page : menu.Submenus().Pages()) {
      if (page->GetNameKey() == name_key) {
        return page;
      }
    }

    return nullptr;
  }

  /// Draws `page` into a frame, the way the renderer does: a submenu holding the update lambda the page
  /// was registered with, and one build of it. Returned by value so that the caller keeps the frame
  /// alive for as long as it reads the components that were built into it.
  Submenu Frame(const std::shared_ptr<ScriptSubmenu>& page) {
    Submenu frame(page->GetNameKey(), [page](Submenu* target) {
      static_cast<void>(page->BuildInto(target));
    });
    frame.UpdateComponents();
    return frame;
  }

  /// The `n`th component of `frame` as `T`, or null - the caller's ASSERT names what it wanted.
  template <typename T>
  const T* Component(const Submenu& frame, const std::size_t n) {
    if (n >= frame.GetComponents().size()) {
      return nullptr;
    }

    return dynamic_cast<const T*>(frame.GetComponents()[n].component.get());
  }

  /// Load a script and make one pass, which is what runs the init() the page tests put their work in.
  void RunInit(Menu& menu, const ScriptManifest& manifest) {
    const auto loaded = menu.Scripts().LoadScript(manifest);
    ASSERT_FALSE(loaded.has_error())
      << "the gui namespace did not register, or one of its declarations does not bind: "
      << loaded.error().GetResultMessage();
    menu.Pass();
  }

  /// The script the component test runs: every kind of component there is, one per row, plus enough
  /// registration around them that a key and a literal are both exercised.
  constexpr const char* kPageScript = R"AS(
gui::Toggle@           g_enabled;
gui::List@             g_spawn;
gui::ToggleList@       g_flags;
gui::IntRange@         g_level;
gui::FloatRange@       g_ratio;
gui::ToggleIntRange@   g_flag_level;
gui::ToggleFloatRange@ g_flag_ratio;
gui::Execute@          g_apply;
gui::Link@             g_advanced_link;
gui::Submenu@          g_root;
gui::Submenu@          g_advanced;

void OnApply() {
  // Nothing to do: what is pinned here is that the row is drawn from a function of this script, not
  // what the function does.
}

void OnRootUi(gui::Submenu@ &in sub) {
  // A literal, which needs no translation to show as itself.
  sub.Add(gui::Label("nowhere"));

  sub.Add(g_enabled);
  sub.Add(g_spawn);
  sub.Add(g_flags);
  sub.Add(g_level);
  sub.Add(g_ratio);
  sub.Add(g_flag_level);
  sub.Add(g_flag_ratio);
  sub.Add(g_apply);
  sub.Add(g_advanced_link);

  // Made and dropped in the same statement. Nothing but the frame holds this one's state, which is what
  // the inline test below pins for a page made of nothing else.
  sub.Add(gui::Toggle("inline", "", true));
}

void OnAdvancedUi(gui::Submenu@ &in sub) {
  sub.Add(gui::Label("only here when you come looking"));
}

void init() {
  gui::translations::Register("menu", "Script Options");
  gui::translations::Register("spawn", "Spawn point");
  gui::translations::Register("spawn/desc", "Where the player is put.");
  gui::translations::Register("apply", "Apply");
  gui::translations::Register("advanced", "Advanced");

  // The `@` on the left of each is what makes it a handle assignment rather than a value assignment,
  // which the engine refuses for reference types. A declaration that initialises - `gui::Toggle@ t =
  // gui::Toggle(...)` - needs no `@`, but taking the global first and filling it in here does.
  @g_enabled    = gui::Toggle("enabled", "", false);
  @g_spawn      = gui::List(gui::translations::Key("spawn"), "", {"garage", "airport", "docks"}, 1);
  @g_flags      = gui::ToggleList("flags", "", {"a", "b"}, 0, true);
  @g_level      = gui::IntRange("level", "", 5, 1, 100, 1);
  @g_ratio      = gui::FloatRange("ratio", "", 1.5f, 0.0f, 2.0f, 0.25f);
  @g_flag_level = gui::ToggleIntRange("flag_level", "", 3, 1, 10, 1, true);
  @g_flag_ratio = gui::ToggleFloatRange("flag_ratio", "", 0.5f, 0.0f, 1.0f, 0.25f, false);
  @g_apply      = gui::Execute(gui::translations::Key("apply"), "", @OnApply);

  // The page the other one is reached from, made first, so that the pair of ids below is a known one.
  @g_advanced      = gui::Submenu("advanced", @OnAdvancedUi);
  @g_root          = gui::Submenu(gui::translations::Key("menu"), @OnRootUi);
  @g_advanced_link = gui::Link(gui::translations::Key("advanced"), "", g_advanced, null);

  serialization::Value results;
  results["root_id"] = int(g_root.GetId());
  results["root_name_key"] = g_root.GetName();
  results["advanced_id"] = int(g_advanced.GetId());
  results["registered"] = g_root.RegisterRoot();
  results["registered_again"] = g_root.RegisterRoot();

  // Outside a build: init() draws nothing, so there is no frame for these to join, and the honest
  // answer is no rather than a crash or a component that turns up in whichever frame is drawn next.
  results["added_outside"] = g_root.Add(gui::Label("outside"));
  results["option_outside"] = g_root.AddOption(Options::Find("no_such_option"), "outside");

  results.Save("gui.json");
}
)AS";
}

// Every kind of component a script can draw, drawn onto one page and read back out of the frame it was
// built into. This is the crossing the binding exists for: what the script wrote on one side, and what
// the renderer is about to draw on the other.
TEST(as_gui, a_page_draws_every_component_kind) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("painter");
  const ScriptData data(name);

  RunInit(menu, dir.WriteScript(name, kPageScript));

  const auto page = Page(menu, name + "/menu");
  ASSERT_NE(page, nullptr) << "the page the script made is not in the registry";

  const Submenu frame = Frame(page);
  ASSERT_EQ(frame.GetComponents().size(), 11U) << "a component the script added did not reach the frame";

  // 0 - the label. A literal, so it shows as written, and wrapped the way a label is drawn.
  const auto* const label = Component<LabelComponent>(frame, 0);
  ASSERT_NE(label, nullptr) << "row 0 is not a label";
  EXPECT_EQ(label->GetCenterText(), "[nowhere]");
  EXPECT_FALSE(label->HasLeftText()) << "a label is content between the rows rather than a row of its own";

  // 1 - a toggle whose description was left empty, with nothing registered under "<name>/desc": the miss
  // the fallback has to survive.
  const auto* const toggle = Component<ToggleComponent>(frame, 1);
  ASSERT_NE(toggle, nullptr) << "row 1 is not a toggle";
  EXPECT_EQ(toggle->GetLeftText(), "enabled");
  EXPECT_EQ(toggle->GetDescription(), "");
  EXPECT_EQ(toggle->GetRightText(), ICON_FA_XMARK) << "the toggle was made switched off";

  // 2 - the list, whose name and description are both keys the script registered: the name under the
  // prefixed key it passed to Key, and the description through the "<name>/desc" fallback, since it
  // passed an empty description and registered "<key>/desc".
  const auto* const list = Component<ListComponent>(frame, 2);
  ASSERT_NE(list, nullptr) << "row 2 is not a list";
  EXPECT_EQ(list->GetLeftText(), "Spawn point") << "a registered key should show the script's string";
  EXPECT_EQ(list->GetDescription(), "Where the player is put.") << "the empty description should fall back to <key>/desc";
  EXPECT_EQ(list->GetRightText(), "airport [2/3]") << "the items and the selection are the script's";

  // 3 - the same with a checkbox on it, which is the only difference between the two.
  const auto* const flags = Component<ToggleListComponent>(frame, 3);
  ASSERT_NE(flags, nullptr) << "row 3 is not a toggle list";
  EXPECT_EQ(flags->GetLeftText(), "flags") << "a literal needs no translation";
  EXPECT_EQ(flags->GetRightText(), std::string("a [1/2] ") + ICON_FA_CHECK);

  // 4 to 7 - the four ranges, which differ in their number and in whether they carry a checkbox. An
  // arrow is left off at a bound, so a value at neither bound has both of them.
  const auto* const level = Component<NumberRangeComponent<std::int32_t>>(frame, 4);
  ASSERT_NE(level, nullptr) << "row 4 is not an int range";
  EXPECT_EQ(level->GetLeftText(), "level");
  EXPECT_EQ(level->GetRightText(), std::string("5 ") + ICON_FA_ANGLE_LEFT + ICON_FA_ANGLE_RIGHT);

  const auto* const ratio = Component<NumberRangeComponent<float>>(frame, 5);
  ASSERT_NE(ratio, nullptr) << "row 5 is not a float range";
  EXPECT_EQ(ratio->GetRightText(), std::string("1.500 ") + ICON_FA_ANGLE_LEFT + ICON_FA_ANGLE_RIGHT)
    << "a float is drawn to three places";

  const auto* const flag_level = Component<ToggleNumberRangeComponent<std::int32_t>>(frame, 6);
  ASSERT_NE(flag_level, nullptr) << "row 6 is not a toggle int range";
  EXPECT_EQ(flag_level->GetRightText(),
            std::string("3 ") + ICON_FA_ANGLE_LEFT + ICON_FA_ANGLE_RIGHT + " " + ICON_FA_CHECK);

  const auto* const flag_ratio = Component<ToggleNumberRangeComponent<float>>(frame, 7);
  ASSERT_NE(flag_ratio, nullptr) << "row 7 is not a toggle float range";
  EXPECT_EQ(flag_ratio->GetRightText(),
            std::string("0.500 ") + ICON_FA_ANGLE_LEFT + ICON_FA_ANGLE_RIGHT + " " + ICON_FA_XMARK)
    << "this one was made switched off";

  // 8 - the row that runs something, whose name is a registered key like any other. Its handler is a
  // function of the script, held as a declaration and resolved again when the row is pressed, so nothing
  // about the script has to be alive for the row to be drawn.
  const auto* const execute = Component<ExecuteComponent<std::function<void()>>>(frame, 8);
  ASSERT_NE(execute, nullptr) << "row 8 is not an execute row";
  EXPECT_EQ(execute->GetLeftText(), "Apply");

  // 9 - the link to the script's other page. It is built from the key the registry made rather than from
  // a renderer id, which is what lets it be built with no renderer at all: as here, and as in the game
  // before the page it points at has ever been opened.
  const auto* const link = Component<SubLinkComponent<std::function<void()>>>(frame, 9);
  ASSERT_NE(link, nullptr) << "row 9 is not a link";
  EXPECT_EQ(link->GetLeftText(), "Advanced");
  EXPECT_EQ(link->GetRightText(), ICON_FA_ARROW_RIGHT);

  // 10 - the one made and dropped in the same statement.
  const auto* const inline_toggle = Component<ToggleComponent>(frame, 10);
  ASSERT_NE(inline_toggle, nullptr) << "row 10 is not a toggle";
  EXPECT_EQ(inline_toggle->GetLeftText(), "inline");
  EXPECT_EQ(inline_toggle->GetRightText(), ICON_FA_CHECK) << "the inline toggle's own state should still be readable";

  // And nothing a script did outside a build reached the frame: the page is eleven rows, not twelve.
  const auto results = data.Read("gui.json");
  EXPECT_FALSE(results.Key("added_outside").ReadBool()) << "Add outside a build must report that nothing was added";
  EXPECT_FALSE(results.Key("option_outside").ReadBool()) << "AddOption outside a build is the same";
}

// The page the inline test runs: nothing but components made and dropped on the spot.
constexpr const char* kInlineScript = R"AS(
gui::Submenu@ g_page;

void OnPageUi(gui::Submenu@ &in sub) {
  // Eight of them, each a temporary the engine destroys at the end of its own statement. The state a
  // row reads through - the atomic behind the checkbox - has no owner left afterwards but the frame it
  // was added to.
  sub.Add(gui::Toggle("a", "", true));
  sub.Add(gui::Toggle("b", "", false));
  sub.Add(gui::Toggle("c", "", true));
  sub.Add(gui::Toggle("d", "", false));
  sub.Add(gui::Toggle("e", "", true));
  sub.Add(gui::Toggle("f", "", false));
  sub.Add(gui::Toggle("g", "", true));
  sub.Add(gui::Toggle("h", "", false));
}

void init() {
  @g_page = gui::Submenu("inline", @OnPageUi);
}
)AS";

// A component a script made inline is a temporary, and the frame is what keeps its state alive. Every
// row below has to report the value it was made with: four on and four off, so a frame that kept none of
// those states would have them all reading through whichever address the allocator handed back next,
// and at least one row would report a value that is not its own.
TEST(as_gui, an_inline_components_state_outlives_the_statement_that_made_it) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("inline");

  RunInit(menu, dir.WriteScript(name, kInlineScript));

  const auto page = Page(menu, "inline");
  ASSERT_NE(page, nullptr);

  const Submenu frame = Frame(page);
  ASSERT_EQ(frame.GetComponents().size(), 8U);

  constexpr bool kExpected[8] = {true, false, true, false, true, false, true, false};
  constexpr const char* kNames[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};

  for (std::size_t i = 0; i < 8; ++i) {
    const auto* const toggle = Component<ToggleComponent>(frame, i);
    ASSERT_NE(toggle, nullptr) << "row " << i << " is not a toggle";
    EXPECT_EQ(toggle->GetLeftText(), kNames[i]);
    EXPECT_EQ(toggle->GetRightText(), kExpected[i] ? ICON_FA_CHECK : ICON_FA_XMARK)
      << "row " << i << " is reading state that is not its own, so the frame let it go";
  }
}

// A page is a page for as long as the script that made it is loaded, and the id it is known by is the
// registry's to give: the same page built twice is the same id, and a reload is a new page with a new
// one - which is exactly why nothing a script stores may be keyed by an id.
TEST(as_gui, a_page_keeps_its_id_across_builds_and_a_reload_gets_a_new_one) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("reloader");
  const auto manifest = dir.WriteScript(name, kPageScript);

  RunInit(menu, manifest);

  const auto page = Page(menu, name + "/menu");
  ASSERT_NE(page, nullptr);

  const std::uint64_t first = page->GetId();
  EXPECT_GE(first, 1'000'000'000ULL) << "a script's ids are handed out above every renderer id there is";
  EXPECT_EQ(page->GetKey(), "script/" + std::to_string(first));

  // Built again: the same page, and so the same id. The build is what the renderer runs every frame, so
  // an id that changed with it would be an id that changed sixty times a second.
  static_cast<void>(Frame(page));
  EXPECT_EQ(page->GetId(), first);

  // The other page the same script made, which is one behind it in the order they were made.
  const auto advanced = Page(menu, "advanced");
  ASSERT_NE(advanced, nullptr);
  EXPECT_EQ(advanced->GetId(), 1'000'000'000ULL) << "the first page a script makes is the first id there is";
  EXPECT_EQ(first, advanced->GetId() + 1);

  // Reloaded: the old page goes with the script, and the page that replaces it is a different one.
  ASSERT_FALSE(menu.Scripts().UnloadScript(name).has_error());
  menu.Pass();
  EXPECT_EQ(menu.Submenus().Count(), 0U) << "unloading a script did not take its pages away";
  EXPECT_FALSE(menu.Submenus().IsOutstanding(first)) << "the id of a page that went must not stay outstanding";

  RunInit(menu, manifest);

  const auto reloaded = Page(menu, name + "/menu");
  ASSERT_NE(reloaded, nullptr);
  EXPECT_NE(reloaded->GetId(), first) << "a reloaded script must not be given the id it had before";
  EXPECT_TRUE(menu.Submenus().IsOutstanding(reloaded->GetId()));
}

// What a script that tidies up after itself calls, and what its unload does for it either way: the page
// is gone from the registry, its id is no longer outstanding, and a second Remove is not a second change.
TEST(as_gui, removing_a_page_leaves_nothing_behind) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("remover");
  const ScriptData data(name);

  RunInit(menu, dir.WriteScript(name, R"AS(
gui::Submenu@ g_gone;
gui::Submenu@ g_kept;

void OnPageUi(gui::Submenu@ &in sub) {
  sub.Add(gui::Label("a"));
}

void init() {
  // Only one of them is a root: a script has one row on the root page of the menu, and its other pages
  // are reached from that one rather than listed beside it.
  @g_gone = gui::Submenu("gone", @OnPageUi);
  @g_kept = gui::Submenu("kept", @OnPageUi);
  g_gone.RegisterRoot();

  serialization::Value results;
  results["id"] = int(g_gone.GetId());
  results["removed"] = g_gone.Remove();
  results["removed_again"] = g_gone.Remove();
  results["registered_after"] = g_gone.RegisterRoot();
  results["kept_id"] = int(g_kept.GetId());
  results.Save("remove.json");
}
)AS"));

  const auto results = data.Read("remove.json");

  EXPECT_TRUE(results.Key("removed").ReadBool());
  EXPECT_FALSE(results.Key("removed_again").ReadBool()) << "a second Remove is harmless and reports nothing was there";
  EXPECT_FALSE(results.Key("registered_after").ReadBool()) << "a removed page must not be put back on the root page";
  EXPECT_EQ(menu.Submenus().Count(), 1U) << "removing one page took another with it";

  const auto id = static_cast<std::uint64_t>(results.Key("id").ReadInt());
  EXPECT_FALSE(menu.Submenus().IsOutstanding(id)) << "a removed page left its id outstanding";
  EXPECT_TRUE(menu.Submenus().IsOutstanding(static_cast<std::uint64_t>(results.Key("kept_id").ReadInt())));
  EXPECT_EQ(Page(menu, "gone"), nullptr) << "a removed page is not one the registry hands out";

  // The other way round - removed from here rather than from the script - reaches the same place, and a
  // frame built from a page that has gone is empty rather than stale. That is the check the renderer
  // makes before it ever reaches the script.
  const auto kept = Page(menu, "kept");
  ASSERT_NE(kept, nullptr);
  ASSERT_EQ(Frame(kept).GetComponents().size(), 1U) << "the page drew nothing while it was still there";

  EXPECT_TRUE(menu.Submenus().Remove(kept));
  EXPECT_EQ(Frame(kept).GetComponents().size(), 0U);
}

// The root page's tail: one row per script, carrying the name of the script that made it, however many
// times that script asked - and a second page of the same script is refused rather than listed beside it.
TEST(as_gui, a_script_gets_one_root_row_named_after_it) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("rooter");
  const ScriptData data(name);

  RunInit(menu, dir.WriteScript(name, R"AS(
gui::Submenu@ g_first;
gui::Submenu@ g_second;

void OnPageUi(gui::Submenu@ &in sub) {
  sub.Add(gui::Label("a"));
}

void init() {
  gui::translations::Register("first", "First");
  gui::translations::Register("second", "Second");

  @g_first  = gui::Submenu(gui::translations::Key("first"), @OnPageUi);
  @g_second = gui::Submenu(gui::translations::Key("second"), @OnPageUi);

  g_first.RegisterRoot();
  g_first.RegisterRoot();
  g_second.RegisterRoot();

  serialization::Value results;
  results["first"] = g_first.RegisterRoot();
  results["second"] = g_second.RegisterRoot();
  results.Save("roots.json");
}
)AS"));

  const auto results = data.Read("roots.json");
  EXPECT_TRUE(results.Key("first").ReadBool()) << "registering an already-registered page is not a failure";
  EXPECT_FALSE(results.Key("second").ReadBool()) << "a script has one root, so a second page cannot be one";

  // The second page is a page like any other - it exists, it draws, a link to it goes there - it is only
  // not listed on the root page as well.
  ASSERT_EQ(menu.Submenus().Count(), 2U);
  ASSERT_NE(Page(menu, name + "/second"), nullptr);

  // The root page as the renderer builds it: one row, showing the script's name rather than the name of
  // the page it opens - the row is what a player finds the script by, and the page's own name is what the
  // page calls itself once it is open.
  Submenu home("ui/script/test/home", [](Submenu*) {});
  menu.Submenus().BuildRootLinks(&home);

  const auto& components = home.GetComponents();
  ASSERT_EQ(components.size(), 1U) << "a script is listed once, however many pages it made";

  const auto* const row = dynamic_cast<const SubLinkComponent<std::function<void()>>*>(components[0].component.get());
  ASSERT_NE(row, nullptr) << "the root page's script rows are not links";
  EXPECT_EQ(row->GetLeftText(), name) << "the row is the script's own name, not the page's";
  EXPECT_EQ(row->GetRightText(), ICON_FA_ARROW_RIGHT);

  // And the rows are rebuilt from the registry every frame rather than stored, so a page that goes is a
  // row that goes with it.
  EXPECT_TRUE(menu.Submenus().Remove(Page(menu, name + "/first")));

  Submenu rebuilt("ui/script/test/home", [](Submenu*) {});
  menu.Submenus().BuildRootLinks(&rebuilt);
  EXPECT_EQ(rebuilt.GetComponents().size(), 0U) << "the removed page's row is still on the root page";
}

// A script that unloads takes its pages with it, without having asked: the registry is what the script's
// own destruction calls, and the pages are what it is destroyed before.
TEST(as_gui, unloading_a_script_takes_away_every_page_it_made) {
  Menu menu;
  const ScriptDir dir;
  const auto name = UniqueName("unloader");
  const ScriptData data(name);

  RunInit(menu, dir.WriteScript(name, R"AS(
gui::Submenu@ g_first;
gui::Submenu@ g_second;

void OnPageUi(gui::Submenu@ &in sub) {
  sub.Add(gui::Label("a"));
}

void init() {
  @g_first  = gui::Submenu("first", @OnPageUi);
  @g_second = gui::Submenu("second", @OnPageUi);
  g_first.RegisterRoot();

  serialization::Value results;
  results["first_id"] = int(g_first.GetId());
  results["second_id"] = int(g_second.GetId());
  results.Save("ids.json");
}
)AS"));

  const auto results = data.Read("ids.json");
  ASSERT_EQ(menu.Submenus().Count(), 2U);

  ASSERT_FALSE(menu.Scripts().UnloadScript(name).has_error());
  menu.Pass();

  EXPECT_EQ(menu.Submenus().Count(), 0U) << "a page the script only navigated to was left behind";
  EXPECT_FALSE(menu.Submenus().IsOutstanding(static_cast<std::uint64_t>(results.Key("first_id").ReadInt())));
  EXPECT_FALSE(menu.Submenus().IsOutstanding(static_cast<std::uint64_t>(results.Key("second_id").ReadInt())));
}
