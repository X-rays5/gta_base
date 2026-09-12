//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

#include <angelscript.h>

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

#include "../../src/as/bindings/as_value.hpp"
#include "../../src/as/script/as_script_manager.hpp"
#include "../../src/as/script/as_script_manifest.hpp"
#include "../../src/util/glaze/generic_tree.hpp"

// What a script's serialization::Value is for is holding data the script cannot hold itself, so these
// tests are about the two ends of that: the document a script builds with subscripts, read back here
// through the same tree the script wrote through, and the values it builds it with - which are the
// script's to be destroyed by AngelScript rather than by us, and must not outlive the call that made
// them.
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
      dir_ = std::filesystem::temp_directory_path() / ("as_value_" + UniqueName("scripts"));
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

  /// The script under test, loaded into a manager of its own and ticked once - which is what runs the
  /// GameInit the tests put their work in.
  void RunGameInit(ScriptManager& manager, const ScriptManifest& manifest) {
    ASSERT_FALSE(manager.LoadScript(manifest).has_error());
    manager.TickScripts();
  }
}

// The shapes a document is built from, written by a script and read back here as a JSON file. Every
// write goes through one opAssign, so what this covers is that the type of what a script assigned is
// what the node becomes - and that subscripts two deep land two deep.
TEST(as_value, a_script_builds_a_document_and_saves_it_as_json_and_toml) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("writer");
  const ScriptData data(name);

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  root["name"] = "value";
  root["count"] = 7;
  root["flag"] = true;
  root["ratio"] = 0.5;
  root["nested"]["inner"] = "deep";
  root["list"][0] = 1;
  root["list"][1] = "two";
  root.Save("cfg.json", serialization::ValueFormat::JSON);
  root.Save("cfg.toml", serialization::ValueFormat::TOML);
}
)AS"));

  const auto json = data.Read("cfg.json");
  EXPECT_TRUE(json.IsObject());
  EXPECT_EQ(json.Key("name").ReadString(), "value");
  EXPECT_EQ(json.Key("count").ReadInt(), 7);
  EXPECT_TRUE(json.Key("flag").ReadBool());
  EXPECT_DOUBLE_EQ(json.Key("ratio").ReadDouble(), 0.5);
  EXPECT_EQ(json.Key("nested").Key("inner").ReadString(), "deep");
  EXPECT_EQ(json.Key("list").Size(), 2u);
  EXPECT_EQ(json.Key("list").Index(0).ReadInt(), 1);
  EXPECT_EQ(json.Key("list").Index(1).ReadString(), "two");

  // The same document as TOML, which is a different writer over the same tree - so a shape that only
  // one of them can spell would show up as a file that is not there.
  const auto toml = data.Read("cfg.toml", ValueFormat::kTOML);
  EXPECT_EQ(toml.Key("name").ReadString(), "value");
  EXPECT_EQ(toml.Key("count").ReadInt(), 7);
  EXPECT_TRUE(toml.Key("flag").ReadBool());
  EXPECT_EQ(toml.Key("nested").Key("inner").ReadString(), "deep");
  EXPECT_EQ(toml.Key("list").Index(1).ReadString(), "two");
}

// A round trip a script drives itself: what it saved is what it reads back, and what it reads back is
// what its own getters answer. The write is a second document rather than an assertion, so the test
// reads the answers off the disk instead of trusting the script to have failed loudly.
TEST(as_value, a_script_reads_back_what_it_saved) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("reader");
  const ScriptData data(name);

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  root["count"] = 7;
  root["name"] = "value";
  root.Save("round.json");

  root.Remove("count");
  root.Remove("name");
  root.Load("round.json");

  serialization::Value result;
  result["count"] = root["count"].GetInt();
  result["name"] = root["name"].GetString();
  result["absent"] = root["nope"].GetInt();
  result["number"] = root["count"].IsNumber();
  result["object"] = root.IsObject();
  result["has_count"] = root.Contains("count");
  result["has_nope"] = root.Contains("nope");
  result["size"] = root.GetSize();
  result["round"] = root["name"].GetString().length();
  result.Save("out.json");
}
)AS"));

  const auto out = data.Read("out.json");
  EXPECT_EQ(out.Key("count").ReadInt(), 7);
  EXPECT_EQ(out.Key("name").ReadString(), "value");

  // A member that is not there reads as the type's zero rather than failing, and asking about the
  // shape is how a script tells the two apart.
  EXPECT_EQ(out.Key("absent").ReadInt(), 0);
  EXPECT_TRUE(out.Key("number").ReadBool());
  EXPECT_TRUE(out.Key("object").ReadBool());
  EXPECT_TRUE(out.Key("has_count").ReadBool());
  EXPECT_FALSE(out.Key("has_nope").ReadBool());
  EXPECT_EQ(out.Key("size").ReadInt(), 2);
  EXPECT_EQ(out.Key("round").ReadInt(), 5) << "GetString read back a string of another length";
}

// The two conversions, which exist because a script's array and dictionary are the shapes it already
// has to hand - and because an array element reached by subscript needs no conversion at all.
TEST(as_value, a_script_converts_arrays_and_dictionaries_both_ways) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("converter");
  const ScriptData data(name);

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  std::array<int> nums(3);
  nums[0] = 1;
  nums[1] = 2;
  nums[2] = 3;
  root.SetIntArray(nums);
  root.Save("arrays.json");

  serialization::Value back;
  back.Load("arrays.json");
  std::array<int>@ read = back.GetIntArray();

  serialization::Value result;
  std::dictionary dict;
  dict["number"] = 4;
  dict["text"] = "five";
  dict["flag"] = true;
  result.SetDictionary(dict);
  result.Save("dict.json");

  serialization::Value dict_back;
  dict_back.Load("dict.json");
  std::dictionary@ read_back = dict_back.GetDictionary();
  result["from_dict"] = int(read_back["number"]);
  result["dict_text"] = std::string(read_back["text"]);
  result["dict_flag"] = bool(read_back["flag"]);
  result.Save("out.json");

  std::array<std::string>@ keys = result.GetKeys();
  serialization::Value notes;
  notes["sum"] = read[0] + read[1] + read[2];
  notes["length"] = int(read.length());
  notes["keys"] = int(keys.length());
  notes["tail_0"] = keys[keys.length() - 3];
  notes["tail_1"] = keys[keys.length() - 2];
  notes["tail_2"] = keys[keys.length() - 1];
  notes["has_number"] = result.Contains("number");
  notes["has_sum"] = result.Contains("sum");
  notes.Save("keys.json");
}
)AS"));

  const auto arrays = data.Read("arrays.json");
  ASSERT_TRUE(arrays.IsArray());
  EXPECT_EQ(arrays.Size(), 3u);
  EXPECT_EQ(arrays.Index(2).ReadInt(), 3);

  // Only the leaf kinds a tree has are converted, so the dictionary went into the node as an object and
  // came back out of it as a dictionary - which is what makes the trip lossless.
  const auto out = data.Read("out.json");
  EXPECT_EQ(out.Key("from_dict").ReadInt(), 4);
  EXPECT_EQ(out.Key("dict_text").ReadString(), "five");
  EXPECT_TRUE(out.Key("dict_flag").ReadBool());

  // SetDictionary sets the node rather than merging into it, so the members it brought are the whole
  // document until the subscripts after it add to it, and nothing written before it is left.
  EXPECT_EQ(out.Size(), 6u);
  EXPECT_FALSE(out.Contains("sum"));

  const auto notes = data.Read("keys.json");
  EXPECT_EQ(notes.Key("sum").ReadInt(), 6);
  EXPECT_EQ(notes.Key("length").ReadInt(), 3);

  // The three the dictionary brought are in the dictionary's own order, which a hash map promises
  // nothing about. The order that is defined is the one a subscript writes, so that is the part of the
  // key list this holds to: the three written after the dictionary, in the order they were written.
  EXPECT_EQ(notes.Key("keys").ReadInt(), 6);
  EXPECT_EQ(notes.Key("tail_0").ReadString(), "from_dict");
  EXPECT_EQ(notes.Key("tail_1").ReadString(), "dict_text");
  EXPECT_EQ(notes.Key("tail_2").ReadString(), "dict_flag");
  EXPECT_TRUE(notes.Key("has_number").ReadBool());
  EXPECT_FALSE(notes.Key("has_sum").ReadBool());
}

// The one the value design was chosen for. AngelScript destroys the temporary a subscript produced at
// the end of the statement that made it, so a script that subscripts a thousand times and keeps none
// of them must leave nothing behind: a leak here would be one object per subscript, every frame.
TEST(as_value, subscripts_leave_no_value_behind) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("loop");
  const ScriptData data(name);

  // Taken before the script is loaded, so that the baseline is the count with nothing of its making
  // alive - the test's own assertions are the only thing that has to be constant across it.
  const auto baseline = base::menu::as::bindings::value::ScriptValue::LiveInstances();

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  serialization::Value list = root["list"];
  for (uint i = 0; i < 500; i++) {
    list[i] = i;
    root["scalar"] = i;
    root["a"]["b"] = i;
  }

  root.Save("loop.json");
}
)AS"));

  const auto document = data.Read("loop.json");
  EXPECT_EQ(document.Key("list").Size(), 500u);
  EXPECT_EQ(document.Key("list").Index(499).ReadInt(), 499);
  EXPECT_EQ(document.Key("a").Key("b").ReadInt(), 499);

  // The script has left its session by now - GameInit returned - so every value it made has been
  // destroyed by the engine, and the count is back where it started.
  EXPECT_EQ(base::menu::as::bindings::value::ScriptValue::LiveInstances(), baseline)
      << "a subscripted value outlived the game init that made it";
}

// A path is resolved inside the script's own directory or refused, and refused means nothing written -
// not even the directories the name would have needed.
TEST(as_value, a_path_that_leaves_the_scripts_directory_is_refused) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("escaping");
  const ScriptData data(name);

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  root["a"] = 1;

  serialization::Value result;
  result["parent"] = root.Save("../escape.json");
  result["nested_parent"] = root.Save("sub/../../escape.json");
  result["empty"] = root.Save("");
  result["parent_load"] = root.Load("../escape.json");
  result["sub"] = root.Save("sub/cfg.json");
  result.Save("results.json");
}
)AS"));

  const auto out = data.Read("results.json");
  EXPECT_FALSE(out.Key("parent").ReadBool());
  EXPECT_FALSE(out.Key("nested_parent").ReadBool());
  EXPECT_FALSE(out.Key("empty").ReadBool());
  EXPECT_FALSE(out.Key("parent_load").ReadBool());

  EXPECT_FALSE(std::filesystem::exists(data.Path().parent_path() / "escape.json"));

  // The positive case in the same run, so a refusal that was really a broken Save would show here.
  ASSERT_TRUE(out.Key("sub").ReadBool());
  EXPECT_TRUE(std::filesystem::is_regular_file(data.Path() / "sub" / "cfg.json"));
}

// A TOML file is a table, so a document that is a bare number has no TOML to be written as - refused
// rather than written as something a reader would reject.
TEST(as_value, a_root_that_is_not_an_object_has_no_toml) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("scalar");
  const ScriptData data(name);

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  root = 5;

  serialization::Value result;
  result["toml"] = root.Save("scalar.toml", serialization::ValueFormat::TOML);
  result["json"] = root.Save("scalar.json");
  result["toml_text"] = root.Dump(serialization::ValueFormat::TOML).length();
  result["json_text"] = root.Dump(serialization::ValueFormat::JSON).length();
  result.Save("results.json");
}
)AS"));

  const auto out = data.Read("results.json");
  EXPECT_FALSE(out.Key("toml").ReadBool());
  EXPECT_TRUE(out.Key("json").ReadBool());
  EXPECT_EQ(out.Key("toml_text").ReadInt(), 0);
  EXPECT_GT(out.Key("json_text").ReadInt(), 0);

  EXPECT_FALSE(std::filesystem::exists(data.Path() / "scalar.toml"));
  EXPECT_TRUE(std::filesystem::exists(data.Path() / "scalar.json"));
}

// Text rather than a file, which is the half of Save and Load a script uses to hand a document to
// something that takes a string.
TEST(as_value, a_script_round_trips_text_without_a_file) {
  const ScriptDir dir;
  ScriptManager manager;
  const auto name = UniqueName("text");
  const ScriptData data(name);

  RunGameInit(manager, dir.WriteScript(name, R"AS(
void GameInit() {
  serialization::Value root;
  root["count"] = 7;
  root["nested"]["inner"] = "deep";

  serialization::Value other;
  other.Parse(root.Dump(serialization::ValueFormat::JSON), serialization::ValueFormat::JSON);

  serialization::Value result;
  result["count"] = other["count"].GetInt();
  result["inner"] = other["nested"]["inner"].GetString();
  result["bad"] = other.Parse("{{{ not a document", serialization::ValueFormat::JSON);
  result["survived"] = other["count"].GetInt();
  result.Save("out.json");
}
)AS"));

  const auto out = data.Read("out.json");
  EXPECT_EQ(out.Key("count").ReadInt(), 7);
  EXPECT_EQ(out.Key("inner").ReadString(), "deep");

  // Text that does not parse leaves the document as it was, so a failed read never half-applies.
  EXPECT_FALSE(out.Key("bad").ReadBool());
  EXPECT_EQ(out.Key("survived").ReadInt(), 7);
}
