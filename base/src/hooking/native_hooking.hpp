//
// Created by X-ray on 10/05/22.
//

#pragma once
#ifndef GTA_BASE_NATIVE_HOOKING_HPP
#define GTA_BASE_NATIVE_HOOKING_HPP
#include <robin_hood.h>
#include <script/GtaThread.hpp>
#include <script/scrNativeHandler.hpp>
#include "hooking_helpers/script.hpp"
#include "../rage/joaat.hpp"

#define GTA_BASE_NATIVE_DETOUR_ALL_SCRIPTS RAGE_JOAAT("ALL_RUNNING_SCRIPTS")

namespace gta_base::hooking {
  struct NativeHooks {
    static void NETWORK_BAIL(rage::scrNativeCallContext* ctx);
  };

  class NativeHooking {
  public:
    NativeHooking();
    ~NativeHooking();

    NativeHooking(const NativeHooking&) = delete;
    NativeHooking(NativeHooking&&) noexcept = delete;
    NativeHooking& operator=(const NativeHooking&) = delete;
    NativeHooking& operator=(NativeHooking&&) noexcept = delete;

    void AddDetour(rage::scrNativeHash native_hash, rage::scrNativeHandler handler);
    void AddDetour(rage::joaat_t script_hash, rage::scrNativeHash native_hash, rage::scrNativeHandler handler);

    void ThreadStart(const GtaThread* thread);
    void ThreadKill(const GtaThread* thread);

  private:
    robin_hood::unordered_map<rage::joaat_t, robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler>> detour_hooks_;
    robin_hood::unordered_map<rage::joaat_t, std::unique_ptr<ScriptHook>> script_hooks_;

  private:
    robin_hood::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> GetNativeReplacements(rage::joaat_t script_hash);
  };

  inline NativeHooking* kNATIVE_HOOKING{};
}
#endif //GTA_BASE_NATIVE_HOOKING_HPP
