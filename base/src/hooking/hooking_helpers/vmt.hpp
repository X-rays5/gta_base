//
// Created by X-ray on 07/31/22.
//

#pragma once
#ifndef GTA_BASE_VMT_HPP
#define GTA_BASE_VMT_HPP
#include <memory>
#include <robin_hood.h>

namespace gta_base::hooking {
class VmtHook {
 public:
  explicit VmtHook(LPVOID vtable);

  void Hook(const std::string &name, std::size_t index, LPVOID detour);
  void Unhook(std::size_t index);

  void Enable(std::size_t index);
  void Disable(std::size_t index);
  void DisableAll();

  template<typename T>
  T GetOriginal(std::size_t index) {
    if (auto it = hooks_.find(index); it == hooks_.end()) {
      LOG_WARN("Tried to get original at idx: {} which doesn't exist", index);
    } else {
      return static_cast<T>(it->second.original);
    }

    return {};
  }
 private:
  struct Hook_t {
    std::string name;
    LPVOID target;
    LPVOID detour;
    LPVOID original;
  };

  LPVOID *vtable_;
  robin_hood::unordered_map<std::size_t, Hook_t> hooks_;

 private:
  void FixHookAddress(const std::string &name, LPVOID target);

  DWORD ExpHandler(PEXCEPTION_POINTERS exp, const std::string &name);
};
}
#endif //GTA_BASE_VMT_HPP
