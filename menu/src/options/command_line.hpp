//
// Created by X-ray on 12/09/2026.
//

#pragma once
#include <argparse/argparse.hpp>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace base::menu::options {
  class BaseCommand;

  /// One argument's value, as it was read out of a command line.
  ///
  /// The value is kept in every shape it has, so that a reader gets an answer whichever one it asks
  /// for: an integer argument read as a number, a float read as an integer, anything read as text. A
  /// getter is then a lookup rather than a conversion that can fail.
  struct ParsedArg {
    /// As the command declares it: "--level".
    std::string name;

    /// The type char it was declared with, which is what says the shape the value is meant to have.
    char type{};

    /// Whether the command line carried it. An argument that did not is still one of these, holding
    /// the zero of its type, so a caller that wants to tell absent from zero asks this.
    bool used{false};

    /// The value as an integer, for 'i' and 'u' exactly and for the others truncated.
    std::int64_t integer{};

    /// The value as a number, for 'f' and 'd' exactly and for the others widened.
    double number{};

    /// The value as a boolean, for 'b'. For the others it is false.
    bool boolean{};

    /// The value as text, in the spelling the command line gave it.
    std::string text;
  };

  /// The argument of `args` that `name` names, or null.
  ///
  /// A name is matched as the command declared it ("--level"), and also spelled without its dashes
  /// ("level") - the spelling a callback is likeliest to reach for, and there is no second argument
  /// it could be taken for when the match is exact either way round.
  [[nodiscard]] const ParsedArg* FindArg(const std::vector<ParsedArg>& args, std::string_view name);

  /// Whether `type` is one of the type chars an argument can be declared with: 'i' for an integer,
  /// 'u' for an unsigned one, 'f' for a float, 'd' for a double, 's' for a string, 'b' for a boolean.
  [[nodiscard]] bool IsValidArgType(char type);

  /// What a type char is spelled as in help text: "int" for 'i'. An unknown char is spelled as
  /// itself, in brackets.
  [[nodiscard]] std::string ArgTypeName(char type);

  /// Split a command line into the arguments a parser is given, honouring quotes and escapes: a run of
  /// whitespace separates, `"..."` and `'...'` group, and inside double quotes `\"` and `\\` are the
  /// two characters they name. A backslash anywhere else is itself, so a Windows path survives being
  /// written down. An empty quoted run is one empty argument rather than none.
  ///
  /// Nothing at all, when a quote is left open: the rest of the line would be one argument whose
  /// value is a guess, and a command line that cannot be read as written should be refused rather
  /// than run with a value nobody typed.
  [[nodiscard]] std::optional<std::vector<std::string>> TokenizeCommandLine(std::string_view line);

  /// The parser of `command`: one argument per GetArgs(), each taking the value its type says it
  /// takes - none at all for a flag - and carrying the help text it was declared with.
  [[nodiscard]] std::shared_ptr<argparse::ArgumentParser> BuildParser(const BaseCommand& command);

  /// Every argument `command` declares, read out of a parser that BuildParser built from it.
  ///
  /// An argument that is not on the command line is read as the zero of its type rather than raising,
  /// which is what makes the getters a script reads its callback's arguments with total.
  [[nodiscard]] std::vector<ParsedArg> ReadArgs(const BaseCommand& command, const argparse::ArgumentParser& parser);

  /// `line` parsed with `command`'s parser, or null when it does not parse - which is reported as it
  /// happens, since the parser's own message is the one that says what was wrong with it.
  [[nodiscard]] std::shared_ptr<argparse::ArgumentParser> ParseArgs(const BaseCommand& command, std::string_view line);

  /// Parse `line` for `command` and run it, answering whether it ran. A command line that does not
  /// parse runs nothing at all, so a command is never executed with half its arguments.
  ///
  /// `execute` is called on this thread. A caller that is not the game thread has to hand it over
  /// first, and that is what the script binding of it does.
  bool ParseAndRun(BaseCommand& command, std::string_view line);

  /// What `command` is and what it takes, as text: its name, its description, and one line per
  /// declared argument naming the type it reads and what it is for.
  [[nodiscard]] std::string FormatHelp(const BaseCommand& command);
}
