//
// Created by X-ray on 11/09/2026.
//

#include "as_bind.hpp"

#include <format>

namespace base::menu::as::util {
  namespace {
    std::vector<std::string_view> SplitLines(std::string_view text) {
      std::vector<std::string_view> lines;
      while (not text.empty()) {
        const auto pos = text.find('\n');
        if (pos == std::string_view::npos) {
          lines.push_back(text);
          break;
        }
        lines.push_back(text.substr(0, pos));
        text.remove_prefix(pos + 1);
      }
      return lines;
    }

    std::string KeyForDecl(const AngelScript::asIScriptEngine* engine, const std::string_view decl) {
      return QualifyNamespace(SymbolPathFromDecl(decl), engine->GetDefaultNamespace());
    }

    std::string KeyForObjectMember(const AngelScript::asIScriptEngine* engine, const std::string_view obj,
                                   const std::string_view decl) {
      std::string member = SymbolPathFromDecl(decl);
      // Object member declarations are normally unqualified ("void SetValue(int)"), but a
      // declaration that repeats the object name is accepted too, so drop any qualifier.
      if (const auto pos = member.rfind("::"); pos != std::string::npos) {
        member.erase(0, pos + 2);
      }
      return std::format("{}::{}", QualifyNamespace(obj, engine->GetDefaultNamespace()), member);
    }

    Doc& Emplace(const std::string& key) {
      return GetDocRegistry()[key];
    }
  }

  std::unordered_map<std::string, Doc>& GetDocRegistry() {
    static std::unordered_map<std::string, Doc> registry;
    return registry;
  }

  const Doc* FindDoc(const std::string_view key) {
    const auto& registry = GetDocRegistry();
    const auto it = registry.find(std::string{key});
    return it == registry.end() ? nullptr : &it->second;
  }

  const std::unordered_map<std::string, Doc>& GetAllDocs() {
    return GetDocRegistry();
  }

  void ClearDocs() {
    GetDocRegistry().clear();
  }

  bool IsDocumented(const Doc& doc) {
    return !doc.description.empty() || !doc.params.empty() || !doc.returns.empty();
  }

  std::string FormatDocComment(const Doc& doc, const std::string_view indent) {
    if (!IsDocumented(doc)) return {};

    std::string out = std::format("{}/**\n", indent);
    const auto line = [&](const std::string_view text) {
      if (text.empty()) {
        out += std::format("{} *\n", indent);
      } else {
        out += std::format("{} * {}\n", indent, text);
      }
    };

    for (const auto text : SplitLines(doc.description)) {
      line(text);
    }
    if (not doc.description.empty() && (not doc.params.empty() || not doc.returns.empty())) {
      line("");
    }
    for (const auto& [name, description] : doc.params) {
      line(std::format("@param {} {}", name, description));
    }
    if (not doc.returns.empty()) {
      line(std::format("@return {}", doc.returns));
    }
    out += std::format("{} */\n", indent);
    return out;
  }

  std::string SymbolPathFromDecl(const std::string_view decl) {
    std::string_view head = decl.substr(0, decl.find('('));
    while (not head.empty() && std::isspace(static_cast<unsigned char>(head.back())) != 0) {
      head.remove_suffix(1);
    }

    const auto is_symbol_char = [](const char c) {
      return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' || c == ':' || c == '~';
    };

    std::size_t start = head.size();
    while (start > 0 && is_symbol_char(head[start - 1])) {
      --start;
    }
    return std::string{head.substr(start)};
  }

  std::string QualifyNamespace(const std::string_view name, const std::string_view defaultNamespace) {
    if (defaultNamespace.empty() || name.find("::") != std::string_view::npos) return std::string{name};
    return std::format("{}::{}", defaultNamespace, name);
  }

  DocBuilder RegisterGlobalFunction(AngelScript::asIScriptEngine* engine, const char* declaration,
                                    const AngelScript::asSFuncPtr& funcPointer, const AngelScript::asDWORD callConv,
                                    void* auxiliary) {
    engine->RegisterGlobalFunction(declaration, funcPointer, callConv, auxiliary);
    return DocBuilder{Emplace(KeyForDecl(engine, declaration))};
  }

  DocBuilder RegisterGlobalProperty(AngelScript::asIScriptEngine* engine, const char* declaration, void* pointer) {
    engine->RegisterGlobalProperty(declaration, pointer);
    return DocBuilder{Emplace(KeyForDecl(engine, declaration))};
  }

  DocBuilder RegisterObjectType(AngelScript::asIScriptEngine* engine, const char* obj, const int byteSize,
                               const AngelScript::asQWORD flags) {
    engine->RegisterObjectType(obj, byteSize, flags);
    return DocBuilder{Emplace(KeyForDecl(engine, obj))};
  }

  DocBuilder RegisterObjectMethod(AngelScript::asIScriptEngine* engine, const char* obj, const char* declaration,
                                 const AngelScript::asSFuncPtr& funcPointer, const AngelScript::asDWORD callConv,
                                 void* auxiliary, const int compositeOffset, const bool isCompositeIndirect) {
    engine->RegisterObjectMethod(obj, declaration, funcPointer, callConv, auxiliary, compositeOffset, isCompositeIndirect);
    return DocBuilder{Emplace(KeyForObjectMember(engine, obj, declaration))};
  }

  DocBuilder RegisterObjectProperty(AngelScript::asIScriptEngine* engine, const char* obj, const char* declaration,
                                   const int byteOffset, const int compositeOffset, const bool isCompositeIndirect) {
    engine->RegisterObjectProperty(obj, declaration, byteOffset, compositeOffset, isCompositeIndirect);
    return DocBuilder{Emplace(KeyForObjectMember(engine, obj, declaration))};
  }

  DocBuilder RegisterObjectBehaviour(AngelScript::asIScriptEngine* engine, const char* obj,
                                     const AngelScript::asEBehaviours behaviour, const char* declaration,
                                     const AngelScript::asSFuncPtr& funcPointer, const AngelScript::asDWORD callConv,
                                     void* auxiliary, const int compositeOffset, const bool isCompositeIndirect) {
    engine->RegisterObjectBehaviour(obj, behaviour, declaration, funcPointer, callConv, auxiliary, compositeOffset,
                                    isCompositeIndirect);
    return DocBuilder{Emplace(KeyForObjectMember(engine, obj, declaration))};
  }

  DocBuilder RegisterEnum(AngelScript::asIScriptEngine* engine, const char* type) {
    engine->RegisterEnum(type);
    return DocBuilder{Emplace(KeyForDecl(engine, type))};
  }

  DocBuilder RegisterEnumValue(AngelScript::asIScriptEngine* engine, const char* type, const char* name, const int value) {
    engine->RegisterEnumValue(type, name, value);
    return DocBuilder{Emplace(std::format("{}::{}", KeyForDecl(engine, type), name))};
  }

  DocBuilder RegisterFuncdef(AngelScript::asIScriptEngine* engine, const char* declaration) {
    engine->RegisterFuncdef(declaration);
    return DocBuilder{Emplace(KeyForDecl(engine, declaration))};
  }

  DocBuilder RegisterTypedef(AngelScript::asIScriptEngine* engine, const char* type, const char* declaration) {
    engine->RegisterTypedef(type, declaration);
    return DocBuilder{Emplace(KeyForDecl(engine, declaration))};
  }
}
