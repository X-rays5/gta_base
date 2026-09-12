//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <angelscript.h>

namespace base::menu::as::util {
  /**
   * Generate stubs for angel-lsp
   * @param engine The AngelScript engine
   * @param path The path to the output file
   */
  void GenerateScriptPredefined(const AngelScript::asIScriptEngine* engine, const std::filesystem::path& path);
}
