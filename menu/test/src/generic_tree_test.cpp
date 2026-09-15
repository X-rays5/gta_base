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
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>
#include <string>

#include "../../src/util/glaze/generic_tree.hpp"

// The tree is what a script's Value is built on, so what it has to get right is what the script
// layer will never see: that a handle taken before a write still finds its node after it, that a
// document survives a trip through each format, and that a file name cannot be used to leave the
// directory the tree was given.
namespace {
  using base::menu::util::GenericTree;
  using base::menu::util::GenericView;
  using base::menu::util::ValueFormat;

  /// A scratch data directory, removed with the test that made it. Named randomly rather than after a
  /// counter: the suite is discovered into one ctest entry per test and ctest is free to run those in
  /// parallel, so the name has to be unique across processes.
  class DataDir {
  public:
    DataDir() {
      static std::atomic<int> counter{0};
      dir_ = std::filesystem::temp_directory_path() /
             ("as_generic_tree_" + std::to_string(std::random_device{}()) + "_" + std::to_string(counter.fetch_add(1)));
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
      std::filesystem::create_directories(dir_);
    }

    ~DataDir() {
      std::error_code ec;
      std::filesystem::remove_all(dir_, ec);
    }

    DataDir(const DataDir&) = delete;
    DataDir(DataDir&&) = delete;
    DataDir& operator=(const DataDir&) = delete;
    DataDir& operator=(DataDir&&) = delete;

    [[nodiscard]] const std::filesystem::path& Path() const {
      return dir_;
    }

    void WriteFile(const std::filesystem::path& path, const std::string& contents) const {
      std::ofstream file(path, std::ios::binary | std::ios::trunc);
      file << contents;
      EXPECT_TRUE(file.good()) << "could not write " << path.string();
    }

  private:
    std::filesystem::path dir_;
  };

  /// A tree over `dir`, and a handle to its root. The tree outlives every handle taken from it, which
  /// is what the handles being shared_ptr-based is for.
  class Document {
  public:
    explicit Document(const DataDir& dir) : tree_(std::make_shared<GenericTree>(dir.Path())), root_(tree_, {}) {}

    [[nodiscard]] const std::shared_ptr<GenericTree>& Tree() const {
      return tree_;
    }

    [[nodiscard]] GenericView Root() const {
      return root_;
    }

  private:
    std::shared_ptr<GenericTree> tree_;
    GenericView root_;
  };
}

TEST(GenericTree, SubscriptedWritesBuildTheDocumentTheyDescribe) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("name").WriteString("value");
  root.Key("count").WriteInt(7);
  root.Key("flag").WriteBool(true);
  root.Key("nested").Key("inner").WriteDouble(1.5);
  root.Key("list").Index(0).WriteInt(10);
  root.Key("list").Index(1).WriteInt(11);

  EXPECT_EQ(root.Key("name").ReadString(), "value");
  EXPECT_EQ(root.Key("count").ReadInt(), 7);
  EXPECT_TRUE(root.Key("flag").ReadBool());
  EXPECT_DOUBLE_EQ(root.Key("nested").Key("inner").ReadDouble(), 1.5);
  EXPECT_EQ(root.Size(), 5u);
  EXPECT_EQ(root.Key("list").Size(), 2u);
  EXPECT_EQ(root.Key("list").Index(1).ReadInt(), 11);
}

TEST(GenericTree, MissingReadsFallBackToTheirArgument) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  EXPECT_EQ(root.Key("absent").ReadInt(99), 99);
  EXPECT_EQ(root.Key("absent").ReadInt64(-1), -1);
  EXPECT_DOUBLE_EQ(root.Key("absent").ReadDouble(2.5), 2.5);
  EXPECT_FLOAT_EQ(root.Key("absent").ReadFloat(2.5f), 2.5f);
  EXPECT_EQ(root.Key("absent").ReadString("fallback"), "fallback");
  EXPECT_TRUE(root.Key("absent").ReadBool(true));

  // Present but of another type is the same answer as absent: a handle made while a node was one
  // thing has to keep working after something makes it another.
  root.Key("count").WriteInt(7);
  EXPECT_EQ(root.Key("count").ReadString("fallback"), "fallback");
  EXPECT_EQ(root.Key("count").ReadInt(), 7);
}

TEST(GenericTree, ReadsOfTheShapesReportTheSameShapeBack) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("null").SetNull();
  root.Key("object").Key("a").WriteInt(1);
  root.Key("array").Index(0).WriteInt(1);
  root.Key("number").WriteInt(1);
  root.Key("bool").WriteBool(true);
  root.Key("string").WriteString("a");

  EXPECT_TRUE(root.Key("null").IsNull());
  EXPECT_TRUE(root.Key("object").IsObject());
  EXPECT_TRUE(root.Key("array").IsArray());
  EXPECT_TRUE(root.Key("number").IsNumber());
  EXPECT_TRUE(root.Key("bool").IsBool());
  EXPECT_TRUE(root.Key("string").IsString());

  EXPECT_FALSE(root.Key("object").IsArray());
  EXPECT_FALSE(root.Key("array").IsObject());

  // A node that is not there at all is null rather than nothing, so a script asking about a document
  // it has not built yet gets an answer instead of a crash - and asking is what the six predicates
  // are for, so exactly one of them is true for every handle there is.
  const auto exactlyOneShape = [](const GenericView& value) {
    return value.IsNull() + value.IsObject() + value.IsArray() + value.IsNumber() + value.IsBool() + value.IsString() == 1;
  };

  EXPECT_TRUE(root.Key("absent").IsNull());
  for (const char* key : {"null", "object", "array", "number", "bool", "string", "absent"}) {
    EXPECT_TRUE(exactlyOneShape(root.Key(key))) << "for " << key;
  }
}

TEST(GenericTree, ANumberReadsBackAsAnyOfTheNumberTypes) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("int").WriteInt(7);
  root.Key("double").WriteDouble(2.6);

  EXPECT_EQ(root.Key("int").ReadInt(), 7);
  EXPECT_DOUBLE_EQ(root.Key("int").ReadDouble(), 7.0);
  EXPECT_FLOAT_EQ(root.Key("int").ReadFloat(), 7.0f);

  // Rounded rather than truncated towards zero, so that the rounding a script sees is the ordinary
  // one rather than the one that makes -0.5 and 0.5 both zero.
  EXPECT_EQ(root.Key("double").ReadInt(), 3);
  root.Key("double").WriteDouble(-2.6);
  EXPECT_EQ(root.Key("double").ReadInt(), -3);
}

TEST(GenericTree, SubscriptingThroughANonObjectReplacesIt) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("a").WriteInt(1);
  ASSERT_TRUE(root.Key("a").IsNumber());

  // The write is what decides the shape on the way down, since nothing else can: root["a"] was a
  // number and the only thing a member can be read out of is an object.
  root.Key("a").Key("b").WriteInt(2);
  EXPECT_TRUE(root.Key("a").IsObject());
  EXPECT_EQ(root.Key("a").Key("b").ReadInt(), 2);

  root.Key("c").WriteString("scalar");
  root.Key("c").Index(0).WriteInt(9);
  EXPECT_TRUE(root.Key("c").IsArray());
  EXPECT_EQ(root.Key("c").Index(0).ReadInt(), 9);
}

TEST(GenericTree, AnIndexPastTheEndGrowsTheArrayWithNulls) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("list").Index(3).WriteString("fourth");

  EXPECT_EQ(root.Key("list").Size(), 4u);
  EXPECT_TRUE(root.Key("list").Index(0).IsNull());
  EXPECT_TRUE(root.Key("list").Index(2).IsNull());
  EXPECT_EQ(root.Key("list").Index(3).ReadString(), "fourth");
  EXPECT_TRUE(root.Key("list").Index(4).IsNull());
}

TEST(GenericTree, MembersReadBackInTheOrderTheyWereWritten) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("b").WriteInt(1);
  root.Key("a").WriteInt(2);
  root.Key("c").WriteInt(3);

  const auto keys = root.Keys();
  ASSERT_EQ(keys.size(), 3u);
  EXPECT_EQ(keys[0], "b");
  EXPECT_EQ(keys[1], "a");
  EXPECT_EQ(keys[2], "c");

  // Not an object, so there are no members to name.
  root.Key("list").Index(0).WriteInt(1);
  EXPECT_TRUE(root.Key("list").Keys().empty());
  EXPECT_TRUE(root.Key("list").IsArray());
}

TEST(GenericTree, EraseRemovesTheNodeAndSaysWhetherThereWasOne) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("a").WriteInt(1);
  root.Key("list").Index(0).WriteInt(1);
  root.Key("list").Index(1).WriteInt(2);

  EXPECT_TRUE(root.Key("a").Erase());
  EXPECT_FALSE(root.Contains("a"));
  EXPECT_FALSE(root.Key("a").Erase());

  EXPECT_TRUE(root.Key("list").Index(0).Erase());
  EXPECT_EQ(root.Key("list").Size(), 1u);

  // The element that was second is now first, which is the whole of what an erased array element
  // means.
  EXPECT_EQ(root.Key("list").Index(0).ReadInt(), 2);
  EXPECT_FALSE(root.Key("list").Index(1).Erase());

  EXPECT_FALSE(root.Key("never").Key("was").Erase());
}

// The test this design exists for. glz::generic keeps an object's members in one flat block, so
// adding a member can move every member - which means a pointer to one taken beforehand can, after
// the insertion, point at another. The handle holds a path instead, so this is the case that has to
// keep working, and the one that would break if the path were ever resolved once and cached.
TEST(GenericTree, AHandleTakenBeforeAGrowthStillFindsItsOwnNode) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("first").WriteInt(42);
  const auto first = root.Key("first");

  for (int i = 0; i < 5000; ++i) {
    root.Key("filler_" + std::to_string(i)).WriteInt(i);
  }

  EXPECT_EQ(root.Size(), 5001u);
  EXPECT_EQ(first.ReadInt(), 42);
  EXPECT_EQ(root.Key("first").ReadInt(), 42);
  EXPECT_EQ(root.Key("filler_4999").ReadInt(), 4999);

  // The same for an array, whose elements are moved by growing it.
  root.Key("list").Index(0).WriteInt(7);
  const auto element = root.Key("list").Index(0);

  for (std::size_t i = 1; i < 5000; ++i) {
    root.Key("list").Index(i).WriteInt(static_cast<std::int32_t>(i));
  }

  EXPECT_EQ(element.ReadInt(), 7);
  EXPECT_EQ(root.Key("list").Index(4999).ReadInt(), 4999);
}

TEST(GenericTree, JsonRoundTripsThroughAFile) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("name").WriteString("value");
  root.Key("count").WriteInt(7);
  root.Key("flag").WriteBool(true);
  root.Key("ratio").WriteDouble(0.5);
  root.Key("nested").Key("inner").WriteString("deep");
  root.Key("list").Index(0).WriteInt(1);
  root.Key("list").Index(1).WriteString("two");

  ASSERT_TRUE(root.Save("cfg.json", ValueFormat::kJSON));
  ASSERT_TRUE(std::filesystem::is_regular_file(dir.Path() / "cfg.json"));

  const Document other(dir);
  ASSERT_TRUE(other.Root().Load("cfg.json", ValueFormat::kJSON));

  EXPECT_EQ(other.Root().Key("name").ReadString(), "value");
  EXPECT_EQ(other.Root().Key("count").ReadInt(), 7);
  EXPECT_TRUE(other.Root().Key("flag").ReadBool());
  EXPECT_DOUBLE_EQ(other.Root().Key("ratio").ReadDouble(), 0.5);
  EXPECT_EQ(other.Root().Key("nested").Key("inner").ReadString(), "deep");
  EXPECT_EQ(other.Root().Key("list").Size(), 2u);
  EXPECT_EQ(other.Root().Key("list").Index(0).ReadInt(), 1);
  EXPECT_EQ(other.Root().Key("list").Index(1).ReadString(), "two");
  EXPECT_EQ(other.Root().Size(), root.Size());
}

TEST(GenericTree, TomlRoundTripsThroughAFile) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("name").WriteString("value");
  root.Key("count").WriteInt(7);
  root.Key("flag").WriteBool(true);
  root.Key("nested").Key("inner").WriteString("deep");
  root.Key("list").Index(0).WriteInt(1);
  root.Key("list").Index(1).WriteInt(2);

  ASSERT_TRUE(root.Save("cfg.toml", ValueFormat::kTOML));
  ASSERT_TRUE(std::filesystem::is_regular_file(dir.Path() / "cfg.toml"));

  const Document other(dir);
  ASSERT_TRUE(other.Root().Load("cfg.toml", ValueFormat::kTOML));

  EXPECT_EQ(other.Root().Key("name").ReadString(), "value");
  EXPECT_EQ(other.Root().Key("count").ReadInt(), 7);
  EXPECT_TRUE(other.Root().Key("flag").ReadBool());
  EXPECT_EQ(other.Root().Key("nested").Key("inner").ReadString(), "deep");
  EXPECT_EQ(other.Root().Key("list").Index(1).ReadInt(), 2);
}

TEST(GenericTree, LoadIntoASubscriptedNodeReadsTheFileInThere) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("count").WriteInt(7);
  ASSERT_TRUE(root.Save("cfg.json", ValueFormat::kJSON));

  const Document other(dir);
  ASSERT_TRUE(other.Root().Key("copy").Load("cfg.json", ValueFormat::kJSON));
  EXPECT_EQ(other.Root().Key("copy").Key("count").ReadInt(), 7);
  EXPECT_FALSE(other.Root().Contains("count"));

  // And the other way round: a node saved is the node, not the document it came from.
  ASSERT_TRUE(other.Root().Key("copy").Save("copy.json", ValueFormat::kJSON));
  const Document third(dir);
  ASSERT_TRUE(third.Root().Load("copy.json", ValueFormat::kJSON));
  EXPECT_EQ(third.Root().Key("count").ReadInt(), 7);
}

TEST(GenericTree, TextRoundTripsWithoutAFile) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("count").WriteInt(7);
  root.Key("nested").Key("inner").WriteString("deep");

  const auto json = root.Dump(ValueFormat::kJSON);
  ASSERT_FALSE(json.empty());
  ASSERT_TRUE(root.Dump(ValueFormat::kTOML).find("count") != std::string::npos);

  const Document other(dir);
  ASSERT_TRUE(other.Root().Parse(json, ValueFormat::kJSON));
  EXPECT_EQ(other.Root().Key("count").ReadInt(), 7);
  EXPECT_EQ(other.Root().Key("nested").Key("inner").ReadString(), "deep");

  const Document third(dir);
  ASSERT_TRUE(third.Root().Parse(root.Dump(ValueFormat::kTOML), ValueFormat::kTOML));
  EXPECT_EQ(third.Root().Key("count").ReadInt(), 7);
  EXPECT_EQ(third.Root().Key("nested").Key("inner").ReadString(), "deep");
}

TEST(GenericTree, WritingARootThatIsNotAnObjectAsTomlIsRefused) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  // A TOML document is a table, so there is no TOML for this to be written as - refused rather than
  // written as something a reader would reject.
  root.WriteInt(5);
  EXPECT_FALSE(root.Save("scalar.toml", ValueFormat::kTOML));
  EXPECT_FALSE(std::filesystem::exists(dir.Path() / "scalar.toml"));
  EXPECT_TRUE(root.Dump(ValueFormat::kTOML).empty());

  // The same node as JSON is fine, because a JSON document can be a bare number.
  EXPECT_TRUE(root.Save("scalar.json", ValueFormat::kJSON));
  EXPECT_TRUE(std::filesystem::is_regular_file(dir.Path() / "scalar.json"));
}

TEST(GenericTree, ANameOutsideTheDirectoryIsRefusedWithNothingWritten) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("count").WriteInt(7);

  EXPECT_FALSE(root.Save("../escape.json", ValueFormat::kJSON));
  EXPECT_FALSE(std::filesystem::exists(dir.Path().parent_path() / "escape.json"));

  EXPECT_FALSE(root.Save("sub/../../escape.json", ValueFormat::kJSON));
  EXPECT_FALSE(std::filesystem::exists(dir.Path().parent_path() / "escape.json"));

  EXPECT_FALSE(root.Save((dir.Path().parent_path() / "absolute.json").string(), ValueFormat::kJSON));
  EXPECT_FALSE(std::filesystem::exists(dir.Path().parent_path() / "absolute.json"));

  // Nothing created on the way, either: a refused name must not leave the directories it would have
  // needed behind.
  EXPECT_FALSE(std::filesystem::exists(dir.Path() / "sub"));

  EXPECT_FALSE(root.Load("../escape.json", ValueFormat::kJSON));
  EXPECT_FALSE(root.Save("", ValueFormat::kJSON));

  // A name that only looks like it leaves is fine: this one resolves inside.
  ASSERT_TRUE(root.Save("sub/cfg.json", ValueFormat::kJSON));
  EXPECT_TRUE(std::filesystem::is_regular_file(dir.Path() / "sub" / "cfg.json"));
}

TEST(GenericTree, AFailedLoadLeavesTheNodeAsItWas) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  root.Key("keep").WriteInt(5);

  EXPECT_FALSE(root.Key("keep").Load("missing.json", ValueFormat::kJSON));
  EXPECT_EQ(root.Key("keep").ReadInt(), 5);

  dir.WriteFile(dir.Path() / "bad.json", "{{{ not a document");
  EXPECT_FALSE(root.Key("keep").Load("bad.json", ValueFormat::kJSON));
  EXPECT_EQ(root.Key("keep").ReadInt(), 5);

  EXPECT_FALSE(root.Key("keep").Parse("{{{ not a document", ValueFormat::kJSON));
  EXPECT_EQ(root.Key("keep").ReadInt(), 5);
}

TEST(GenericTree, TheArrayAndObjectAreReachableForConversion) {
  const DataDir dir;
  const Document doc(dir);
  const auto root = doc.Root();

  EXPECT_EQ(root.GetArray(), nullptr);
  EXPECT_EQ(root.GetObject(), nullptr);

  // Coercing, so that a caller meaning to build one does not have to check the shape first.
  auto& array = root.Key("list").MakeArray();
  EXPECT_TRUE(array.empty());
  EXPECT_NE(root.Key("list").GetArray(), nullptr);

  root.Key("list").Index(0).WriteInt(1);
  EXPECT_EQ(root.Key("list").GetArray()->size(), 1u);

  auto& object = root.Key("map").MakeObject();
  EXPECT_TRUE(object.empty());
  EXPECT_NE(root.Key("map").GetObject(), nullptr);

  // And a shape that is already right is left alone rather than emptied.
  root.Key("list").Index(1).WriteInt(2);
  EXPECT_EQ(root.Key("list").MakeArray().size(), 2u);
}

TEST(GenericTree, TheTreeKeepsTheDirectoryItWasGiven) {
  const DataDir dir;
  const Document doc(dir);

  EXPECT_EQ(doc.Tree()->GetDir(), dir.Path());
}
