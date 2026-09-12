//
// Created by X-ray on 11/09/2026.
//

#include <gtest/gtest.h>

#include <angelscript.h>

#include "../../src/as/util/as_bind.hpp"
#include "../../src/as/util/as_generate_predefined.hpp"
#include "../../src/as/bindings/as_native_types.hpp"
#include "../../src/as/util/as_util.hpp"
#include "../../src/game/native_types.hpp"
#include "../../src/natives/natives_as.hpp"

#include <base-common/conversion/joaat.hpp>
#include <rage/vector.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// The generated bindings spell every native's declaration out by hand rather than deducing it from
// the function pointer the way the sol2 emitter can, so a wrong spelling is silent memory corruption
// at runtime instead of a compile error. These tests are the only thing standing between that and
// the game: the first group pins the ABI of each mapping by running real C functions through the
// engine, and the last group registers all ~6,600 natives and fails on any declaration the engine
// refuses.
namespace {
  namespace as_util = base::menu::as::util;

  using as_util::RegisterGlobalFunction;
  using as_util::RegisterGlobalProperty;
  using base::menu::as::bindings::native_types::Register;

  std::uint32_t g_error_count = 0;
  std::vector<std::string> g_errors;

  /// Collects the first few errors rather than only the last, since one bad declaration is rarely
  /// the only one and the count alone does not say which types the engine refused.
  void CollectErrors(const AngelScript::asSMessageInfo* msg, void*) {
    if (msg->type != AngelScript::asMSGTYPE_ERROR) {
      return;
    }
    if (g_errors.size() < 5) {
      g_errors.push_back(std::string(msg->section ? msg->section : "") + ":" + std::to_string(msg->row) + " " + msg->message);
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

  /// A real engine with the add-ons and the custom types, set up the way as_script.hpp does it.
  AngelScript::asIScriptEngine* MakeEngine(const bool watch_errors = false) {
    auto* engine = AngelScript::asCreateScriptEngine();
    EXPECT_NE(engine, nullptr);

    g_error_count = 0;
    g_errors.clear();
    if (watch_errors) {
      engine->SetMessageCallback(AngelScript::asFUNCTION(CollectErrors), nullptr, AngelScript::asCALL_CDECL);
    }

    as_util::ClearDocs();
    as_util::RegisterAddOns(engine);
    Register(engine);
    return engine;
  }

  /// Where the stubs below leave the values a script produced.
  struct Host {
    rage::Vector3 vec;
    rage::Vector3 other;
    int i = 0;
    float f = 0.0f;
    bool b = false;
    base::menu::natives::Hash hash = 0;
    std::uint32_t taken_hash = 0;
  };

  Host g_host;

  // ---- Stand-ins for the natives, with the C++ signatures the generated bindings declare.

  rage::Vector3 ReturnVector3() { return rage::Vector3(1.5f, -2.5f, 3.5f); }

  void FillVector3(rage::Vector3& out) { out = rage::Vector3(9.0f, 8.0f, 7.0f); }
  void FillInt(int& out) { out = 1234; }
  void FillFloat(float& out) { out = 2.5f; }
  void FillBool(bool& out) { out = true; }
  void FillHash(base::menu::natives::Hash& out) { out = 0xDEADBEEFu; }

  void TakeHash(const base::menu::natives::Hash value) { g_host.taken_hash = value; }

  void HostPoke(const rage::Vector3& value) { g_host.other = value; }

  /// Registers the stubs and the globals the scripts read from and write to.
  void RegisterStubs(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("test");

    // The custom types live in the base native namespace, so the stubs spell them out the way the
    // generated declarations do.
    RegisterGlobalProperty(engine, "natives::Vector3 g_vec", &g_host.vec);
    RegisterGlobalProperty(engine, "natives::Vector3 g_other", &g_host.other);
    RegisterGlobalProperty(engine, "int g_int", &g_host.i);
    RegisterGlobalProperty(engine, "float g_float", &g_host.f);
    RegisterGlobalProperty(engine, "bool g_bool", &g_host.b);
    RegisterGlobalProperty(engine, "natives::Hash g_hash", &g_host.hash);

    // asFUNCTION expands to an unqualified asFunctionPtr, which only resolves here for the stub that
    // takes an asSMessageInfo argument, so the prefix is what makes these compile.
    RegisterGlobalFunction(engine, "natives::Vector3 ReturnVector3()", AngelScript::asFUNCTION(ReturnVector3), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void FillVector3(natives::Vector3&out)", AngelScript::asFUNCTION(FillVector3), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void FillInt(int&out)", AngelScript::asFUNCTION(FillInt), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void FillFloat(float&out)", AngelScript::asFUNCTION(FillFloat), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void FillBool(bool&out)", AngelScript::asFUNCTION(FillBool), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void FillHash(natives::Hash&out)", AngelScript::asFUNCTION(FillHash), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void TakeHash(natives::Hash)", AngelScript::asFUNCTION(TakeHash), AngelScript::asCALL_CDECL);
    RegisterGlobalFunction(engine, "void HostPoke(const natives::Vector3 &in)", AngelScript::asFUNCTION(HostPoke), AngelScript::asCALL_CDECL);

    engine->SetDefaultNamespace("");
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

  AngelScript::asIScriptEngine* EngineWithStubs() {
    auto* engine = MakeEngine();
    RegisterStubs(engine);
    return engine;
  }

  std::string ReadFile(const std::filesystem::path& path) {
    std::ifstream stream{path};
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
  }

  std::vector<std::string> SplitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream{text};
    for (std::string line; std::getline(stream, line);) {
      if (!line.empty() && line.back() == '\r') line.pop_back();
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
}

// ---------------------------------------------------------------- the ABI of each mapping

// rage::Vector3 is alignas(16), but the vendored AngelScript compiles with WIP_16BYTE_ALIGN
// undefined, so the buffer it hands a returned value type is only 4-byte aligned. Whether that
// faults depends on whether MSVC emits aligned moves for the copy, which cannot be settled by
// reading the source - only by running it. Run this in Release as well as Debug.
TEST(as_natives, a_vector3_returned_by_value_survives_the_script_boundary) {
  auto* engine = EngineWithStubs();
  g_host.vec = rage::Vector3{};

  const auto result = RunScript(engine, "test::g_vec = test::ReturnVector3();");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_host.vec.x, 1.5f);
  EXPECT_FLOAT_EQ(g_host.vec.y, -2.5f);
  EXPECT_FLOAT_EQ(g_host.vec.z, 3.5f);
}

// The other half of the same risk: a Vector3&out reference is a pointer, and the callee writes a
// 16-byte aligned value through it.
TEST(as_natives, a_vector3_out_parameter_is_filled_in) {
  auto* engine = EngineWithStubs();
  g_host.vec = rage::Vector3{};

  const auto result = RunScript(engine, "test::FillVector3(test::g_vec);");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_host.vec.x, 9.0f);
  EXPECT_FLOAT_EQ(g_host.vec.y, 8.0f);
  EXPECT_FLOAT_EQ(g_host.vec.z, 7.0f);
}

// A Vector3 is also passed by reference into a native, which is the shape of the Vector3* the
// database declares.
TEST(as_natives, a_vector3_reference_parameter_reaches_the_callee) {
  auto* engine = EngineWithStubs();

  const auto result = RunScript(engine, "test::HostPoke(natives::Vector3(4.0f, 5.0f, 6.0f));");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_host.other.x, 4.0f);
  EXPECT_FLOAT_EQ(g_host.other.y, 5.0f);
  EXPECT_FLOAT_EQ(g_host.other.z, 6.0f);
}

// int* / float* / bool* in the database are spelled `int &out` and friends. A reference is passed as
// a pointer, so these register with no shim at all.
TEST(as_natives, out_parameters_map_to_references) {
  auto* engine = EngineWithStubs();
  g_host.i = 0;
  g_host.f = 0.0f;
  g_host.b = false;
  g_host.hash = 0;

  const auto result = RunScript(engine,
                                "test::FillInt(test::g_int);\n"
                                "test::FillFloat(test::g_float);\n"
                                "test::FillBool(test::g_bool);\n"
                                "test::FillHash(test::g_hash);");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.i, 1234);
  EXPECT_FLOAT_EQ(g_host.f, 2.5f);
  EXPECT_TRUE(g_host.b);
  EXPECT_EQ(g_host.hash, 0xDEADBEEFu);
}

// The whole reason Hash is a type of its own: one conversion constructor on the target type means a
// string argument becomes a joaat hash with no per-native overloads. This is what would break
// silently if a copy constructor were ever registered alongside it.
TEST(as_natives, hashes_are_built_from_strings) {
  auto* engine = EngineWithStubs();
  g_host.taken_hash = 0;

  const auto result = RunScript(engine, "test::TakeHash(\"MY_HASH\");");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.taken_hash, base::common::conversion::joaat("MY_HASH"));
}

// Numeric literals reach a Hash parameter through the other conversion constructor.
TEST(as_natives, hashes_are_built_from_numbers) {
  auto* engine = EngineWithStubs();
  g_host.taken_hash = 0;

  const auto result = RunScript(engine, "test::TakeHash(0x1234);");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_EQ(g_host.taken_hash, 0x1234u);
}

// A Vector3 method returning a Vector3 is the one method shape where AngelScript and the game are
// most likely to disagree, so the operators the vectors expose are worth pinning down.
TEST(as_natives, vector3_operators_return_values) {
  auto* engine = EngineWithStubs();
  g_host.vec = rage::Vector3{};

  const auto result = RunScript(engine,
                                "natives::Vector3 a = natives::Vector3(1.0f, 2.0f, 3.0f);\n"
                                "natives::Vector3 b = natives::Vector3(10.0f, 20.0f, 30.0f);\n"
                                "test::g_vec = (a + b) * 2.0f - b;");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_host.vec.x, 12.0f);
  EXPECT_FLOAT_EQ(g_host.vec.y, 24.0f);
  EXPECT_FLOAT_EQ(g_host.vec.z, 36.0f);
}

TEST(as_natives, vector3_helpers_and_comparison) {
  auto* engine = EngineWithStubs();

  const auto result = RunScript(engine,
                                "natives::Vector3 a = natives::Vector3(3.0f, 4.0f, 0.0f);\n"
                                "test::g_float = a.GetMagnitude();\n"
                                "test::g_bool = a == natives::Vector3(3.0f, 4.0f, 0.0f);\n"
                                "test::g_int = int(natives::Vector3(0.0f, 0.0f, 0.0f).GetDistance(natives::Vector3(0.0f, 0.0f, 5.0f)));");
  engine->ShutDownAndRelease();
  ASSERT_TRUE(result);

  EXPECT_FLOAT_EQ(g_host.f, 5.0f);
  EXPECT_TRUE(g_host.b);
  EXPECT_EQ(g_host.i, 5);
}

// ---------------------------------------------------------------- the generated surface

TEST(as_natives, the_custom_types_are_registered) {
  auto* engine = MakeEngine();

  for (const auto* name : {"natives::Vector2", "natives::Vector3", "natives::Vector4", "natives::Hash", "natives::NativeHandle"}) {
    EXPECT_NE(engine->GetTypeInfoByDecl(name), nullptr) << name;
  }
  engine->ShutDownAndRelease();
}

// Every declaration the generator emits has to be one the engine accepts. A type spelled wrong, or a
// parameter named after an AngelScript keyword, fails here rather than in the game.
TEST(as_natives, every_generated_declaration_registers_without_error) {
  auto* engine = MakeEngine(true);

  natives::RegisterAngelScript(engine);

  EXPECT_EQ(g_error_count, 0U) << ReportedErrors();
  engine->ShutDownAndRelease();
}

// The names a script actually types: group sub-namespace, PascalCase native.
TEST(as_natives, natives_are_reachable_under_their_group_namespace) {
  auto* engine = MakeEngine(true);
  natives::RegisterAngelScript(engine);
  EXPECT_EQ(g_error_count, 0U) << ReportedErrors();

  // Only the script's own diagnostics are of interest from here on.
  g_error_count = 0;
  g_errors.clear();

  AngelScript::asIScriptModule* mod = engine->GetModule("Probe", AngelScript::asGM_ALWAYS_CREATE);
  mod->AddScriptSection("probe.as",
                        "int main() {\n"
                        "  return natives::ped::GetPedArmour(natives::player::PlayerPedId());\n"
                        "}\n");
  const int built = mod->Build();
  engine->ShutDownAndRelease();

  EXPECT_GE(built, 0) << ReportedErrors();
}

// The NativeDB prose has to land above the declaration, which is what angel-lsp reads for hover.
TEST(as_natives, native_docs_land_on_their_declarations) {
  auto* engine = MakeEngine(true);
  natives::RegisterAngelScript(engine);

  const auto path = std::filesystem::temp_directory_path() / "gta_base_as_natives_test" / "as.predefined";
  std::filesystem::create_directories(path.parent_path());
  as_util::GenerateScriptPredefined(engine, path);
  engine->ShutDownAndRelease();

  ASSERT_EQ(g_error_count, 0U) << ReportedErrors();

  const auto lines = SplitLines(ReadFile(path));

  bool found_doc = false;
  for (std::size_t i = 1; i < lines.size(); ++i) {
    if (Trimmed(lines[i]) != "void AppSetApp(const std::string&in appName);") {
      continue;
    }
    EXPECT_EQ(Trimmed(lines[i - 1]), "*/") << "AppSetApp has no doc block above it";
    found_doc = true;
  }
  EXPECT_TRUE(found_doc) << "AppSetApp was not found in the predefined dump";
}
