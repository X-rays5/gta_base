//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <angelscript.h>

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace base::menu::as {
  struct ParamDoc {
    std::string name;
    std::string description;
  };

  /**
   * Documentation for a single script binding, authored at the registration site.
   */
  struct Doc {
    std::string description;
    std::vector<ParamDoc> params;
    std::string returns;
  };

  /**
   * Fluent view over the Doc stored for a binding. The underlying reference stays valid for the
   * lifetime of the registry entry, so it may be held past the registration call that produced it.
   */
  class DocBuilder {
  public:
    explicit DocBuilder(Doc& doc) : doc_(&doc) {}

    DocBuilder& Desc(const std::string_view text) {
      doc_->description.assign(text);
      return *this;
    }

    DocBuilder& Param(const std::string_view name, const std::string_view description) {
      doc_->params.push_back(ParamDoc{std::string{name}, std::string{description}});
      return *this;
    }

    DocBuilder& Returns(const std::string_view text) {
      doc_->returns.assign(text);
      return *this;
    }

  private:
    Doc* doc_;
  };

  /**
   * Docs are keyed by symbol name rather than by signature, so a single doc covers every overload of
   * a name (log::info has both a fixed and a variadic overload that read the same).
   *   global function/property  ns::name
   *   object type               ns::Type
   *   method/object property    ns::Type::member
   *   enum/enum value           ns::Enum, ns::Enum::value
   *   typedef/funcdef           ns::Name
   */
  std::unordered_map<std::string, Doc>& GetDocRegistry();

  /**
   * Look up the docs registered for a key, or nullptr when nothing was registered.
   */
  const Doc* FindDoc(std::string_view key);

  /**
   * Every registered doc, used to report entries that never matched anything in the engine.
   */
  const std::unordered_map<std::string, Doc>& GetAllDocs();

  /**
   * Drop every registered doc. The registry outlives a single engine, so a second registration pass
   * in the same process would otherwise append a duplicate of every parameter to the doc it wrote
   * the first time. Call this before re-registering, while no DocBuilder is still alive.
   */
  void ClearDocs();

  /**
   * Render a javadoc-style comment block, every line prefixed with `indent`. Returns an empty string
   * when there is nothing to document.
   */
  std::string FormatDocComment(const Doc& doc, std::string_view indent);

  /**
   * The trailing symbol name of a declaration: "void log::info(const string &in)" -> "log::info",
   * "int m_value" -> "m_value".
   */
  std::string SymbolPathFromDecl(std::string_view decl);

  /**
   * Prefix `name` with `defaultNamespace` unless it already carries a namespace of its own.
   */
  std::string QualifyNamespace(std::string_view name, std::string_view defaultNamespace);

  // Registration helpers mirroring asIScriptEngine, which additionally capture docs for the binding.
  // The generator keys docs off symbol names, so only one overload of a name needs documenting.

  DocBuilder RegisterGlobalFunction(AngelScript::asIScriptEngine* engine, const char* declaration,
                                    const AngelScript::asSFuncPtr& funcPointer, AngelScript::asDWORD callConv,
                                    void* auxiliary = nullptr);

  DocBuilder RegisterGlobalProperty(AngelScript::asIScriptEngine* engine, const char* declaration, void* pointer);

  DocBuilder RegisterObjectType(AngelScript::asIScriptEngine* engine, const char* obj, int byteSize,
                               AngelScript::asQWORD flags);

  DocBuilder RegisterObjectMethod(AngelScript::asIScriptEngine* engine, const char* obj, const char* declaration,
                                 const AngelScript::asSFuncPtr& funcPointer, AngelScript::asDWORD callConv,
                                 void* auxiliary = nullptr, int compositeOffset = 0, bool isCompositeIndirect = false);

  DocBuilder RegisterObjectProperty(AngelScript::asIScriptEngine* engine, const char* obj, const char* declaration,
                                   int byteOffset, int compositeOffset = 0, bool isCompositeIndirect = false);

  DocBuilder RegisterEnum(AngelScript::asIScriptEngine* engine, const char* type);

  DocBuilder RegisterEnumValue(AngelScript::asIScriptEngine* engine, const char* type, const char* name, int value);

  DocBuilder RegisterFuncdef(AngelScript::asIScriptEngine* engine, const char* declaration);

  DocBuilder RegisterTypedef(AngelScript::asIScriptEngine* engine, const char* type, const char* declaration);
}
