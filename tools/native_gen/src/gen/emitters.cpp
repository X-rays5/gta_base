#include "emitters.hpp"

#include <map>
#include <vector>

#include "../constants.hpp"
#include "../util/util.hpp"

// Every string literal in this file is a transcription of the Python generator's output.
// Whitespace, tabs and blank lines are significant - the port is verified byte-for-byte.
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

  std::string EmitSol2Header(const config::Config& config) {
    std::string out;
    out += "#pragma once\n";
    out += "\n";
    AppendWarningsPush(out);
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// Sol2 native registration functions\n";
    out += "\n";
    out += "#pragma warning(push)\n";
    out += "#pragma warning(disable: 5321)\n";
    out += "#include <sol/sol.hpp>\n";
    out += "#pragma warning(pop)\n";
    out += "\n";
    out += "namespace " + config.generation.base_namespace + " {\n";
    out += "\n";
    out += "\t/**\n";
    out += "\t * Register all natives to a sol2 lua state.\n";
    out += "\t * \n";
    out += "\t * @param lua The sol::state to register natives into\n";
    out += "\t * @return A sol::table containing all registered native groups\n";
    out += "\t */\n";
    out += "\tsol::table register_natives(sol::state& lua);\n";
    out += "\n";
    out += "} // namespace " + config.generation.base_namespace + "\n";
    out += "\n#pragma warning(pop)\n";
    return out;
  }

  std::string EmitSol2Impl(const config::Config& config, const db::NativeDb& db) {
    const auto& generation = config.generation;

    std::string out;
    AppendWarningsPush(out);
    out += "// Auto-generated file - DO NOT EDIT\n";
    out += "// Sol2 native registration functions\n";
    out += "\n";
    out += "#include \"" + std::string(kOutputSol2Header) + "\"\n";
    out += "#include \"" + std::string(kOutputHeader) + "\"\n";
    out += "\n";
    out += "namespace " + generation.base_namespace + " {\n";
    out += "\n";

    std::vector<std::string> active_groups;
    for (const auto* group : ActiveGroups(generation, db)) {
      active_groups.push_back(group->name);
      const std::string lower = util::ToLowerAscii(group->name);

      out += "\tnamespace " + lower + " {\n";
      out += "\t\tsol::table register_" + lower + "(sol::state& lua) {\n";
      out += "\t\t\tauto " + lower + "_table = lua.create_table();\n";
      out += "\n";

      for (const auto& native : group->natives) {
        const std::string full_name = generation.base_namespace + "::" + group->name + "::" + native.name;
        out += "\t\t\t" + lower + "_table[\"" + util::PascalCase(native.name) + "\"] = sol::c_call<decltype(&" + full_name +
               "), &" + full_name + ">;\n";
      }

      out += "\n";
      out += "\t\t\treturn " + lower + "_table;\n";
      out += "\t\t}\n";
      out += "\t} // namespace " + lower + "\n\n";
    }

    out += "\tsol::table register_natives(sol::state& lua) {\n";
    out += "\t\tauto natives = lua.create_table();\n";
    out += "\n";
    for (const auto& group : active_groups) {
      const std::string lower = util::ToLowerAscii(group);
      out += "\t\tnatives[\"" + lower + "\"] = " + lower + "::register_" + lower + "(lua);\n";
    }
    out += "\n";
    out += "\t\treturn natives;\n";
    out += "\t}\n";
    out += "\n";
    out += "} // namespace " + generation.base_namespace + "\n";
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
