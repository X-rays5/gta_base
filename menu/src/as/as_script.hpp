//
// Created by X-ray on 11/09/2026.
//

#pragma once

#include <base-common/fs/vfs.hpp>
#include "as_bind.hpp"
#include "as_generate_predefined.hpp"
#include "as_log.hpp"
#include "as_native_types.hpp"
#include "as_util.hpp"
#include "../natives/natives_as.hpp"

#include <angelscript.h>

namespace base::menu::as {
  inline void MessageCallback(const AngelScript::asSMessageInfo* msg, void* param) {
    const char* type = "ERR ";
    if (msg->type == AngelScript::asMSGTYPE_WARNING) type = "WARN";
    if (msg->type == AngelScript::asMSGTYPE_INFORMATION) type = "INFO";
    LOG_INFO("[{}] {} ({}:{})", type, msg->message, msg->section, msg->row);
  }

  inline void ExecuteScriptTest() {
    AngelScript::asIScriptEngine* engine = AngelScript::asCreateScriptEngine();
    assert(engine != nullptr);

    engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, AngelScript::asCALL_CDECL);

    // Register host bindings. The docs are authored again from scratch, so drop whatever a previous
    // engine in this process left behind.
    ClearDocs();
    util::RegisterAddOns(engine);
    log::RegisterLog(engine);

    // The generated declarations are spelled in terms of these, so the types have to exist first.
    native_types::Register(engine);
    natives::RegisterAngelScript(engine);

    // Runs last so the dump reflects everything registered above.
    GenerateScriptPredefined(engine, common::fs::vfs::GetWorkingDir() / "as.predefined");

    // Test AngelScript source code calling your log namespace
    const std::string testScript = R"(
        void main() {
            log::info("Hello from AngelScript! Version: {}", 1.0f);
            log::warn("Testing player ID: {}, health: {}", 42, 98.5f);
            log::error("System error code: {}", 0x80000003);

            std::string formatted = log::format("Custom formatted string: {}", "success");
            log::debug(formatted);

            int player = natives::player::PlayerId();
            log::info("Player ped: {}", natives::player::PlayerPedId());
            log::info("Ped armour: {}", natives::ped::GetPedArmour(player));
        }
    )";

    LOG_INFO("Executing script...");
    AngelScript::asIScriptModule* mod = engine->GetModule("TestModule", AngelScript::asGM_ALWAYS_CREATE);
    mod->AddScriptSection("test.as", testScript.c_str());

    if (mod->Build() < 0) {
      LOG_ERROR("Failed to build script module.");
      engine->ShutDownAndRelease();
      return;
    }

    // 6. Execute main()
    AngelScript::asIScriptFunction* func = mod->GetFunctionByDecl("void main()");
    AngelScript::asIScriptContext* ctx = engine->CreateContext();

    ctx->Prepare(func);
    int result = ctx->Execute();

    if (result == AngelScript::asEXECUTION_EXCEPTION) {
      LOG_ERROR("Script exception: {}", ctx->GetExceptionString());
    }
    LOG_INFO("Script execution result: {}", result);

    // 7. Cleanup
    ctx->Release();
    engine->ShutDownAndRelease();
    LOG_INFO("Script execution completed.");
    AngelScript::asThreadCleanup();
  }
}