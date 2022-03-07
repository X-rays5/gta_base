//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_MINHOOK_HPP
#define GTABASE_MINHOOK_HPP
#include <unordered_map>
#include <string>
#include <MinHook.h>

namespace gta_base {
  namespace hooking {
    class MinHook {
    public:
      MinHook();
      ~MinHook();

      MH_STATUS AddHook(const std::string& name, LPVOID src, LPVOID dst, LPVOID* og);
      MH_STATUS RemoveHook(const std::string& name);
    private:
      std::unordered_map<std::string, LPVOID> hooks;
    };
  }
}
#endif //GTABASE_MINHOOK_HPP
