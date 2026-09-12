//
// Created by X-ray on 11/09/2026.
//

#include <gtest/gtest.h>

#include <angelscript.h>

#include "../../src/as/util/as_bind.hpp"
#include "../../src/as/util/as_generate_predefined.hpp"
#include "../../src/as/bindings/as_native_types.hpp"
#include "../../src/as/util/as_util.hpp"

#include <rage/vector.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// as_bind.hpp exists because a registration spells its signature twice - once as the AngelScript
// declaration string the engine parses, and once as the C++ function the thunk calls - and a pair
// that disagrees compiles cleanly and corrupts memory at run time. Every test below pins one
// helper's ABI by running a script through a real engine, so a helper that derives the wrong
// spelling fails here rather than passing garbage in the game.
namespace {
  namespace as_util = base::menu::as::util;

  using as_util::ClearDocs;
  using as_util::RegisterConstructor;
  using as_util::RegisterEnum;
  using as_util::RegisterGlobalProperty;
  using as_util::RegisterMethod;
  using as_util::RegisterProperty;
  using as_util::RegisterValueType;
  namespace detail = as_util::detail;

  std::uint32_t g_error_count = 0;
  std::vector<std::string> g_errors;

  /// Collects the first few errors rather than only the last: one bad declaration is rarely the only
  /// one, and the count alone does not say which type or parameter the engine refused.
  void CollectErrors(const AngelScript::asSMessageInfo* msg, void*) {
    if (msg->type != AngelScript::asMSGTYPE_ERROR) {
      return;
    }
    if (g_errors.size() < 5) {
      g_errors.push_back(std::string(msg->section != nullptr ? msg->section : "") + ":" + std::to_string(msg->row) +
                         " " + msg->message);
    }
    ++g_error_count;
  }

  std::string ReportedErrors() {
    std::string joined;
    for (const auto& error : g_errors) {
      joined += "\n  " + error;
    }
    return joined;
  }

  /// Deliberately non-sequential and negative: a registration that walked the enumerators by index
  /// would give Busy a different value to the one the C++ type has.
  enum class Mood : int { Grim = -3, Calm = 0, Busy = 7 };

  /// A second enum, so the explicit-name form of RegisterEnum and a value away from zero are covered.
  enum class Level : int { Low = 1, High = 100 };

  /**
   * Every parameter shape RegisterMethod has to spell: a scalar by const reference (the shape the
   * hand-written opMul got wrong), a scalar the member writes back through, an enum, a value type
   * returned by value (the shape that faulted on the thiscall path), and a bool return.
   */
  struct Gauge {
    int ticks = 0;
    Mood mood = Mood::Calm;
    bool ready = false;
    float factor = 1.0f;

    /// Declared "float" because a scalar rides in a register; documenting it as "const float &in"
    /// would have the thunk dereference the float's bits as a pointer.
    void Advance(const float& amount) { factor += amount; }

    /// The other side of the same rule: a non-const reference is a write-back, so this one has to be
    /// declared "int &out" and the thunk has to receive a reference.
    void Split(int& half) const { half = ticks / 2; }

    bool Retune(const Mood value) {
      mood = value;
      return value == Mood::Busy;
    }

    Mood Current() const { return mood; }

    /// A method returning a type wider than a register is what the thiscall path gets wrong.
    rage::Vector3 Offset(const float& amount) const {
      return rage::Vector3(amount, factor, static_cast<float>(ticks));
    }
  };

  /// Three members of one type, so a constructor taking its arguments in the wrong order produces a
  /// value that reads differently rather than one that reads the same.
  struct Triple {
    int first = 0;
    int second = 0;
    int third = 0;
  };

  /// Where the stubs leave what a script produced.
  struct Host {
    int i = 0;
    int j = 0;
    float f = 0.0f;
    bool b = false;
    rage::Vector3 vec;
  };

  Host g_host;
  Gauge g_gauge;

  void ResetHost() {
    g_host = Host{};
    g_gauge = Gauge{};
  }

  void RegisterProbe(AngelScript::asIScriptEngine* engine) {
    // Nothing here is namespaced yet, so both enums land in the global namespace, where the "Mood"
    // RegisterMethod derives for a Mood parameter finds them.
    RegisterEnum<Mood>(engine).Desc("How busy the probe is.");
    RegisterEnum<Level>(engine, "Priority").Desc("An explicitly named enum.");

    // The test type lives where the natives do, because a method declared with a Vector3 return has
    // its declaration spelled "Vector3" and the engine resolves that inside the type's own namespace.
    engine->SetDefaultNamespace("natives");

    RegisterValueType<Gauge>(engine, "Gauge");
    RegisterConstructor<Gauge>(engine, "Gauge");
    RegisterProperty<&Gauge::ticks>(engine, "Gauge", "ticks");
    RegisterProperty<&Gauge::mood>(engine, "Gauge", "mood");
    RegisterProperty<&Gauge::ready>(engine, "Gauge", "ready");
    RegisterProperty<&Gauge::factor>(engine, "Gauge", "factor");

    RegisterMethod<&Gauge::Advance>(engine, "Gauge", "Advance", {"amount"});
    RegisterMethod<&Gauge::Split>(engine, "Gauge", "Split");
    RegisterMethod<&Gauge::Retune>(engine, "Gauge", "Retune", {"value"});
    RegisterMethod<&Gauge::Current>(engine, "Gauge", "Current");
    RegisterMethod<&Gauge::Offset>(engine, "Gauge", "Offset", {"amount"});

    RegisterValueType<Triple>(engine, "Triple");
    RegisterProperty<&Triple::first>(engine, "Triple", "first");
    RegisterProperty<&Triple::second>(engine, "Triple", "second");
    RegisterProperty<&Triple::third>(engine, "Triple", "third");
    RegisterConstructor<Triple, int, int, int>(engine, "Triple", {"first", "second", "third"})
      .Desc("A triple built from its three components.")
      .Param("first", "The first component.");

    engine->SetDefaultNamespace("probe");
    RegisterGlobalProperty(engine, "int i", &g_host.i);
    RegisterGlobalProperty(engine, "int j", &g_host.j);
    RegisterGlobalProperty(engine, "float f", &g_host.f);
    RegisterGlobalProperty(engine, "bool b", &g_host.b);
    RegisterGlobalProperty(engine, "natives::Vector3 vec", &g_host.vec);
    RegisterGlobalProperty(engine, "natives::Gauge gauge", &g_gauge);
    engine->SetDefaultNamespace("");
  }

  /// A real engine with the add-ons, the custom types and the probe type registered.
  AngelScript::asIScriptEngine* MakeEngine(const bool watch_errors = false) {
    auto* engine = AngelScript::asCreateScriptEngine();
    EXPECT_NE(engine, nullptr);

    g_error_count = 0;
    g_errors.clear();
    if (watch_errors) {
      engine->SetMessageCallback(AngelScript::asFUNCTION(CollectErrors), nullptr, AngelScript::asCALL_CDECL);
    }

    // The doc registry outlives the engine, so start from a clean slate the way a second debug run
    // of the script test does.
    ClearDocs();
    as_util::RegisterAddOns(engine);
    base::menu::as::bindings::native_types::Register(engine);
    RegisterProbe(engine);
    return engine;
  }

  /// Builds and runs `main` from `body`, without the surrounding boilerplate.
  testing::AssertionResult RunScript(AngelScript::asIScriptEngine* engine, const std::string& body) {
    const std::string source = "void main() {\n" + body + "\n}\n";

    AngelScript::asIScriptModule* mod = engine->GetModule("TestModule", AngelScript::asGM_ALWAYS_CREATE);
    mod->AddScriptSection("test.as", source.c_str());
    if (mod->Build() < 0) {
      return testing::AssertionFailure() << "the script did not build:\n" << source;
    }

    AngelScript::asIScriptFunction* func = mod->GetFunctionByDecl("void main()");
    if (func == nullptr) {
      return testing::AssertionFailure() << "main was not compiled";
    }

    AngelScript::asIScriptContext* ctx = engine->CreateContext();
    const int result = ctx->Prepare(func) >= 0 ? ctx->Execute() : -1;
    testing::AssertionResult outcome = testing::AssertionSuccess();
    if (result != AngelScript::asEXECUTION_FINISHED) {
      outcome = testing::AssertionFailure()
                << "execution ended with " << result << ": "
                << (ctx->GetExceptionString() != nullptr ? ctx->GetExceptionString() : "no exception string");
    }
    ctx->Release();
    return outcome;
  }

  /// Runs `body` against a fresh engine, so a test is one call rather than a wall of teardown.
  testing::AssertionResult RunOnFreshEngine(const std::string& body, const bool watch_errors = false) {
    auto* engine = MakeEngine(watch_errors);
    const auto result = RunScript(engine, body);
    engine->ShutDownAndRelease();
    return result;
  }

  std::vector<std::string> SplitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream{text};
    for (std::string line; std::getline(stream, line);) {
      if (not line.empty() && line.back() == '\r') line.pop_back();
      lines.push_back(line);
    }
    return lines;
  }

  std::string Trimmed(const std::string& line) {
    const auto begin = line.find_first_not_of(" \t");
    if (begin == std::string::npos) return {};
    const auto end = line.find_last_not_of(" \t");
    return line.substr(begin, end - begin + 1);
  }

  std::size_t FindDeclaration(const std::vector<std::string>& lines, const std::string& declaration) {
    for (std::size_t i = 0; i < lines.size(); ++i) {
      if (Trimmed(lines[i]) == declaration) return i;
    }
    return std::string::npos;
  }

  /// The first line at or after `from` whose trimmed text contains `needle`.
  std::size_t FindDeclarationAfter(const std::vector<std::string>& lines, const std::size_t from,
                                   const std::string& needle) {
    for (std::size_t i = from; i < lines.size(); ++i) {
      if (Trimmed(lines[i]).find(needle) != std::string::npos) return i;
    }
    return std::string::npos;
  }

  /**
   * The javadoc block sitting directly above a declaration, without the comment markers. Empty when
   * the line above the declaration is not the end of a doc comment.
   */
  std::string DocAbove(const std::vector<std::string>& lines, const std::size_t declaration) {
    if (declaration == 0 || Trimmed(lines[declaration - 1]) != "*/") return {};

    std::string doc;
    for (std::size_t i = declaration - 1; i-- > 0;) {
      const auto line = Trimmed(lines[i]);
      if (line == "/**") return doc;
      std::string content = line;
      if (content.starts_with("*")) {
        content.erase(0, 1);
        if (content.starts_with(" ")) content.erase(0, 1);
      }
      doc = doc.empty() ? content : content + "\n" + doc;
    }
    return {};
  }

  std::string GeneratePredefined(AngelScript::asIScriptEngine* engine) {
    const auto path = std::filesystem::temp_directory_path() / "gta_base_as_bind_test" / "as.predefined";
    std::filesystem::create_directories(path.parent_path());
    as_util::GenerateScriptPredefined(engine, path);

    std::ifstream stream{path};
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
  }
}

// ---------------------------------------------------------------- the ABI of each helper

// The declaration a helper writes is the one the engine parses, so the spelling is worth pinning
// down directly. "float" where the member takes a const float& is the opMul bug; "int &out" where
// ParamType saw a plain int is a write-back the script never sees.
TEST(as_bind, declarations_are_spelled_from_the_cpp_types) {
  EXPECT_EQ(detail::MethodTraits<decltype(&Gauge::Advance)>::Declare("Advance", {"amount"}),
            "void Advance(float amount)");
  EXPECT_EQ(detail::MethodTraits<decltype(&Gauge::Split)>::Declare("Split", {}),
            "void Split(int &out) const");
  EXPECT_EQ(detail::MethodTraits<decltype(&Gauge::Retune)>::Declare("Retune", {"value"}),
            "bool Retune(Mood value)");
  EXPECT_EQ(detail::MethodTraits<decltype(&Gauge::Current)>::Declare("Current", {}),
            "Mood Current() const");
  EXPECT_EQ(detail::MethodTraits<decltype(&Gauge::Offset)>::Declare("Offset", {"amount"}),
            "Vector3 Offset(float amount) const");
}

// The other half of the pair the helper has to keep in step: what the thunk actually receives. This
// is the assertion that would have caught the opMul bug at compile time - a member taking a const
// float& whose thunk takes a float by value.
TEST(as_bind, the_thunk_receives_what_the_declaration_passes) {
  static_assert(std::is_same_v<decltype(&detail::MethodTraits<decltype(&Gauge::Advance)>::template Call<&Gauge::Advance>),
                               void (*)(Gauge&, float)>);
  static_assert(std::is_same_v<decltype(&detail::MethodTraits<decltype(&Gauge::Split)>::template Call<&Gauge::Split>),
                               void (*)(const Gauge&, int&)>);
  static_assert(std::is_same_v<decltype(&detail::MethodTraits<decltype(&Gauge::Retune)>::template Call<&Gauge::Retune>),
                               bool (*)(Gauge&, Mood)>);
  static_assert(std::is_same_v<decltype(&detail::MethodTraits<decltype(&Gauge::Current)>::template Call<&Gauge::Current>),
                               Mood (*)(const Gauge&)>);
  static_assert(std::is_same_v<decltype(&detail::MethodTraits<decltype(&Gauge::Offset)>::template Call<&Gauge::Offset>),
                               rage::Vector3 (*)(const Gauge&, float)>);

  SUCCEED();
}

// A scalar the member takes by const reference is declared by value, so the value has to reach it.
TEST(as_bind, a_scalar_const_reference_parameter_reaches_the_member) {
  ResetHost();

  const auto result = RunOnFreshEngine("probe::gauge.Advance(2.5f);");
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_gauge.factor, 3.5f);
}

// A scalar the member writes through has to be declared "int &out", or the script never sees the
// value the member left behind.
TEST(as_bind, a_scalar_write_back_parameter_reaches_the_script) {
  ResetHost();

  const auto result = RunOnFreshEngine(
    "probe::gauge.ticks = 9;\n"
    "probe::gauge.Split(probe::i);");
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.i, 4);
}

// A bool return and an enum parameter are the two shapes where a width mismatch is silent: four
// bytes either way, so a wrong spelling reads a plausible value instead of faulting.
TEST(as_bind, a_bool_return_and_an_enum_parameter_round_trip) {
  ResetHost();

  const auto result = RunOnFreshEngine(
    "probe::b = probe::gauge.Retune(Mood::Busy);\n"
    "probe::i = int(probe::gauge.Current());");
  ASSERT_TRUE(result);

  EXPECT_TRUE(g_host.b);
  EXPECT_EQ(g_host.i, 7);
}

// The same again for a negative enumerator: a registration that stored the index rather than the
// value would give Grim 0 here.
TEST(as_bind, a_negative_enumerator_keeps_its_value) {
  ResetHost();

  const auto result = RunOnFreshEngine(
    "probe::gauge.Retune(Mood::Grim);\n"
    "probe::i = int(probe::gauge.Current());");
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.i, -3);
}

// A method returning a type wider than a register is the shape the thiscall path gets wrong, so this
// is the test that would have caught the original access violation. It is worth running in Release
// too, where the return buffer is actually used by the codegen.
TEST(as_bind, a_value_type_return_survives_a_method_call) {
  ResetHost();

  const auto result = RunOnFreshEngine(
    "probe::gauge.ticks = 2;\n"
    "probe::gauge.factor = 4.0f;\n"
    "probe::vec = probe::gauge.Offset(0.5f);");
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_host.vec.x, 0.5f);
  EXPECT_FLOAT_EQ(g_host.vec.y, 4.0f);
  EXPECT_FLOAT_EQ(g_host.vec.z, 2.0f);
}

// The offset has to be the member's real one, and the declared type its real type; writing every
// member and reading them all back catches an offset that lands on a neighbour.
TEST(as_bind, properties_read_and_write_the_members_they_name) {
  EXPECT_EQ(detail::MemberOffset<&Gauge::ticks>(), offsetof(Gauge, ticks));
  EXPECT_EQ(detail::MemberOffset<&Gauge::mood>(), offsetof(Gauge, mood));
  EXPECT_EQ(detail::MemberOffset<&Gauge::ready>(), offsetof(Gauge, ready));
  EXPECT_EQ(detail::MemberOffset<&Gauge::factor>(), offsetof(Gauge, factor));

  ResetHost();

  const auto result = RunOnFreshEngine(
    "probe::gauge.ticks = 12;\n"
    "probe::gauge.mood = Mood::Busy;\n"
    "probe::gauge.ready = true;\n"
    "probe::gauge.factor = 3.5f;");
  ASSERT_TRUE(result);

  EXPECT_EQ(g_gauge.ticks, 12);
  EXPECT_EQ(g_gauge.mood, Mood::Busy);
  EXPECT_TRUE(g_gauge.ready);
  EXPECT_FLOAT_EQ(g_gauge.factor, 3.5f);
}

// A generated constructor has to build the object from the arguments in the order they were
// declared; the members are read back through the same object the script holds.
TEST(as_bind, a_generated_constructor_takes_its_arguments_in_order) {
  ResetHost();

  const auto result = RunOnFreshEngine(
    "natives::Triple t = natives::Triple(1, 2, 3);\n"
    "probe::i = t.first * 100 + t.second * 10 + t.third;");
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.i, 123);
}

// A constructor's docs are keyed under "Type::f" because every constructor reflects as "f". If the
// dump never looked that key up it would report the doc as matching no binding, and the block would
// not be written at all - so finding it above the constructor line is the whole assertion.
TEST(as_bind, a_constructor_doc_lands_above_the_constructor) {
  auto* engine = MakeEngine(true);
  const auto dump = GeneratePredefined(engine);
  engine->ShutDownAndRelease();
  ASSERT_EQ(g_error_count, 0U) << ReportedErrors();

  const auto lines = SplitLines(dump);
  const auto type = FindDeclaration(lines, "class Triple{");
  ASSERT_NE(type, std::string::npos) << "the test type was not emitted";

  // The engine renders a constructor as "Triple(int first, int second, int third);" rather than the
  // "void f(...)" it was registered with.
  const auto constructor = FindDeclarationAfter(lines, type, "Triple(");
  ASSERT_NE(constructor, std::string::npos) << "the constructor was not emitted";

  const auto doc = DocAbove(lines, constructor);
  EXPECT_NE(doc.find("A triple built from its three components."), std::string::npos) << doc;
  EXPECT_NE(doc.find("@param first The first component."), std::string::npos) << doc;
}

// Both forms of RegisterEnum: the derived name, and an explicit one where the leaf name would be
// wrong or collide.
TEST(as_bind, register_enum_registers_every_enumerator) {
  ResetHost();

  const auto result = RunOnFreshEngine(
    "probe::i = int(Mood::Busy);\n"
    "probe::j = int(Priority::High);\n");
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.i, 7);
  EXPECT_EQ(g_host.j, 100);
}

// The derived name is the C++ leaf name, which is what lets a Mood parameter be spelled "Mood" in a
// method declaration without anyone writing the name down twice.
TEST(as_bind, a_derived_enum_name_is_the_leaf_name) {
  EXPECT_EQ(as_util::kAsTypeName<Mood>, "Mood");
  EXPECT_EQ(as_util::kAsTypeName<Level>, "Level");
  EXPECT_EQ(as_util::kAsTypeName<rage::Vector3>, "Vector3");
  EXPECT_EQ(as_util::kAsTypeName<const rage::Vector3&>, "Vector3");
}

// Every declaration the probe registers has to be one the engine accepts. This is the test that
// fails loudly when a helper starts spelling a parameter wrong, rather than the type silently
// registering as something else.
TEST(as_bind, the_probe_type_registers_without_error) {
  auto* engine = MakeEngine(true);
  engine->ShutDownAndRelease();

  EXPECT_EQ(g_error_count, 0U) << ReportedErrors();
}
