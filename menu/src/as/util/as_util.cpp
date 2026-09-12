//
// Created by X-ray on 11/09/2026.
//

#include "as_util.hpp"
#include <angelscript/datetime/datetime.h>
#include <angelscript/scriptany/scriptany.h>
#include <angelscript/scriptarray/scriptarray.h>
#include <angelscript/scriptdictionary/scriptdictionary.h>
#include <angelscript/scriptmath/scriptmath.h>
#include <angelscript/scriptmath/scriptmathcomplex.h>
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <enchantum/enchantum.hpp>

namespace base::menu::as::util {
  std::string ArgToString(AngelScript::asIScriptGeneric* gen, const int argIdx, const int typeId) {
  void* ptr = gen->GetArgAddress(argIdx);
  if (!ptr) return "null";

  const AngelScript::asIScriptEngine* engine = gen->GetEngine();

  // Mask out all high flag bits (asTYPEID_HANDLETOCONST, asTYPEID_OBJHANDLE, etc.)
  // Primitive IDs range from 0 (void) to 11 (double), occupying only the lowest bits.
  const int baseTypeId = typeId & 0x00FFFFFF;

  switch (baseTypeId) {
    case AngelScript::asTYPEID_VOID:    return "void";
    case AngelScript::asTYPEID_BOOL:    return *static_cast<bool*>(ptr) ? "true" : "false";
    case AngelScript::asTYPEID_INT8:    return std::to_string(*static_cast<int8_t*>(ptr));
    case AngelScript::asTYPEID_INT16:   return std::to_string(*static_cast<int16_t*>(ptr));
    case AngelScript::asTYPEID_INT32:   return std::to_string(*static_cast<int32_t*>(ptr));
    case AngelScript::asTYPEID_INT64:   return std::to_string(*static_cast<int64_t*>(ptr));
    case AngelScript::asTYPEID_UINT8:   return std::to_string(*static_cast<uint8_t*>(ptr));
    case AngelScript::asTYPEID_UINT16:  return std::to_string(*static_cast<uint16_t*>(ptr));
    case AngelScript::asTYPEID_UINT32:  return std::to_string(*static_cast<uint32_t*>(ptr));
    case AngelScript::asTYPEID_UINT64:  return std::to_string(*static_cast<uint64_t*>(ptr));
    case AngelScript::asTYPEID_FLOAT:   return std::to_string(*static_cast<float*>(ptr));
    case AngelScript::asTYPEID_DOUBLE:  return std::to_string(*static_cast<double*>(ptr));
    default: break;
  }

  // Check std::string if registered
  const AngelScript::asITypeInfo* type = engine->GetTypeInfoById(typeId);
  if (type && std::string_view(type->GetName()) == "string") {
    return *static_cast<std::string*>(ptr);
  }

  // Check Object Handles
  if (typeId & AngelScript::asTYPEID_OBJHANDLE) {
    const void* obj = *static_cast<void**>(ptr);
    if (!obj) return "null";
  }

  std::string_view fallback = type ? type->GetName() : enchantum::to_string(static_cast<AngelScript::asETypeIdFlags>(typeId));
  return fmt::format("<{}>", fallback.empty() ? "unknown" : fallback);
}

  void RegisterAddOns(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("std");
    AngelScript::RegisterScriptDateTime(engine);
    AngelScript::RegisterScriptArray(engine, true);
    AngelScript::RegisterStdString(engine);
    AngelScript::RegisterStdStringUtils(engine);
    AngelScript::RegisterScriptAny(engine);
    AngelScript::RegisterScriptDictionary(engine);
    engine->SetDefaultNamespace("std::math");
    AngelScript::RegisterScriptMath(engine);
    AngelScript::RegisterScriptMathComplex(engine);
    engine->SetDefaultNamespace("");
  }
}
