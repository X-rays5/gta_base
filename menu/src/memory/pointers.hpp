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

    IDXGISwapChain** swap_chain_{};
  };

  inline Pointers* kPOINTERS{};
}
#endif //POINTERS_HPP
