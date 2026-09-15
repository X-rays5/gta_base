//
// Created by X-ray on 12/09/2026.
//

#include <gtest/gtest.h>

// base_command.hpp is written against the menu precompiled header: it names LOG_ERROR, and through
// GetDescription it names the localization manager, which is itself written against the pch's Status.
// A test is not that translation unit, so the two names are brought in here instead.
#include <base-common/logging/logging_macro.hpp>
#include <base-common/util/result.hpp>

#include <argparse/argparse.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../../src/options/base_command.hpp"
#include "../../src/options/command_line.hpp"

// The command line a command is run with: how one is split into arguments, and what a command's
// declared arguments make of them. Both halves are pure C++ and both are what a script's options
// stand on, so they are pinned here rather than through an engine.
namespace {
  using base::menu::options::ArgTypeName;
  using base::menu::options::BaseCommand;
  using base::menu::options::CommandArg;
  using base::menu::options::FindArg;
  using base::menu::options::FormatHelp;
  using base::menu::options::IsValidArgType;
  using base::menu::options::ParseArgs;
  using base::menu::options::ParsedArg;
  using base::menu::options::ReadArgs;
  using base::menu::options::ParseAndRun;
  using base::menu::options::TokenizeCommandLine;

  /// A command that declares what the test tells it to and records the command line it was run with,
  /// so that a test asserts on what the parser made of a line rather than on the parser itself.
  class TestCommand final : public BaseCommand {
  public:
    TestCommand() : BaseCommand("test_option", "") {}

    explicit TestCommand(std::vector<CommandArg> args) : BaseCommand("test_option", ""), args_(std::move(args)) {}

    std::vector<CommandArg> GetArgs() const override {
      return args_;
    }

    void execute(std::shared_ptr<argparse::ArgumentParser> args) override {
      ++runs_;
      parsed_ = ReadArgs(*this, *args);
    }

    [[nodiscard]] int Runs() const {
      return runs_;
    }

    [[nodiscard]] const std::vector<ParsedArg>& Parsed() const {
      return parsed_;
    }

  private:
    std::vector<CommandArg> args_;
    int runs_{0};
    std::vector<ParsedArg> parsed_;
  };

  /// The six type chars, one argument each, as a command a test can run a line against.
  std::vector<CommandArg> AllTypes() {
    return {
        {"--count", 'i', "How many."},
        {"--size", 'u', ""},
        {"--ratio", 'f', ""},
        {"--precise", 'd', ""},
        {"--name", 's', ""},
        {"--flag", 'b', ""},
    };
  }

  std::vector<std::string> Tokenize(const std::string& line) {
    const auto tokens = TokenizeCommandLine(line);
    EXPECT_TRUE(tokens.has_value()) << "the line should have tokenized: " << line;
    return tokens.value_or(std::vector<std::string>{});
  }
}

TEST(CommandLine, a_line_splits_on_runs_of_whitespace) {
  EXPECT_TRUE(Tokenize("").empty());
  EXPECT_TRUE(Tokenize("   \t ").empty());

  const auto tokens = Tokenize("--count  7\t--name  value");
  ASSERT_EQ(tokens.size(), 4u);
  EXPECT_EQ(tokens[0], "--count");
  EXPECT_EQ(tokens[1], "7");
  EXPECT_EQ(tokens[2], "--name");
  EXPECT_EQ(tokens[3], "value");
}

TEST(CommandLine, quotes_hold_an_argument_together) {
  const auto double_quoted = Tokenize("--name \"a b c\"");
  ASSERT_EQ(double_quoted.size(), 2u);
  EXPECT_EQ(double_quoted[1], "a b c");

  const auto single_quoted = Tokenize("--name 'a b c'");
  ASSERT_EQ(single_quoted.size(), 2u);
  EXPECT_EQ(single_quoted[1], "a b c");

  // An empty run is an argument that is there and empty, which is not the same as no argument: a
  // script clearing a name has to be able to say so.
  const auto empty = Tokenize("--name \"\"");
  ASSERT_EQ(empty.size(), 2u);
  EXPECT_EQ(empty[1], "");

  // A quote in the middle of a token opens a run and the token carries on after it closes.
  const auto joined = Tokenize("--name a\"b c\"d");
  ASSERT_EQ(joined.size(), 2u);
  EXPECT_EQ(joined[1], "ab cd");
}

TEST(CommandLine, an_escape_names_the_character_it_escapes) {
  const auto quote = Tokenize("\"a\\\"b\"");
  ASSERT_EQ(quote.size(), 1u);
  EXPECT_EQ(quote[0], "a\"b");

  const auto space = Tokenize("--name a\\ b");
  ASSERT_EQ(space.size(), 2u);
  EXPECT_EQ(space[1], "a b");

  // A backslash that escapes nothing is itself, which is what keeps a Windows path a path.
  const auto path = Tokenize("--name C:\\scripts\\run.as");
  ASSERT_EQ(path.size(), 2u);
  EXPECT_EQ(path[1], "C:\\scripts\\run.as");
}

TEST(CommandLine, a_quote_left_open_is_not_a_command_line) {
  EXPECT_FALSE(TokenizeCommandLine("--name \"a b").has_value());
  EXPECT_FALSE(TokenizeCommandLine("--name 'a b").has_value());
  // The closing quote of the first run does not close the second.
  EXPECT_FALSE(TokenizeCommandLine("'a' \"b").has_value());
}

TEST(CommandLine, a_type_char_is_only_valid_when_it_is_a_type) {
  for (const char type : {'i', 'u', 'f', 'd', 's', 'b'}) {
    EXPECT_TRUE(IsValidArgType(type)) << "a declared type should be valid: " << type;
  }

  EXPECT_FALSE(IsValidArgType('x'));
  EXPECT_FALSE(IsValidArgType('-'));
  EXPECT_FALSE(IsValidArgType('\0'));

  EXPECT_EQ(ArgTypeName('i'), "int");
  EXPECT_EQ(ArgTypeName('b'), "bool");
  EXPECT_EQ(ArgTypeName('x'), "[x]") << "an unknown type should still be printable";
}

TEST(CommandLine, every_declared_type_reads_back_as_the_value_it_was_given) {
  TestCommand command(AllTypes());

  ASSERT_TRUE(ParseAndRun(command, "--count 7 --size 3 --ratio 1.5 --precise 2.25 --name value --flag"));
  ASSERT_EQ(command.Runs(), 1);

  const auto& parsed = command.Parsed();
  ASSERT_EQ(parsed.size(), 6u);

  for (const auto& arg : parsed) {
    EXPECT_TRUE(arg.used) << "the argument should have been read as given: " << arg.name;
  }

  EXPECT_EQ(FindArg(parsed, "--count")->integer, 7);
  EXPECT_EQ(FindArg(parsed, "--size")->integer, 3);
  EXPECT_DOUBLE_EQ(FindArg(parsed, "--ratio")->number, 1.5);
  EXPECT_DOUBLE_EQ(FindArg(parsed, "--precise")->number, 2.25);
  EXPECT_EQ(FindArg(parsed, "--name")->text, "value");
  EXPECT_TRUE(FindArg(parsed, "--flag")->boolean);

  // Every argument answers as text as well as in its own type, so that a script reading one as a
  // string gets what was typed rather than nothing.
  EXPECT_EQ(FindArg(parsed, "--count")->text, "7");
  EXPECT_EQ(FindArg(parsed, "--flag")->text, "true");
  EXPECT_EQ(FindArg(parsed, "--ratio")->text, "1.5");
}

TEST(CommandLine, an_argument_the_line_left_out_is_zero_and_not_used) {
  TestCommand command(AllTypes());

  ASSERT_TRUE(ParseAndRun(command, ""));
  ASSERT_EQ(command.Runs(), 1);

  const auto& parsed = command.Parsed();
  ASSERT_EQ(parsed.size(), 6u);

  for (const auto& arg : parsed) {
    EXPECT_FALSE(arg.used) << "the argument was not on the line: " << arg.name;
    EXPECT_EQ(arg.integer, 0);
    EXPECT_DOUBLE_EQ(arg.number, 0.0);
    EXPECT_FALSE(arg.boolean);

    // An absent argument answers as text too, in the spelling its own type gives zero: reading one as
    // a string has to answer, and the answer is the zero rather than nothing typed.
    const std::string zero = arg.type == 'b' ? "false" : arg.type == 's' ? "" : "0";
    EXPECT_EQ(arg.text, zero) << "an absent argument reads as the zero of its type: " << arg.name;
  }
}

TEST(CommandLine, an_argument_is_read_by_either_spelling_of_its_name) {
  TestCommand command(AllTypes());

  ASSERT_TRUE(ParseAndRun(command, "--count 7"));
  const auto& parsed = command.Parsed();

  // The dashes are how it is declared and how the parser is given it; without them is how a script is
  // likeliest to name it back.
  ASSERT_NE(FindArg(parsed, "--count"), nullptr);
  ASSERT_NE(FindArg(parsed, "count"), nullptr);
  EXPECT_EQ(FindArg(parsed, "count"), FindArg(parsed, "--count"));
  EXPECT_EQ(FindArg(parsed, "nope"), nullptr);
}

TEST(CommandLine, a_text_argument_that_holds_a_number_answers_as_that_number) {
  TestCommand command({{"--level", 's', ""}});

  ASSERT_TRUE(ParseAndRun(command, "--level 12"));
  const auto* const level = FindArg(command.Parsed(), "--level");

  ASSERT_NE(level, nullptr);
  EXPECT_EQ(level->text, "12");
  EXPECT_EQ(level->integer, 12);
  EXPECT_DOUBLE_EQ(level->number, 12.0);
}

TEST(CommandLine, a_line_the_arguments_do_not_accept_runs_nothing) {
  TestCommand command(AllTypes());

  // An argument nobody declared, a value of the wrong type, and an argument whose value is missing -
  // the last of which is a refusal rather than a zero, because a value argument means a value.
  EXPECT_FALSE(ParseAndRun(command, "--nope 1"));
  EXPECT_FALSE(ParseAndRun(command, "--count abc"));
  EXPECT_FALSE(ParseAndRun(command, "--count"));
  EXPECT_EQ(command.Runs(), 0) << "a command line that does not parse must not run the command";

  // And a line that only the parser can read, as the last case, so that a refusal to run anything is
  // not a parser that refuses everything.
  EXPECT_TRUE(ParseAndRun(command, "--count=3"));
  EXPECT_EQ(command.Runs(), 1);
  ASSERT_NE(FindArg(command.Parsed(), "--count"), nullptr);
  EXPECT_EQ(FindArg(command.Parsed(), "--count")->integer, 3);
}

TEST(CommandLine, a_line_with_a_quote_left_open_runs_nothing) {
  TestCommand command(AllTypes());

  EXPECT_FALSE(ParseAndRun(command, "--name \"a b"));
  EXPECT_EQ(command.Runs(), 0);
}

TEST(CommandLine, parsing_answers_with_the_parser_the_command_would_build_itself) {
  TestCommand command(AllTypes());

  const auto parser = ParseArgs(command, "--count 7 --flag");
  ASSERT_NE(parser, nullptr);
  EXPECT_EQ(parser->get<int>("--count"), 7);
  EXPECT_TRUE(parser->is_used("--flag"));
  EXPECT_FALSE(parser->is_used("--name"));

  // Nothing is given, so nothing is read: a caller that wants the values reads them through ReadArgs.
  EXPECT_EQ(ParseArgs(command, "--count abc"), nullptr);
}

TEST(CommandLine, help_names_the_command_its_arguments_and_their_types) {
  TestCommand described({{"--count", 'i', "How many."}, {"--precise", 'd', ""}, {"--flag", 'b', "Whether."}});

  EXPECT_EQ(FormatHelp(described),
            "test_option\n"
            "  --count <int>  How many.\n"
            "  --precise <double>\n"
            "  --flag <bool>  Whether.");
}

TEST(CommandLine, a_command_that_declares_no_arguments_has_help_for_itself_alone) {
  const TestCommand bare;

  EXPECT_EQ(FormatHelp(bare), "test_option");
  EXPECT_TRUE(bare.GetArgs().empty());
}
