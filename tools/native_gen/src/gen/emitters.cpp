#include "emitters.hpp"

#include <cctype>
#include <expected>
#include <map>
#include <optional>
#include <unordered_set>
#include <vector>

#include "../constants.hpp"
#include "../util/log.hpp"
#include "../util/util.hpp"

// The string literals in the emitters carried over from the Python generator are transcriptions
// of its output. Whitespace, tabs and blank lines are significant there - the port is verified
// byte-for-byte. The AngelScript emitter is new, so it carries no such constraint.
namespace base::tools::native_gen::gen {
  namespace {
    std::vector<const db::NativeGroup*> ActiveGroups(const config::GenerationConfig& generation, const db::NativeDb& db) {
      std::vector<const db::NativeGroup*> groups;
      groups.reserve(db.size());
      for (const auto& group : db) {
        if (!config::IsGroupExcluded(generation, group.name)) {
          groups.push_back(&group);
        }
      }
      return groups;
    }

    // "type name, type name", with the generator's BOOL -> bool normalisation applied.
    std::string FormatParams(const std::vector<db::NativeParam>& params) {
      std::string result;
      for (std::size_t i = 0; i < params.size(); ++i) {
        if (i != 0) {
          result += ", ";
        }
        result += util::NormalizeType(params[i].type);
        result += ' ';
        result += params[i].name;
      }
      return result;
    }

    std::string FormatArguments(const std::vector<db::NativeParam>& params) {
      std::string result;
      for (std::size_t i = 0; i < params.size(); ++i) {
        if (i != 0) {
          result += ", ";
        }
        result += params[i].name;
      }
      return result;
    }

    std::vector<std::string> SplitLines(const std::string& text) {
      std::vector<std::string> lines;
      std::size_t position = 0;
      while (true) {
        const auto newline = text.find('\n', position);
        const auto end = newline == std::string::npos ? text.size() : newline;
        lines.push_back(text.substr(position, end - position));
        if (newline == std::string::npos) {
          return lines;
        }
        position = newline + 1;
      }
    }

    // Block comment, or nothing at all when the database has no comment for the native.
    std::string FormatComment(const std::string& comment) {
      if (comment.empty()) {
        return {};
      }

      std::string joined;
      const auto lines = SplitLines(comment);
      for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i != 0) {
          joined += "\n\t\t * ";
        }
        joined += util::ReplaceAll(util::Strip(lines[i]), "*/", "* /");
      }
      return "\t\t/**\n\t\t * " + joined + "\n\t\t */\n";
    }

    void AppendAutoGenBanner(std::string& out) {
      out += "// Auto-generated file - DO NOT EDIT\n";
      out += "// Generated from GTA5 NativeDB data\n";
      out += "\n";
    }

    void AppendWarningsPush(std::string& out) {
      out += "#pragma warning(push)\n";
      out += "#pragma warning(disable: 4100)\n";
      out += "\n";
    }

    // --------------------------------------------------------------- AngelScript

    // Size of the raw char* out-buffers the string shims fill. The API never reports the required
    // size - one of the affected natives takes an explicit bufferSize parameter, the rest assume a
    // fixed buffer - so this is an assumption. Rockstar's own SDK documents 64-byte buffers for
    // these accessors, so this leaves a wide margin. Eleven natives are affected.
    constexpr std::size_t kCharBufferSize = 4096;

    // AngelScript reserves these as real tokens, so a parameter spelled `out` or `set` would be a
    // syntax error inside a declaration string rather than a shadowed name.
    bool IsReservedScriptWord(const std::string& name) {
      static const std::unordered_set<std::string> kWords{
          "abstract", "and",       "auto",       "bool",     "break",     "case",      "cast",
          "class",    "const",     "continue",   "default",  "do",        "double",    "else",
          "enum",     "false",     "final",      "float",    "for",       "from",      "funcdef",
          "get",      "if",        "import",     "in",       "inout",     "int",       "int8",
          "int16",    "int32",     "int64",      "interface","is",        "let",       "mixin",
          "namespace","not",       "null",       "or",       "out",       "override",  "private",
          "protected","return",    "set",        "shared",   "super",     "switch",    "this",
          "true",     "try",       "typedef",    "uint",     "uint8",     "uint16",    "uint32",
          "uint64",   "void",      "while",      "xor",
      };
      return kWords.contains(name);
    }

    std::string SafeParamName(const std::string& name) {
      return IsReservedScriptWord(name) ? name + "_" : name;
    }

    // Whitespace removed, so "const char *" and "const char*" compare equal.
    std::string CompactType(const std::string_view type) {
      std::string out;
      for (const char c : type) {
        if (std::isspace(static_cast<unsigned char>(c)) == 0) {
          out += c;
        }
      }
      return out;
    }

    // menu/src/game/native_types.hpp aliases every handle to Entity = int32_t and Any to uint32_t,
    // and AngelScript's int is the same width, so they all spell `int` on the script side.
    // AngelScript cannot typedef over a registered type, so distinct names would stop
    // PlayerPedId() feeding GetPlayerPed(). Hash and Vector3 are registered types of their own.
    bool IsIntHandleAlias(const std::string& compact) {
      static const std::unordered_set<std::string> kAliases{
          "Any",   "Blip",   "Cam",      "Entity",  "FireId",    "Interior",
          "Object","Ped",    "Pickup",   "Player",  "ScrHandle", "Vehicle",
      };
      return kAliases.contains(compact);
    }

    // The handle aliases that are classes in C++ rather than the int32_t they read as in a script.
    // Everything in the alias set is one except Any, which native_types.hpp defines as a plain
    // uint32_t: that one really is the scalar it is spelled as, and crosses on its own.
    //
    // The distinction matters because the two sides of a call disagree about a class. A script is
    // told `int` and so reads the result out of EAX, while MSVC brings a class back in memory,
    // through a pointer it takes in RCX - a register AngelScript, having no return buffer to pass,
    // fills with the call's first argument. The game's first argument written through as a return
    // slot is a fault rather than a wrong value, so a native returning one of these cannot be
    // registered directly. See BuildAsBinding for the shim that crosses for it.
    bool IsHandleClassAlias(const std::string& compact) {
      return IsIntHandleAlias(compact) && compact != "Any";
    }

    bool IsPlainPrimitive(const std::string& compact) {
      return compact == "void" || compact == "bool" || compact == "int" || compact == "float";
    }

    bool IsStringPointer(const std::string& compact) {
      return compact == "constchar*" || compact == "char*";
    }

    bool IsWritableCharBuffer(const std::string& compact) { return compact == "char*"; }

    // The AngelScript spelling of a parameter. An empty optional means the type has no ABI-safe
    // spelling, which is fatal: a wrong spelling corrupts the call rather than failing to compile.
    //
    // The string type is registered by the std add-ons while "std" is the default namespace, so its
    // script name is std::string. A bare `string` is not a type in this engine.
    std::optional<std::string> AsParamType(const std::string& compact) {
      if (compact == "constchar*") {
        return std::string{"const std::string&in"};
      }
      if (compact == "Any*") {
        return std::string{"NativeHandle@"};
      }
      if (compact == "Hash*") {
        return std::string{"Hash&out"};
      }
      if (compact == "Vector3*") {
        return std::string{"Vector3&out"};
      }
      if (compact == "int*" || compact == "float*" || compact == "bool*") {
        return compact.substr(0, compact.size() - 1) + "&out";
      }
      if (compact.size() > 1 && compact.back() == '*') {
        // int* float* bool* Hash* Vector3* Any* and the handle aliases are the whole vocabulary.
        if (IsIntHandleAlias(compact.substr(0, compact.size() - 1))) {
          return std::string{"int&out"};
        }
        return std::nullopt;
      }
      if (IsPlainPrimitive(compact) || compact == "Hash" || compact == "Vector3") {
        return compact;
      }
      // The handle aliases are all typedefs of int32_t in native_types.hpp, so a script sees them as
      // plain ints. Registering them as types of their own would stop a Ped from feeding a function
      // taking an Entity, which the natives rely on.
      if (IsIntHandleAlias(compact)) {
        return std::string{"int"};
      }
      return std::nullopt;
    }

    std::optional<std::string> AsReturnType(const std::string& compact) {
      if (compact == "Any*") {
        return std::string{"NativeHandle@"};
      }
      if (IsIntHandleAlias(compact)) {
        return std::string{"int"};
      }
      if (IsPlainPrimitive(compact) || compact == "Hash" || compact == "Vector3") {
        return compact;
      }
      return std::nullopt;
    }

    // The C++ spelling of a type. The shims sit outside base::menu::natives, so the aliases have to
    // be qualified where they are used there.
    std::string CppType(const std::string& compact) {
      if (compact == "constchar*") {
        return "const char*";
      }
      if (compact == "char*") {
        return "char*";
      }
      if (compact == "Vector3") {
        return "::base::menu::natives::Vector3";
      }
      if (IsIntHandleAlias(compact) || compact == "Hash") {
        return "::base::menu::natives::" + compact;
      }
      if (compact.size() > 1 && compact.back() == '*') {
        const std::string base = compact.substr(0, compact.size() - 1);
        if (base == "Vector3") {
          return "::base::menu::natives::Vector3*";
        }
        if (IsIntHandleAlias(base) || base == "Hash") {
          return "::base::menu::natives::" + base + "*";
        }
        return base + "*";
      }
      return compact;
    }

    // The NativeDB comment is unstructured prose, so it is the only thing worth emitting: the
    // database carries no parameter or return descriptions.
    std::string FormatAsDoc(const std::string& comment) {
      std::vector<std::string> lines;
      for (const auto& line : SplitLines(comment)) {
        lines.push_back(util::Strip(line));
      }
      while (!lines.empty() && lines.front().empty()) {
        lines.erase(lines.begin());
      }
      while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
      }

      std::string joined;
      for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i != 0) {
          joined += "\n";
        }
        joined += lines[i];
      }
      return joined;
    }
  }

  std::string EmitNativesHeader(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db) {
    const auto& generation = config.generation;

    std::string out;
    out += "#pragma once\n";
    out += "\n";
    AppendWarningsPush(out);
    AppendAutoGenBanner(out);

    if (!generation.header_includes.empty()) {
      for (const auto& include : generation.header_includes) {
        out += include;
        out += '\n';
      }
      out += "\n";
    }

    out += "namespace " + generation.base_namespace + " {\n\n";

    for (const auto* group : ActiveGroups(generation, db)) {
      out += "\tnamespace " + group->name + " {\n\n";

      for (const auto& native : group->natives) {
        out += FormatComment(native.comment);
        const std::string params = FormatParams(native.params);
        if (crossmap.Has(native.hash)) {
          out += "\t\t" + util::NormalizeType(native.return_type) + " " + native.name + "(" + params + ");\n";
        } else {
          out += "\t\tvoid " + native.name + "(" + params + "); // Missing in crossmap\n";
        }
      }

      out += "\n\t} // namespace " + group->name + "\n\n";
    }

    out += "} // namespace " + generation.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  std::string EmitNativesImpl(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db) {
    const auto& generation = config.generation;

    std::string out;
    AppendWarningsPush(out);
    AppendAutoGenBanner(out);
    out += "#include \"" + std::string(kOutputHeader) + "\"\n";
    out += "\n";
    out += "namespace " + generation.base_namespace + " {\n";
    out += "\n";

    for (const auto* group : ActiveGroups(generation, db)) {
      out += "\tnamespace " + group->name + " {\n\n";

      for (const auto& native : group->natives) {
        const std::string params = FormatParams(native.params);
        out += "\t\t";
        if (!crossmap.Has(native.hash)) {
          out += "void " + native.name + "(" + params + ") {\n\t\t\t" + generation.log_native_error_func +
                 "(\"[NATIVES] Missing in crossmap: " + native.name + "\");\n\t\t}\n\n";
          continue;
        }

        const std::string return_type = util::NormalizeType(native.return_type);
        std::string invoke = generation.invoker_function + "<" + return_type + ", " +
                             std::to_string(crossmap.IndexOf(native.hash)) + ">(" + FormatArguments(native.params) +
                             ")";
        out += return_type + " " + native.name + "(" + params + ") {\n\t\t\t";
        out += return_type == "void" ? invoke + ";" : "return " + invoke + ";";
        out += "\n\t\t}\n\n";
      }

      out += "\t} // namespace " + group->name + "\n\n";
    }

    out += "} // namespace " + generation.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  std::string EmitCrossmapHeader(const config::Config& config, const db::Crossmap& crossmap) {
    std::string out;
    out += "#pragma once\n";
    out += "\n";
    AppendWarningsPush(out);
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// Native crossmap array for index-based lookup\n";
    out += "\n";
    out += "#include <cstdint>\n";
    out += "\n";
    out += "namespace " + config.generation.base_namespace + " {\n";
    out += "\n";

    if (crossmap.empty()) {
      out += "\t// No crossmap data available\n";
      out += "\tconstexpr std::size_t NATIVE_CROSSMAP_SIZE = 0;\n";
    } else {
      out += "\t// Crossmap array size\n";
      out += "\tconstexpr std::size_t NATIVE_CROSSMAP_SIZE = " + std::to_string(crossmap.entries.size()) + ";\n\n";
      out += "\t// Native crossmap array declaration\n";
      out += "\textern const std::uint64_t NATIVE_CROSSMAP[NATIVE_CROSSMAP_SIZE];\n";
    }

    out += "\n} // namespace " + config.generation.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  std::string EmitCrossmapImpl(const config::Config& config, const db::Crossmap& crossmap) {
    std::string out;
    AppendWarningsPush(out);
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// Native crossmap array for index-based lookup\n";
    out += "\n";
    out += "#include \"" + std::string(kOutputCrossmapHeader) + "\"\n";
    out += "\n";
    out += "namespace " + config.generation.base_namespace + " {\n";
    out += "\n";

    if (crossmap.empty()) {
      out += "\t// No crossmap data available\n";
    } else {
      out += "\t// Native crossmap array definition\n";
      out += "\tconst std::uint64_t NATIVE_CROSSMAP[NATIVE_CROSSMAP_SIZE] = {\n";
      for (std::size_t i = 0; i < crossmap.entries.size(); ++i) {
        const auto& [hash1, hash2] = crossmap.entries[i];
        const char* comma = i + 1 < crossmap.entries.size() ? "," : "";
        out += "\t\t" + hash2 + comma + "  // Index " + std::to_string(i) + ": " + hash1 + " -> " + hash2 + "\n";
      }
      out += "\t};\n";
    }

    out += "\n} // namespace " + config.generation.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  namespace {
    // One native's AngelScript surface.
    struct AsBinding {
      std::string declaration;  // "int SetPedArmour(int ped, int armour)"
      std::string symbol;       // what asFUNCTION is handed
      std::string shim;         // empty when the native registers directly
      std::string doc;          // trimmed NativeDB prose, empty when there is none or docs are off
    };

    // Maps one native onto the AngelScript surface. AngelScript takes an explicit declaration
    // string rather than deducing the signature from the function pointer, so this is where the
    // native vocabulary is spelled out - and a spelling that is wrong is silent memory corruption,
    // not a compile error. An unmapped type therefore fails the run.
    std::expected<AsBinding, std::string> BuildAsBinding(const config::GenerationConfig& generation,
                                                        const config::AngelScriptConfig& as,
                                                        const db::NativeGroup& group,
                                                        const db::Native& native) {
      const auto where = [&] { return "native " + group.name + "::" + native.name; };

      const std::string cpp_symbol = generation.base_namespace + "::" + group.name + "::" + native.name;
      // A handful of natives are named after their hash, e.g. _0x0292BD7F3766CEBC. PascalCase drops
      // the leading underscore, leaving something that starts with a digit and is not an identifier,
      // so put one back.
      std::string as_name = util::PascalCase(native.name);
      if (as_name.empty() || std::isdigit(static_cast<unsigned char>(as_name.front()))) {
        as_name.insert(as_name.begin(), '_');
      }

      std::vector<std::string> param_types;
      param_types.reserve(native.params.size());
      for (const auto& param : native.params) {
        param_types.push_back(CompactType(util::NormalizeType(param.type)));
      }
      const std::string return_type = CompactType(util::NormalizeType(native.return_type));

      bool has_char_buffer = false;
      bool has_string_input = false;
      std::size_t char_buffers = 0;
      for (const auto& compact : param_types) {
        if (IsWritableCharBuffer(compact)) {
          has_char_buffer = true;
          ++char_buffers;
        } else if (compact == "constchar*") {
          has_string_input = true;
        }
      }
      if (char_buffers > 1) {
        return std::unexpected(where() + " has " + std::to_string(char_buffers) +
                               " char* out-buffers, which the string shim cannot express");
      }

      const bool string_return = IsStringPointer(return_type);
      // A handle return needs the shim even though nothing about its spelling looks wrong: the
      // declaration says `int`, which is what a script should see, but the function behind it returns
      // a class, and that is not the same call. The shim makes the class-returning call in C++, where
      // the hidden return pointer is the compiler's own business, and hands the identity back as the
      // int the script was told to expect.
      const bool handle_return = IsHandleClassAlias(return_type);
      const bool needs_shim = has_char_buffer || has_string_input || string_return || handle_return;
      const bool shim_returns_string = has_char_buffer || string_return;

      // The out-buffer goes, and so does a bufferSize that describes it, since the shim owns the
      // buffer and knows its size. The native still receives that size on the forward call.
      const auto dropped = [&](const std::size_t index) {
        if (needs_shim && IsWritableCharBuffer(param_types[index])) {
          return true;
        }
        return has_char_buffer && native.params[index].name == "bufferSize";
      };

      AsBinding binding;
      binding.symbol = cpp_symbol;

      std::string decl_params;
      std::size_t listed = 0;
      for (std::size_t i = 0; i < native.params.size(); ++i) {
        if (dropped(i)) {
          continue;
        }
        const auto spelling = AsParamType(param_types[i]);
        if (!spelling) {
          return std::unexpected(where() + ": parameter '" + native.params[i].name +
                                 "' has unsupported type '" + native.params[i].type + "'");
        }
        if (listed != 0) {
          decl_params += ", ";
        }
        decl_params += *spelling;
        decl_params += " ";
        decl_params += SafeParamName(native.params[i].name);
        ++listed;
      }

      std::string decl_return;
      if (shim_returns_string) {
        decl_return = "std::string";
      } else {
        const auto spelling = AsReturnType(return_type);
        if (!spelling) {
          return std::unexpected(where() + ": unsupported return type '" + native.return_type + "'");
        }
        decl_return = *spelling;
      }
      binding.declaration = decl_return + " " + as_name + "(" + decl_params + ")";

      if (needs_shim) {
        const std::string shim_return =
            shim_returns_string ? "std::string" : (handle_return ? "int" : CppType(return_type));

        std::string shim_params;
        listed = 0;
        for (std::size_t i = 0; i < native.params.size(); ++i) {
          if (dropped(i)) {
            continue;
          }
          if (listed != 0) {
            shim_params += ", ";
          }
          // The script hands over a real string, so the shim takes one and converts on the way in.
          shim_params += param_types[i] == "constchar*" ? "const std::string& " : CppType(param_types[i]) + " ";
          shim_params += native.params[i].name;
          ++listed;
        }

        std::string shim_args;
        std::size_t forwarded = 0;
        for (std::size_t i = 0; i < native.params.size(); ++i) {
          if (forwarded != 0) {
            shim_args += ", ";
          }
          if (IsWritableCharBuffer(param_types[i])) {
            shim_args += "as_char_buffer";
          } else if (has_char_buffer && native.params[i].name == "bufferSize") {
            // Not a parameter of the shim, but still an argument of the native: it describes the
            // buffer the shim owns, so it forwards the size of that buffer.
            shim_args += "static_cast<" + CppType(param_types[i]) + ">(sizeof as_char_buffer)";
          } else if (param_types[i] == "constchar*") {
            shim_args += native.params[i].name + ".c_str()";
          } else {
            shim_args += native.params[i].name;
          }
          ++forwarded;
        }

        std::string body;
        if (has_char_buffer) {
          body += "char as_char_buffer[" + std::to_string(kCharBufferSize) + "]{};\n";
        }
        const std::string call = cpp_symbol + "(" + shim_args + ")";
        if (shim_returns_string) {
          if (has_char_buffer) {
            body += call + ";\n";
            body += "return std::string(as_char_buffer);\n";
          } else {
            body += "const char* const result = " + call + ";\n";
            body += "return result != nullptr ? std::string(result) : std::string();\n";
          }
        } else if (handle_return) {
          body += "return " + call + ".Get();\n";
        } else if (shim_return == "void") {
          body += call + ";\n";
        } else {
          body += "return " + call + ";\n";
        }

        std::string indented_body;
        for (const auto& line : SplitLines(body)) {
          if (line.empty()) {
            continue;
          }
          indented_body += "\t" + line + "\n";
        }

        binding.symbol = util::PascalCase(group.name) + as_name + "Shim";
        binding.shim =
            shim_return + " " + binding.symbol + "(" + shim_params + ") {\n" + indented_body + "}\n";
      }

      if (as.docs) {
        binding.doc = FormatAsDoc(native.comment);
        if (binding.doc.find(")ASDOC\"") != std::string::npos) {
          LOG_WARN("warning: {} has a comment containing the raw string delimiter, dropping its docs", where());
          binding.doc.clear();
        }
      }
      return binding;
    }
  }

  std::string EmitAngelScriptHeader(const config::Config& config) {
    const auto& as = config.angelscript;

    std::string out;
    out += "#pragma once\n";
    out += "\n";
    AppendWarningsPush(out);
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// AngelScript native registration\n";
    out += "\n";
    out += "#include <angelscript.h>\n";
    out += "\n";
    out += "namespace " + as.base_namespace + " {\n";
    out += "\n";
    out += "\t// The namespace the native bindings are registered under: one lowercased sub-namespace\n";
    out += "\t// per NativeDB group, with PascalCase native names.\n";
    out += "\tinline constexpr const char* kNamespace = \"" + as.base_namespace + "\";\n";
    out += "\n";
    out += "\t// Registers every native that is present in the crossmap. The custom types\n";
    out += "\t// (Vector2/3/4, Hash, NativeHandle) must already be registered by the caller.\n";
    out += "\tvoid RegisterAngelScript(AngelScript::asIScriptEngine* engine);\n";
    out += "\n";
    out += "} // namespace " + as.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  std::expected<std::string, std::string> EmitAngelScriptImpl(const config::Config& config,
                                                             const db::Crossmap& crossmap,
                                                             const db::NativeDb& db) {
    const auto& generation = config.generation;
    const auto& as = config.angelscript;

    std::string shims;
    std::string registrations;

    for (const auto* group : ActiveGroups(generation, db)) {
      const std::string lower = util::ToLowerAscii(group->name);

      std::string group_body;
      for (const auto& native : group->natives) {
        // A native outside the crossmap is generated as a void stub that only logs, so the real
        // signature is gone and a binding derived from the database would not match it.
        if (!crossmap.Has(native.hash)) {
          continue;
        }

        const auto binding = BuildAsBinding(generation, as, *group, native);
        if (!binding) {
          return std::unexpected(binding.error());
        }

        if (!binding->shim.empty()) {
          const auto lines = SplitLines(binding->shim);
          for (std::size_t i = 0; i < lines.size(); ++i) {
            if (i + 1 == lines.size() && lines[i].empty()) {
              break;  // SplitLines keeps the empty tail after the final newline
            }
            shims += "\t\t" + lines[i] + "\n";
          }
          shims += "\n";
        }

        if (as.docs) {
          group_body += "\t\tbase::menu::as::util::RegisterGlobalFunction(engine, \"" + binding->declaration +
                        "\", AngelScript::asFUNCTION(" + binding->symbol + "), AngelScript::asCALL_CDECL)";
          if (binding->doc.empty()) {
            group_body += ";\n";
          } else {
            group_body += "\n\t\t\t.Desc(R\"ASDOC(" + binding->doc + ")ASDOC\");\n";
          }
        } else {
          group_body += "\t\tengine->RegisterGlobalFunction(\"" + binding->declaration +
                        "\", AngelScript::asFUNCTION(" + binding->symbol + "), AngelScript::asCALL_CDECL);\n";
        }
      }

      if (group_body.empty()) {
        continue;
      }

      registrations += "\t// " + lower + "\n";
      registrations += "\tengine->SetDefaultNamespace(\"" + as.base_namespace + "::" + lower + "\");\n";
      registrations += "\n";
      registrations += group_body;
      registrations += "\n";
    }

    if (!registrations.empty()) {
      registrations += "\tengine->SetDefaultNamespace(\"\");\n";
    }

    std::string out;
    AppendWarningsPush(out);
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// AngelScript native registration\n";
    out += "\n";
    out += "#include \"" + std::string(kOutputAsHeader) + "\"\n";
    out += "#include \"" + std::string(kOutputHeader) + "\"\n";
    out += "\n";
    for (const auto& include : as.includes) {
      out += include + "\n";
    }
    if (!as.includes.empty()) {
      out += "\n";
    }
    out += "#include <string>\n";
    out += "\n";
    out += "namespace " + as.base_namespace + " {\n";
    out += "\n";
    if (!shims.empty()) {
      out += "namespace {\n";
      out += "\n";
      out += shims;
      out += "} // namespace\n";
      out += "\n";
    }
    out += "\tvoid RegisterAngelScript(AngelScript::asIScriptEngine* engine) {\n";
    out += registrations;
    out += "\t}\n";
    out += "\n";
    out += "} // namespace " + as.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  std::string EmitNativeLookup(const config::Config& config, const db::Crossmap& crossmap, const db::NativeDb& db) {
    struct Entry {
      std::string name;
      std::string return_type;
      std::string params;
    };

    // std::map gives both the numeric ordering of the case labels and last-write-wins on a
    // duplicated index, matching the dumper's dict-of-index behaviour.
    std::map<std::uint64_t, Entry> entries;
    for (const auto* group : ActiveGroups(config.generation, db)) {
      for (const auto& native : group->natives) {
        if (!crossmap.Has(native.hash)) {
          continue;
        }

        std::string params;
        for (std::size_t i = 0; i < native.params.size(); ++i) {
          if (i != 0) {
            params += ", ";
          }
          params += util::MapTypeToPrimitive(util::NormalizeType(native.params[i].type));
          params += ' ';
          params += native.params[i].name;
        }

        entries[crossmap.IndexOf(native.hash)] = Entry{
            group->name + "::" + native.name,
            util::MapTypeToPrimitive(util::NormalizeType(native.return_type)),
            params,
        };
      }
    }

    std::string out;
    out += "//\n";
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// Native index to signature lookup table\n";
    out += "//\n";
    out += "\n";
    out += "#pragma once\n";
    out += "\n";
    out += "#ifndef NDEBUG\n";
    out += "\n";
    out += "#include <string>\n";
    out += "#include <cstdint>\n";
    out += "\n";
    out += "namespace base::menu::natives {\n";
    out += "\n";
    out += "struct NativeSignature {\n";
    out += "    std::string name;\n";
    out += "    std::string return_type;\n";
    out += "    std::string params;\n";
    out += "};\n";
    out += "\n";
    out += "inline NativeSignature GetNativeSignature(std::uint64_t index) {\n";
    out += "    switch (index) {\n";

    for (const auto& [index, entry] : entries) {
      out += "        case " + std::to_string(index) + ": return {\"" + entry.name + "\", \"" + entry.return_type +
             "\", \"" + util::ReplaceAll(entry.params, "\"", "\\\"") + "\"};\n";
    }

    out += "        default:\n";
    out += "            return {\n";
    out += "                \"UNKNOWN_NATIVE_\" + std::to_string(index),\n";
    out += "                \"void\",\n";
    out += "                \"\"\n";
    out += "            };\n";
    out += "    }\n";
    out += "}\n";
    out += "\n";
    out += "inline std::string GetNativeName(std::uint64_t index) {\n";
    out += "    return GetNativeSignature(index).name;\n";
    out += "}\n";
    out += "\n";
    out += "} // namespace base::menu::natives\n";
    out += "\n";
    out += "#endif // NDEBUG\n";
    return out;
  }
}
