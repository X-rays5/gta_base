//
// Created by X-ray on 11/09/2026.
//

#include "as_script.hpp"
#include <angelscript.h>
#include "../../natives/natives_as.hpp"
#include "../bindings/as_coro.hpp"
#include "../bindings/as_log.hpp"
#include "../bindings/as_mutex.hpp"
#include "../bindings/as_native_types.hpp"
#include "../util/as_generate_predefined.hpp"
#include "../util/as_util.hpp"
#include <angelscript/scriptbuilder/scriptbuilder.h>

namespace base::menu::as::script {
  namespace {
    void MessageCallback(const AngelScript::asSMessageInfo* msg) {
      switch (msg->type) {
        case AngelScript::asMSGTYPE_ERROR:
          LOG_ERROR("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
        case AngelScript::asMSGTYPE_WARNING:
          LOG_WARN("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
        case AngelScript::asMSGTYPE_INFORMATION:
          LOG_INFO("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
        default:
          LOG_INFO("[AS] {} ({}:{})", msg->message, msg->section, msg->row);
          break;
      }
    }

    int IncludeCallback(const char *include, const char *from, AngelScript::CScriptBuilder *builder, void *userParam) {
      const auto script_dir = static_cast<const std::filesystem::path*>(userParam);
      if (!script_dir) {
        return -1;
      }

      std::filesystem::path include_path = include;
      if (include_path.is_relative()) {
        include_path = *script_dir / include_path;
      } else {
        const std::filesystem::path absolute_include = std::filesystem::absolute(include_path);
        if (absolute_include.string().find(script_dir->string()) != 0) {
          LOG_ERROR("[AS] Include path '{}' requested by '{}' is outside of the script directory '{}'", include, from, script_dir->string());
          return -1;
        }

        include_path = absolute_include;
      }

      return builder->AddSectionFromFile(include_path.string().c_str());
    }

    void SetLanguageModifications(AngelScript::asIScriptEngine* engine) {
      engine->SetEngineProperty(AngelScript::asEP_DISALLOW_EMPTY_LIST_ELEMENTS, true);
      engine->SetEngineProperty(AngelScript::asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE, true);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_UNSAFE_REFERENCES, false);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_MULTILINE_STRINGS, true);
      engine->SetEngineProperty(AngelScript::asEP_HEREDOC_TRIM_MODE, 0);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_IMPLICIT_HANDLE_TYPES, false);
      engine->SetEngineProperty(AngelScript::asEP_REQUIRE_ENUM_SCOPE, true);
      engine->SetEngineProperty(AngelScript::asEP_PROPERTY_ACCESSOR_MODE, 3);
      engine->SetEngineProperty(AngelScript::asEP_DISALLOW_GLOBAL_VARS, false);
      engine->SetEngineProperty(AngelScript::asEP_ALWAYS_IMPL_DEFAULT_CONSTRUCT, 0);
      engine->SetEngineProperty(AngelScript::asEP_ALWAYS_IMPL_DEFAULT_COPY, 0);
      engine->SetEngineProperty(AngelScript::asEP_ALTER_SYNTAX_NAMED_ARGS, 1);
      engine->SetEngineProperty(AngelScript::asEP_DISABLE_INTEGER_DIVISION, true);
      engine->SetEngineProperty(AngelScript::asEP_PRIVATE_PROP_AS_PROTECTED, false);
      engine->SetEngineProperty(AngelScript::asEP_ALLOW_UNICODE_IDENTIFIERS, true);
      engine->SetEngineProperty(AngelScript::asEP_IGNORE_DUPLICATE_SHARED_INTF, false);
      engine->SetEngineProperty(AngelScript::asEP_BOOL_CONVERSION_MODE, false);
      engine->SetEngineProperty(AngelScript::asEP_FOREACH_SUPPORT, true);
      engine->SetEngineProperty(AngelScript::asEP_MEMBER_INIT_MODE, 1);
    }

    void SetEngineBehaviors(AngelScript::asIScriptEngine* engine) {
      engine->SetEngineProperty(AngelScript::asEP_MAX_NESTED_CALLS, 100);
      engine->SetEngineProperty(AngelScript::asEP_OPTIMIZE_BYTECODE, true);
      engine->SetEngineProperty(AngelScript::asEP_COPY_SCRIPT_SECTIONS, true);
      engine->SetEngineProperty(AngelScript::asEP_INIT_STACK_SIZE, 1024 * 128);
      engine->SetEngineProperty(AngelScript::asEP_INIT_CALL_STACK_SIZE, 1024 * 32);
      engine->SetEngineProperty(AngelScript::asEP_BUILD_WITHOUT_LINE_CUES, false);
      engine->SetEngineProperty(AngelScript::asEP_INIT_GLOBAL_VARS_AFTER_BUILD, true);
      engine->SetEngineProperty(AngelScript::asEP_INCLUDE_JIT_INSTRUCTIONS, false);
      engine->SetEngineProperty(AngelScript::asEP_EXPAND_DEF_ARRAY_TO_TMPL, true);
      engine->SetEngineProperty(AngelScript::asEP_AUTO_GARBAGE_COLLECT, true);
      engine->SetEngineProperty(AngelScript::asEP_NO_DEBUG_OUTPUT, true);
    }

    void SetEngineProperties(AngelScript::asIScriptEngine* engine) {
      SetLanguageModifications(engine);
      SetEngineBehaviors(engine);
    }

    void RegisterBindings(AngelScript::asIScriptEngine* engine) {
      util::RegisterAddOns(engine);
      bindings::log::RegisterLog(engine);
      bindings::coro::RegisterCoro(engine);
      bindings::mutex::RegisterMutex(engine);
      bindings::native_types::Register(engine);
      natives::RegisterAngelScript(engine);
    }

    AngelScript::asIScriptEngine* InitEngine() {
      AngelScript::asIScriptEngine* engine = AngelScript::asCreateScriptEngine();

      const int r = engine->SetMessageCallback(AngelScript::asFUNCTION(MessageCallback), nullptr, AngelScript::asCALL_CDECL);
      if (r < 0) {
        LOG_ERROR("[AS] Failed to set message callback");
        throw std::runtime_error("Failed to set message callback");
      }

      SetEngineProperties(engine);
      RegisterBindings(engine);

      return engine;
    }
  }

  Script::Script(const ScriptMetaData& metadata) {
    engine_ = InitEngine();

    std::filesystem::path script_dir = metadata.GetPath();

    AngelScript::CScriptBuilder builder;
    builder.StartNewModule(engine_, metadata.GetName().c_str());
    builder.SetIncludeCallback(&IncludeCallback, &script_dir);

    builder.AddSectionFromFile(metadata.GetMainFile().string().c_str());

    builder.BuildModule();
    module_ = builder.GetModule();
  }

  Script::Script() : module_{nullptr} {
    engine_ = InitEngine();
  }

  Script::~Script() {
    if (engine_) {
      engine_->ShutDownAndRelease();
      engine_ = nullptr;
    }
  }

  ScriptContext Script::CreateContext() const {
    return ScriptContext(engine_);
  }

  void Script::DumpAsPredefined(const std::filesystem::path& path) {
    const Script script;
    util::GenerateScriptPredefined(script.engine_, path);
  }

  AngelScript::asIScriptFunction* Script::GetGameInitFunction() const {
    const auto init = module_->GetFunctionByDecl("void GameInit()");
    LOG_ERROR_CONDITIONAL(!init, "GameInit function not found in script module '{}'", module_->GetName());
    return init;
  }

  AngelScript::asIScriptFunction* Script::GetGameTickFunction() const {
    const auto tick = module_->GetFunctionByDecl("void GameTick()");
    LOG_ERROR_CONDITIONAL(!tick, "GameTick function not found in script module '{}'", module_->GetName());
    return tick;
  }
}
