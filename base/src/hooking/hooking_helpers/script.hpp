//
// Created by X-ray on 10/05/22.
//

#pragma once
#ifndef GTA_BASE_SCRIPT_HOOK_HPP
#define GTA_BASE_SCRIPT_HOOK_HPP
#include <robin_hood.h>
#include <script/scrProgram.hpp>
#include <script/scrNativeHandler.hpp>
#include "vmt.hpp"
#include "../../rage/joaat.hpp"

namespace gta_base::hooking {
    class ScriptHook {
    public:
      explicit ScriptHook(rage::joaat_t script_hash, robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements);
      ~ScriptHook();

      void Ensure();
    private:
      void HookInst(rage::scrProgram *program);

      static void ScrProgramDetour(rage::scrProgram *this_, bool free_memory);
      static robin_hood::unordered_map<rage::scrProgram*, ScriptHook*> map_;

      rage::joaat_t script_hash_;
      rage::scrProgram* program_;
      std::unique_ptr<VmtHook> vmt_hook_;
      robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements_;
      robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler*> native_handlers_ptrs_;
    };
  }
#endif //GTA_BASE_SCRIPT_HOOK_HPP
