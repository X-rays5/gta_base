//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#define BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
#include "draw.hpp"
#include "render_hook.hpp"

namespace base::render {
  class Manager {
  public:

    inline Draw GetDraw() {
      return draw_;
    }

  private:
    Draw draw_;
    Hook hook_;

  private:

  };

  inline Manager* kMANAGER{};
}
#endif //BASE_MODULES_MANAGER_291A40EA31B145B997BBD872BCDC21D6_HPP
