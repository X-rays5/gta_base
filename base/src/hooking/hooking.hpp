//
// Created by X-ray on 3/6/2022.
//

#pragma once

#ifndef GTABASE_HOOKING_HPP
#define GTABASE_HOOKING_HPP
#include <memory>
#include "minhook/minhook.hpp"

namespace gta_base {
  class Hooking {
  public:
    Hooking();
    ~Hooking();

    hooking::MinHook& operator->() {
      return mh_hook_;
    }
  private:
    hooking::MinHook mh_hook_;
  };
  inline std::unique_ptr<Hooking> kHOOKING;
}
#endif //GTABASE_HOOKING_HPP
