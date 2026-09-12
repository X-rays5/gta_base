//
// Created by X-ray on 11/09/2026.
//

#include <gtest/gtest.h>

#include <angelscript.h>

#include "../../src/as/util/as_bind.hpp"
#include "../../src/as/util/as_generate_predefined.hpp"
#include "../../src/as/bindings/as_log.hpp"
#include "../../src/as/util/as_util.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {
  namespace as_util = base::menu::as::util;

  struct TestThing {
    int value;
  };

  int TestThingDoubled(TestThing* self) {
    return self->value * 2;
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

  /// Declarations whose parameter types are spelled by the engine, which is not worth pinning down.
  std::vector<std::size_t> FindDeclarationsContaining(const std::vector<std::string>& lines, const std::string& needle) {
    std::vector<std::size_t> found;
    for (std::size_t i = 0; i < lines.size(); ++i) {
      if (Trimmed(lines[i]).find(needle) != std::string::npos) found.push_back(i);
    }
    return found;
  }

  /// How often a needle occurs, used to catch a doc that was written more than once.
  std::size_t CountOccurrences(const std::string& text, const std::string& needle) {
    std::size_t count = 0;
    for (auto pos = text.find(needle); pos != std::string::npos; pos = text.find(needle, pos + needle.size())) {
      ++count;
    }
    return count;
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

  std::string GeneratePredefined() {
    AngelScript::asIScriptEngine* engine = AngelScript::asCreateScriptEngine();
    EXPECT_NE(engine, nullptr);

    // The doc registry outlives the engine, so start from a clean slate the way a second debug run
    // of the script test does.
    as_util::ClearDocs();
    as_util::RegisterAddOns(engine);
    base::menu::as::bindings::log::RegisterLog(engine);

    as_util::RegisterEnum(engine, "TestEnum")
      .Desc("An enum used to test generated documentation.");
    as_util::RegisterEnumValue(engine, "TestEnum", "First", 0)
      .Desc("The first test value.");
    as_util::RegisterObjectType(engine, "TestThing", 0, AngelScript::asOBJ_REF | AngelScript::asOBJ_NOCOUNT)
      .Desc("A type used to test generated documentation.");
    as_util::RegisterObjectProperty(engine, "TestThing", "int value", 0)
      .Desc("The stored value.");
    as_util::RegisterObjectMethod(engine, "TestThing", "int doubled()", AngelScript::asFUNCTION(TestThingDoubled), AngelScript::asCALL_CDECL_OBJFIRST)
      .Desc("Doubles the stored value.")
      .Returns("The stored value multiplied by two.");

    const auto path = std::filesystem::temp_directory_path() / "gta_base_as_docs_test" / "as.predefined";
    std::filesystem::create_directories(path.parent_path());
    as_util::GenerateScriptPredefined(engine, path);

    engine->ShutDownAndRelease();

    std::ifstream stream{path};
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
  }
}

TEST(as_docs, symbol_path_from_decl) {
  EXPECT_EQ(as_util::SymbolPathFromDecl("void info(const std::string &in)"), "info");
  EXPECT_EQ(as_util::SymbolPathFromDecl("void log::info(const std::string &in)"), "log::info");
  EXPECT_EQ(as_util::SymbolPathFromDecl("int m_value"), "m_value");
  EXPECT_EQ(as_util::SymbolPathFromDecl("std::string format(const std::string &in)"), "format");
}

TEST(as_docs, qualify_namespace) {
  EXPECT_EQ(as_util::QualifyNamespace("info", "log"), "log::info");
  EXPECT_EQ(as_util::QualifyNamespace("info", ""), "info");
  EXPECT_EQ(as_util::QualifyNamespace("log::info", "other"), "log::info");
}

TEST(as_docs, format_doc_comment) {
  as_util::Doc doc;
  doc.description = "First line.\nSecond line.";
  doc.params.push_back({"in", "The message."});
  doc.returns = "The message.";

  EXPECT_EQ(as_util::FormatDocComment(doc, "\t"),
            "\t/**\n"
            "\t * First line.\n"
            "\t * Second line.\n"
            "\t *\n"
            "\t * @param in The message.\n"
            "\t * @return The message.\n"
            "\t */\n");

  EXPECT_TRUE(as_util::FormatDocComment(as_util::Doc{}, "").empty());
}

TEST(as_docs, global_functions_get_docs) {
  const auto lines = SplitLines(GeneratePredefined());

  const auto infos = FindDeclarationsContaining(lines, "void info(");
  ASSERT_FALSE(infos.empty()) << "the info binding was not emitted";
  const auto infoDoc = DocAbove(lines, infos.front());
  EXPECT_NE(infoDoc.find("Logs an informational message to the menu console."), std::string::npos) << infoDoc;
  EXPECT_NE(infoDoc.find("@param in The message to log."), std::string::npos) << infoDoc;

  // The vendored string add-on also registers a bare 'format', so match on the return type.
  const auto formats = FindDeclarationsContaining(lines, "std::string format(");
  ASSERT_FALSE(formats.empty()) << "the format binding was not emitted";

  // Every overload of a documented name carries the same documentation.
  for (const auto info : infos) {
    EXPECT_NE(DocAbove(lines, info).find("Logs an informational message"), std::string::npos);
  }
  for (const auto format : formats) {
    const auto formatDoc = DocAbove(lines, format);
    EXPECT_NE(formatDoc.find("@param in The format string."), std::string::npos) << formatDoc;
    EXPECT_NE(formatDoc.find("@return The formatted message."), std::string::npos) << formatDoc;
  }
}

TEST(as_docs, a_second_registration_pass_rewrites_the_docs) {
  GeneratePredefined();
  const auto second = GeneratePredefined();

  // Registration appends parameters, so a pass that failed to drop the previous docs would leave
  // every documented function with two copies of each @param. Both counts are per overload, since
  // the shared doc is written above each one: 2 for log::format, 4 functions x 2 for the loggers.
  EXPECT_EQ(CountOccurrences(second, "@param in The format string."), 2U);
  EXPECT_EQ(CountOccurrences(second, "@param in The message to log."), 8U);
}

TEST(as_docs, types_and_members_get_docs) {
  const auto lines = SplitLines(GeneratePredefined());

  const auto type = FindDeclaration(lines, "class TestThing{");
  ASSERT_NE(type, std::string::npos) << "the test type was not emitted";
  EXPECT_NE(DocAbove(lines, type).find("A type used to test generated documentation."), std::string::npos);

  const auto property = FindDeclaration(lines, "int value;");
  ASSERT_NE(property, std::string::npos);
  EXPECT_NE(DocAbove(lines, property).find("The stored value."), std::string::npos);

  const auto method = FindDeclaration(lines, "int doubled();");
  ASSERT_NE(method, std::string::npos);
  const auto methodDoc = DocAbove(lines, method);
  EXPECT_NE(methodDoc.find("Doubles the stored value."), std::string::npos) << methodDoc;
  EXPECT_NE(methodDoc.find("@return The stored value multiplied by two."), std::string::npos) << methodDoc;

  const auto enumeration = FindDeclaration(lines, "enum TestEnum {");
  ASSERT_NE(enumeration, std::string::npos);
  EXPECT_NE(DocAbove(lines, enumeration).find("An enum used to test generated documentation."), std::string::npos);

  const auto enumValue = FindDeclaration(lines, "First");
  ASSERT_NE(enumValue, std::string::npos);
  EXPECT_NE(DocAbove(lines, enumValue).find("The first test value."), std::string::npos);
}
