//
// Created by X-ray on 10/05/22.
//

#include "script.hpp"
#include "../../natives/crossmap.hpp"
#include "../../memory/pointers.hpp"

namespace gta_base::hooking {
    inline robin_hood::unordered_map<rage::scrProgram*, ScriptHook*> ScriptHook::map_;

    static bool MapNative(rage::scrNativeHash* hash) {
      auto entry = rage::kCROSSMAP.find(*hash);
      if (entry != rage::kCROSSMAP.end()) {
        *hash = entry->second;
        return true;
      }

      return false;
    }

    ScriptHook::ScriptHook(rage::joaat_t script_hash, robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements) :
      script_hash_(script_hash),
      native_replacements_(std::move(native_replacements))
    {
      Ensure();
    }

    ScriptHook::~ScriptHook() {
      if (program_) {
        for (auto [hash, handler_ptr] : native_handlers_ptrs_) {
          auto og_handler = memory::kPOINTERS->GetNativeHandler(memory::kPOINTERS->native_registration_table_, hash);
          *handler_ptr = og_handler;
        }
      }

      if (vmt_hook_) {
        vmt_hook_->DisableAll();
        map_.erase(program_);
      }
    }

    void ScriptHook::Ensure() {
      if (vmt_hook_)
        return;

      if (auto program = memory::kPOINTERS->script_programs_table->find_script(script_hash_)) {
        if (program->is_valid()) {
          HookInst(program);
          LOG_DEBUG("Hooked {} script ({})", program->m_name, static_cast<void*>(program));
        }
      }
    }

    void ScriptHook::HookInst(rage::scrProgram* program) {
      program_ = program;
      map_.emplace(program_, this);
      vmt_hook_ = std::make_unique<VmtHook>(program_);
      vmt_hook_->Hook("ScrProgramDetour", 0, &ScrProgramDetour);

      for (auto [replacement_hash, replacement_handler] : native_replacements_) {
        auto hash = replacement_hash;
        MapNative(&hash);

        auto og_handler = memory::kPOINTERS->GetNativeHandler(memory::kPOINTERS->native_registration_table_, hash);
        if (!og_handler)
          continue;

        auto handler_ptr = program_->get_address_of_native_entrypoint(og_handler);
        if (!handler_ptr)
          continue;

        native_handlers_ptrs_.emplace(hash, reinterpret_cast<rage::scrNativeHandler*>(handler_ptr));
        *handler_ptr = replacement_handler;
      }
    }

    void ScriptHook::ScrProgramDetour(rage::scrProgram* this_, bool free_memory) {
      if (auto it = map_.find(this_); it != map_.end()) {
        auto hook = it->second;

        hook->program_ = nullptr;
        map_.erase(it);

        auto og_func = hook->vmt_hook_->GetOriginal<decltype(&ScrProgramDetour)>(0);
        hook->vmt_hook_->DisableAll();
        hook->vmt_hook_.reset();

        og_func(this_, free_memory);
      }
    }
  }