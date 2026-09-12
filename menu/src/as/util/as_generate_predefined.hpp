//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <angelscript.h>

#include <filesystem>
#include <string>
#include <vector>

namespace base::menu::as::util {
  /**
   * Generate stubs for angel-lsp
   * @param engine The AngelScript engine
   * @param path The path to the output file
   */
  void GenerateScriptPredefined(const AngelScript::asIScriptEngine* engine, const std::filesystem::path& path);

  /**
   * Every registered doc the engine has no binding to carry, sorted - what GenerateScriptPredefined
   * reports as warnings, for a caller that wants the list itself rather than the log.
   *
   * A binding that documented nothing is not one of these. Every registration creates an entry for the
   * fluent calls after it to write into, and the entries nobody writes to - the behaviours of a
   * hand-registered type, for one - are a binding with no docs rather than a doc with no binding. A key
   * that does come back is a name something was written for and the engine emitted nothing for: a typo
   * at a registration site, or a binding this generator does not know how to write out.
   */
  [[nodiscard]] std::vector<std::string> UnmatchedDocKeys(const AngelScript::asIScriptEngine* engine);
}
