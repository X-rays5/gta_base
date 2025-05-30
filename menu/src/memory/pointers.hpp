//
// Created by X-ray on 02/12/2023.
//

#ifndef POINTERS_HPP
#define POINTERS_HPP
#include <dxgi.h>

namespace base::menu::memory {
  class Pointers {
  public:
    Pointers();
    ~Pointers();

    struct ScreenRes {
      std::uint32_t x;
      std::uint32_t y;
    } screen_res_;

    IDXGISwapChain** swap_chain_{};
  };

  inline Pointers* kPOINTERS{};
}
#endif //POINTERS_HPP
