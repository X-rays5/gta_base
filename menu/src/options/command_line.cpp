//
// Created by X-ray on 12/09/2026.
//

#include "command_line.hpp"
#include "base_command.hpp"

#include <charconv>
#include <exception>
#include <string>
#include <utility>

#include <fmt/format.h>

// Turning a command line into a declared argument list and back. Pure C++: nothing here knows about
// scripts, and the parser is the only thing that knows what a command line may say, so a command's
// arguments are declared once and both its parsing and its help text are written from that.
namespace base::menu::options {
  namespace {
    bool IsSpace(const char c) {
      return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    /// Whether `c` is one a backslash escapes outside quotes. A backslash in front of anything else is
    /// itself, which is what keeps a Windows path a path.
    bool IsEscapable(const char c) {
      return c == '"' || c == '\'' || c == '\\' || IsSpace(c);
    }

    /// The same name with its leading dashes taken off, so that "--level" and "level" are one name.
    std::string_view WithoutDashes(std::string_view name) {
      while (!name.empty() && name.front() == '-') {
        name.remove_prefix(1);
      }
      return name;
    }

    /// `text` read as a number. False when it is not one, which leaves `out` alone.
    bool ParseInteger(const std::string_view text, std::int64_t& out) {
      const auto* const first = text.data();
      const auto* const last = text.data() + text.size();
      return std::from_chars(first, last, out).ec == std::errc{};
    }

    bool ParseNumber(const std::string_view text, double& out) {
      const auto* const first = text.data();
      const auto* const last = text.data() + text.size();
      return std::from_chars(first, last, out).ec == std::errc{};
    }

    /// The value of `name` as the declared type says it should be read, which is the only read that
    /// cannot be a type error.
    void ReadDeclared(const char type, const argparse::ArgumentParser& parser, const std::string& name, ParsedArg& arg) {
      switch (type) {
        case 'i': {
          arg.integer = parser.get<int>(name);
          arg.number = static_cast<double>(arg.integer);
          break;
        }
        case 'u': {
          arg.integer = static_cast<std::int64_t>(parser.get<unsigned>(name));
          arg.number = static_cast<double>(arg.integer);
          break;
        }
        case 'f': {
          arg.number = static_cast<double>(parser.get<float>(name));
          arg.integer = static_cast<std::int64_t>(arg.number);
          break;
        }
        case 'd': {
          arg.number = parser.get<double>(name);
          arg.integer = static_cast<std::int64_t>(arg.number);
          break;
        }
        case 'b': {
          // Registered as a flag, so this is the bool the parser stored and never a string.
          arg.boolean = parser.get<bool>(name);
          break;
        }
        case 's': {
          arg.text = parser.get<std::string>(name);
          // Read as a number as well when it happens to be one: a script that declared the argument
          // as text and reads it as a number is asking a question this can answer, and it can answer
          // it here rather than making every getter a conversion that can fail.
          static_cast<void>(ParseInteger(arg.text, arg.integer));
          static_cast<void>(ParseNumber(arg.text, arg.number));
          break;
        }
        default:
          // Unreachable for a command whose arguments have been through IsValidArgType, and a
          // declaration that skips the check is the only way here, so it is worth saying so.
          LOG_ERROR("[Options] The argument '{}' of a command is declared as '{}', which is not a type a value is read as", name, type);
          break;
      }
    }

    /// The spelling of a value in the shape it was declared with, which is the text every getter of
    /// it can fall back to.
    std::string ValueAsText(const ParsedArg& arg) {
      switch (arg.type) {
        case 'b':
          return arg.boolean ? "true" : "false";
        case 'f':
        case 'd':
          return fmt::format("{}", arg.number);
        case 'i':
        case 'u':
          return fmt::format("{}", arg.integer);
        default:
          return arg.text;
      }
    }
  }

  bool IsValidArgType(const char type) {
    return type == 'i' || type == 'u' || type == 'f' || type == 'd' || type == 's' || type == 'b';
  }

  std::string ArgTypeName(const char type) {
    switch (type) {
      case 'i': return "int";
      case 'u': return "uint";
      case 'f': return "float";
      case 'd': return "double";
      case 's': return "string";
      case 'b': return "bool";
      default: return fmt::format("[{}]", type);
    }
  }

  const ParsedArg* FindArg(const std::vector<ParsedArg>& args, const std::string_view name) {
    for (const auto& arg : args) {
      if (arg.name == name) {
        return &arg;
      }
    }

    for (const auto& arg : args) {
      if (WithoutDashes(arg.name) == WithoutDashes(name)) {
        return &arg;
      }
    }

    return nullptr;
  }

  std::optional<std::vector<std::string>> TokenizeCommandLine(const std::string_view line) {
    std::vector<std::string> tokens;
    std::string token;
    bool token_started = false;

    // Which quote is open, if any: a run inside one ends at its own quote rather than at whitespace.
    char quote = 0;

    for (std::size_t i = 0; i < line.size(); ++i) {
      const char c = line[i];

      if (quote == '"') {
        if (c == '\\' && i + 1 < line.size() && (line[i + 1] == '"' || line[i + 1] == '\\')) {
          token += line[i + 1];
          ++i;
        } else if (c == '"') {
          quote = 0;
        } else {
          token += c;
        }
        continue;
      }

      if (quote == '\'') {
        if (c == '\'') {
          quote = 0;
        } else {
          token += c;
        }
        continue;
      }

      if (c == '\\' && i + 1 < line.size() && IsEscapable(line[i + 1])) {
        token += line[i + 1];
        ++i;
        token_started = true;
        continue;
      }

      if (c == '"' || c == '\'') {
        quote = c;
        // So that "" is one empty argument rather than no argument at all.
        token_started = true;
        continue;
      }

      if (IsSpace(c)) {
        if (token_started) {
          tokens.push_back(std::move(token));
          token.clear();
          token_started = false;
        }
        continue;
      }

      token += c;
      token_started = true;
    }

    if (quote != 0) {
      return std::nullopt;
    }

    if (token_started) {
      tokens.push_back(std::move(token));
    }

    return tokens;
  }

  std::shared_ptr<argparse::ArgumentParser> BuildParser(const BaseCommand& command) {
    // No default arguments of argparse's: its --help and --version print to stdout and end the
    // process, and a command line here can come from a script.
    auto parser = std::make_shared<argparse::ArgumentParser>(command.GetName(), "1.0", argparse::default_arguments::none);

    const auto description = command.GetDescription();
    if (!description.empty()) {
      parser->add_description(description);
    }

    for (const auto& declared : command.GetArgs()) {
      auto& arg = parser->add_argument(declared.name);
      if (!declared.help.empty()) {
        arg.help(declared.help);
      }

      // No default value on anything but a flag. Giving an argument one also relaxes it to take no
      // value at all, so "--count" on its own would be read as an argument that was given without
      // one and would quietly mean zero; without it, a value argument means a value argument and
      // naming one and leaving its value out is refused. That an argument the line left out answers
      // with the zero of its type is ReadArgs' doing rather than the parser's.
      //
      // A flag is the exception, and has to be: it is the type that takes no value, so false is what
      // an absent one means and true is what being there means.
      switch (declared.type) {
        case 'i': arg.scan<'i', int>(); break;
        case 'u': arg.scan<'u', unsigned>(); break;
        case 'f': arg.scan<'g', float>(); break;
        case 'd': arg.scan<'g', double>(); break;
        case 's': break;
        case 'b': arg.default_value(false).implicit_value(true); break;
        default:
          LOG_ERROR("[Options] '{}' declares '{}' without a type, so it takes no value", command.GetName(), declared.name);
          break;
      }
    }

    return parser;
  }

  std::vector<ParsedArg> ReadArgs(const BaseCommand& command, const argparse::ArgumentParser& parser) {
    std::vector<ParsedArg> args;

    for (const auto& declared : command.GetArgs()) {
      ParsedArg arg;
      arg.name = declared.name;
      arg.type = declared.type;

      try {
        arg.used = parser.is_used(declared.name);
        // Only what the command line carried is read: the parser has no default to answer with, so
        // asking it for an argument that was not there would raise rather than answer zero. What an
        // absent argument leaves behind is the zero of its type, which is what makes the getters a
        // script reads its callback's arguments with total.
        if (arg.used) {
          ReadDeclared(declared.type, parser, declared.name, arg);
        }

        // For the string case this is the parsed text, for everything else the value written out, so
        // that every argument can answer as text whichever type it was declared with.
        if (arg.text.empty()) {
          arg.text = ValueAsText(arg);
        }
      } catch (const std::exception& e) {
        LOG_ERROR("[Options] The argument '{}' of '{}' could not be read back from its command line: {}",
                  declared.name, command.GetName(), e.what());
      }

      args.push_back(std::move(arg));
    }

    return args;
  }

  std::shared_ptr<argparse::ArgumentParser> ParseArgs(const BaseCommand& command, const std::string_view line) {
    const auto tokens = TokenizeCommandLine(line);
    if (!tokens) {
      LOG_ERROR("[Options] '{}' was given a command line with a quote left open, so nothing was run: {}",
                command.GetName(), line);
      return nullptr;
    }

    const auto parser = command.GetParser();

    // The command's own name goes in front of what was typed: argparse takes the first entry of the
    // list it is given for the program name and starts reading at the second, so a command line
    // passed as it was written would lose its first argument to that.
    std::vector<std::string> arguments;
    arguments.reserve(tokens->size() + 1);
    arguments.push_back(command.GetName());
    arguments.insert(arguments.end(), tokens->begin(), tokens->end());

    try {
      parser->parse_args(arguments);
    } catch (const std::exception& e) {
      // The parser's own message is what says which argument it was and what was wrong with it, so it
      // is carried through rather than replaced by one of ours.
      LOG_ERROR("[Options] '{}' was given a command line it does not take ({}), so nothing was run: {}",
                command.GetName(), e.what(), line);
      return nullptr;
    }

    return parser;
  }

  bool ParseAndRun(BaseCommand& command, const std::string_view line) {
    const auto parser = ParseArgs(command, line);
    if (!parser) {
      return false;
    }

    command.execute(parser);
    return true;
  }

  std::string FormatHelp(const BaseCommand& command) {
    std::string help = command.GetName();

    const auto description = command.GetDescription();
    if (!description.empty()) {
      help += " - " + description;
    }

    for (const auto& arg : command.GetArgs()) {
      help += fmt::format("\n  {} <{}>", arg.name, ArgTypeName(arg.type));
      if (!arg.help.empty()) {
        help += "  " + arg.help;
      }
    }

    return help;
  }
}
