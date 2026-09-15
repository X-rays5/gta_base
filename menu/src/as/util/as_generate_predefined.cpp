//
// Created by X-ray on 11/09/2026.
//

#include "as_generate_predefined.hpp"
#include "as_bind.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <sstream>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace base::menu::as::util {
  namespace {
    /// Keys matched against registered docs while emitting, so typos can be reported afterwards.
    using ConsumedDocs = std::unordered_set<std::string>;

    /// Reflected names may or may not carry their namespace, so normalise both spellings against
    /// the namespace the engine reports alongside them.
    std::string_view bareName(const std::string_view name) {
      const auto pos = name.rfind("::");
      return pos == std::string_view::npos ? name : name.substr(pos + 2);
    }

    /// The engine hands back a null pointer rather than an empty string for anything registered
    /// outside a namespace.
    std::string_view namespaceOf(const char* ns) {
      return ns == nullptr ? std::string_view{} : std::string_view{ns};
    }

    std::string reflectedKey(const std::string_view name, const std::string_view ns) {
      return QualifyNamespace(bareName(name), ns);
    }

    /**
     * Emit the javadoc-style comment for a binding, if one was registered. The comment has to end up
     * on the line directly above the declaration, which is where tooling looks for hover docs.
     */
    template <class Stream>
    void printDoc(Stream& stream, const std::string_view key, ConsumedDocs& consumed, const std::string_view indent = "") {
      const auto* const doc = FindDoc(key);
      if (not doc) return;

      consumed.emplace(key);
      stream << FormatDocComment(*doc, indent);
    }

    template <class Stream>
    void printEnumList(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      for (std::uint32_t i = 0; i < engine->GetEnumCount(); i++) {
        const auto e = engine->GetEnumByIndex(i);
        if (not e) continue;
        const std::string_view ns = namespaceOf(e->GetNamespace());
        const std::string enumKey = reflectedKey(e->GetName(), ns);
        if (not ns.empty()) stream << std::format("namespace {} {{\n", ns);
        printDoc(stream, enumKey, consumed);
        stream << std::format("enum {} {{\n", e->GetName());
        for (std::uint32_t j = 0; j < e->GetEnumValueCount(); ++j) {
          const char* valueName = e->GetEnumValueByIndex(j, nullptr);
          if (not valueName) continue;
          printDoc(stream, std::format("{}::{}", enumKey, valueName), consumed, "\t");
          stream << std::format("\t{}", valueName);
          if (j < e->GetEnumValueCount() - 1) stream << ",";
          stream << "\n";
        }
        stream << "}\n";
        if (not ns.empty()) stream << "}\n";
      }
    }

    template <class Stream>
    void printClassTypeList(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      for (std::uint32_t i = 0; i < engine->GetObjectTypeCount(); i++) {
        const auto t = engine->GetObjectTypeByIndex(i);
        if (not t) continue;

        const std::string_view ns = namespaceOf(t->GetNamespace());
        const std::string typeKey = reflectedKey(t->GetName(), ns);
        if (not ns.empty()) stream << std::format("namespace {} {{\n", ns);

        printDoc(stream, typeKey, consumed, ns.empty() ? "" : "\t");

        stream << std::format("class {}", t->GetName());
        if (t->GetSubTypeCount() > 0) {
          stream << "<";
          for (std::uint32_t sub = 0; sub < t->GetSubTypeCount(); ++sub) {
            if (sub < t->GetSubTypeCount() - 1) stream << ", ";
            const auto st = t->GetSubType(sub);
            stream << st->GetName();
          }

          stream << ">";
        }

        stream << "{\n";
        // A constructor, a destructor and a factory all reflect as "f" - the engine names every one of
        // them after the type rather than after a name of their own - and docs are keyed by name rather
        // than by signature, so the first of them carries the block for the whole set.
        bool madeDocumented = false;

        // The factories come first, and they are what a reference type is made by: the engine keeps them
        // out of the behaviour list ("Count the number of behaviours (except factory functions)"), so a
        // type that is only ever made through one - gui::Toggle, gui::Submenu, Option, every type this
        // API is written in - has no other declaration of how it is made in this file, and the
        // documentation written on its factory matched nothing here and was dropped with it.
        for (std::uint32_t j = 0; j < t->GetFactoryCount(); ++j) {
          const auto f = t->GetFactoryByIndex(j);
          if (not f) continue;
          if (not madeDocumented) {
            madeDocumented = true;
            printDoc(stream, std::format("{}::f", typeKey), consumed, "\t");
          }
          stream << std::format("\t{};\n", f->GetDeclaration(false, true, true));
        }

        for (std::uint32_t j = 0; j < t->GetBehaviourCount(); ++j) {
          AngelScript::asEBehaviours behaviours;
          const auto f = t->GetBehaviourByIndex(j, &behaviours);
          if (behaviours == AngelScript::asBEHAVE_CONSTRUCT
            || behaviours == AngelScript::asBEHAVE_DESTRUCT) {
            if (not madeDocumented) {
              madeDocumented = true;
              printDoc(stream, std::format("{}::f", typeKey), consumed, "\t");
            }
            stream << std::format("\t{};\n", f->GetDeclaration(false, true, true));
          }
        }
        for (std::uint32_t j = 0; j < t->GetMethodCount(); ++j) {
          const auto m = t->GetMethodByIndex(j);
          printDoc(stream, std::format("{}::{}", typeKey, bareName(m->GetName())), consumed, "\t");
          stream << std::format("\t{};\n", m->GetDeclaration(false, true, true));
        }
        for (std::uint32_t j = 0; j < t->GetPropertyCount(); ++j) {
          const char* name = nullptr;
          t->GetProperty(j, &name);
          if (name) printDoc(stream, std::format("{}::{}", typeKey, bareName(name)), consumed, "\t");
          stream << std::format("\t{};\n", t->GetPropertyDeclaration(j, true));
        }
        for (std::uint32_t j = 0; j < t->GetChildFuncdefCount(); ++j) {
          const auto funcdef = t->GetChildFuncdef(j);
          printDoc(stream, std::format("{}::{}", typeKey, bareName(funcdef->GetName())), consumed, "\t");
          stream << std::format("\tfuncdef {};\n", funcdef->GetFuncdefSignature()->GetDeclaration(false));
        }
        stream << "}\n";
        if (not ns.empty()) stream << "}\n";
      }
    }

    /**
     * Global funcdefs, which are the shapes a script hands to a native: thread::TaskFunc is what
     * thread::queue_game_task() takes. Nothing else in this file declares one, and a signature that
     * names one - queue_game_task(TaskFunc@+ fn) - would otherwise name a type the reader never sees.
     */
    template <class Stream>
    void printGlobalFuncdefList(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      for (std::uint32_t i = 0; i < engine->GetFuncdefCount(); ++i) {
        const auto t = engine->GetFuncdefByIndex(i);
        if (not t) continue;
        const auto signature = t->GetFuncdefSignature();
        if (not signature) continue;

        const std::string_view ns = namespaceOf(t->GetNamespace());
        if (not ns.empty()) stream << std::format("namespace {} {{\n", ns);
        printDoc(stream, reflectedKey(t->GetName(), ns), consumed, ns.empty() ? "" : "\t");
        stream << std::format("{}funcdef {};\n", ns.empty() ? "" : "\t", signature->GetDeclaration(false));
        if (not ns.empty()) stream << "}\n";
      }
    }

    template <class Stream>
    void printGlobalFunctionList(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      for (std::uint32_t i = 0; i < engine->GetGlobalFunctionCount(); i++) {
        const auto f = engine->GetGlobalFunctionByIndex(i);
        if (not f) continue;
        const std::string_view ns = namespaceOf(f->GetNamespace());

        // Emitted as a namespace block rather than a single line, so that a doc comment can sit
        // directly above the declaration instead of above the namespace.
        if (not ns.empty()) stream << std::format("namespace {} {{\n", ns);
        printDoc(stream, reflectedKey(f->GetName(), ns), consumed, ns.empty() ? "" : "\t");
        stream << std::format("{}{};\n", ns.empty() ? "" : "\t", f->GetDeclaration(false, false, true));
        if (not ns.empty()) stream << "}\n";
      }
    }

    template <class Stream>
    void printGlobalPropertyList(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      for (std::uint32_t i = 0; i < engine->GetGlobalPropertyCount(); i++) {
        const char* name;
        const char* ns0;
        int type;
        engine->GetGlobalPropertyByIndex(i, &name, &ns0, &type, nullptr, nullptr, nullptr, nullptr);

        const std::string t = engine->GetTypeDeclaration(type, true);
        if (t.empty()) continue;

        const std::string_view ns = namespaceOf(ns0);
        if (not ns.empty()) stream << std::format("namespace {} {{\n", ns);
        printDoc(stream, reflectedKey(name, ns), consumed, ns.empty() ? "" : "\t");
        stream << std::format("{}{} {};\n", ns.empty() ? "" : "\t", t, name);
        if (not ns.empty()) stream << "}\n";
      }
    }

    template <class Stream>
    void printGlobalTypedef(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      for (std::uint32_t i = 0; i < engine->GetTypedefCount(); ++i) {
        const auto type = engine->GetTypedefByIndex(i);
        if (not type) continue;
        const std::string_view ns = namespaceOf(type->GetNamespace());
        if (not ns.empty()) stream << std::format("namespace {} {{\n", ns);
        printDoc(stream, reflectedKey(type->GetName(), ns), consumed, ns.empty() ? "" : "\t");
        stream << std::format("{}typedef {} {};\n", ns.empty() ? "" : "\t",
                              engine->GetTypeDeclaration(type->GetTypedefTypeId()), type->GetName());
        if (not ns.empty()) stream << "}\n";
      }
    }
    /// Write out everything the engine has, in the order a reader wants it: the types first, since
    /// the declarations after them are written in terms of those types.
    template <class Stream>
    void emitPredefined(const AngelScript::asIScriptEngine* engine, Stream& stream, ConsumedDocs& consumed) {
      printEnumList(engine, stream, consumed);

      printClassTypeList(engine, stream, consumed);

      printGlobalFuncdefList(engine, stream, consumed);

      printGlobalFunctionList(engine, stream, consumed);

      printGlobalPropertyList(engine, stream, consumed);

      printGlobalTypedef(engine, stream, consumed);
    }

    /// The docs no declaration was written for, sorted so that the report reads the same every run.
    std::vector<std::string> unmatchedDocKeys(const ConsumedDocs& consumed) {
      std::vector<std::string> unmatched;
      for (const auto& [key, doc] : GetAllDocs()) {
        if (IsDocumented(doc) && not consumed.contains(key)) {
          unmatched.push_back(key);
        }
      }

      std::ranges::sort(unmatched);
      return unmatched;
    }
  }

  std::vector<std::string> UnmatchedDocKeys(const AngelScript::asIScriptEngine* engine) {
    // The declarations are emitted to find out which docs they carry; the text itself is not wanted.
    std::ostringstream discarded;

    ConsumedDocs consumed;
    emitPredefined(engine, discarded, consumed);

    return unmatchedDocKeys(consumed);
  }

  void GenerateScriptPredefined(const AngelScript::asIScriptEngine* engine, const std::filesystem::path& path) {
    GTA_BASE_ASSERT(path.string().ends_with("as.predefined"), "Output path must end with 'as.predefined'");

    std::ofstream stream{path};

    ConsumedDocs consumed;
    emitPredefined(engine, stream, consumed);

    for (const auto& key : unmatchedDocKeys(consumed)) {
      LOG_WARN("Script documentation for '{}' matched no binding in the engine", key);
    }
  }
}
