//
// Created by X-ray on 10/05/22.
//

#include "native_hooking.hpp"
#include "../memory/pointers.hpp"
#include "../misc/globals.hpp"
#include "../natives/natives.hpp"
#include "../rage/util/find_script_thread.hpp"

namespace gta_base {
  namespace hooking {

    NativeHooking::NativeHooking() {

      for (auto&& detours_for_script : detour_hooks_) {
        const auto thread = rage::FindScriptThread(detours_for_script.first);
        if (thread && thread->m_context.m_state == rage::eThreadState::running) {
          ThreadStart(thread);
        }
      }

      kNATIVE_HOOKING = this;
    }

    NativeHooking::~NativeHooking() {
      kNATIVE_HOOKING = nullptr;
    }

    void NativeHooking::AddDetour(rage::scrNativeHash native_hash, rage::scrNativeHandler handler) {
      AddDetour(GTA_BASE_NATIVE_DETOUR_ALL_SCRIPTS, native_hash, handler);
    }

    void NativeHooking::AddDetour(rage::joaat_t script_hash, rage::scrNativeHash native_hash, rage::scrNativeHandler handler) {
      if (detour_hooks_.find(script_hash) == detour_hooks_.end())
        detour_hooks_[script_hash] = {};

      detour_hooks_[script_hash][native_hash] = handler;
    }

    void NativeHooking::ThreadStart(const GtaThread* thread) {
      if (!thread)
        return;

      robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements;
      const auto script_hash = thread->m_script_hash;

      // Functions that need to be detoured for all scripts
      if (const auto& pair = detour_hooks_.find(GTA_BASE_NATIVE_DETOUR_ALL_SCRIPTS); pair != detour_hooks_.end())
        for (const auto& native_hook_reg : pair->second)
          native_replacements.insert(native_hook_reg);

      // Functions that only need to be detoured for a specific script
      if (const auto& pair = detour_hooks_.find(script_hash); pair != detour_hooks_.end())
        for (const auto& native_hook_reg : pair->second)
          native_replacements.insert(native_hook_reg);

      if (!native_replacements.empty()) {
        if (script_hooks_.find(thread->m_script_hash) != script_hooks_.end()) {
          // this should never happen but if it does we catch it
          LOG_WARN("Dynamic native script hook still active for script, cleaning up...");

          script_hooks_.erase(thread->m_script_hash);
        }

        script_hooks_.emplace(thread->m_script_hash, std::make_unique<ScriptHook>(thread->m_script_hash, native_replacements));
      }
    }

    void NativeHooking::ThreadKill(const GtaThread* thread) {
      if (!thread)
        return;

      if (script_hooks_.erase(thread->m_script_hash)) {
        LOG_INFO("{} script terminated, cleaning up hooks", thread->m_name);
      }
    }
  }
}