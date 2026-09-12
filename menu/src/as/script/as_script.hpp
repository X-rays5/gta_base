//
// Created by X-ray on 11/09/2026.
//

#pragma once
#include <angelscript.h>
#include "as_script_metadata.hpp"
#include "as_script_context.hpp"

namespace base::menu::as::script {
  class Script {
    friend class ScriptManager;

  public:
    explicit Script(const ScriptMetaData& metadata);
    ~Script();

    Script(const Script&) = delete;
    Script(Script&&) = delete;
    Script& operator=(const Script&) = delete;
    Script& operator=(Script&&) = delete;

    ScriptContext CreateContext() const;

  protected:
    static void DumpAsPredefined(const std::filesystem::path& path);

  private:
    Script();

    AngelScript::asIScriptFunction* GetGameInitFunction() const;
    AngelScript::asIScriptFunction* GetGameTickFunction() const;

  private:
    AngelScript::asIScriptEngine* engine_;
    AngelScript::asIScriptModule* module_;
  };
}