//
// Created by X-ray on 02/12/2023.
//

#ifndef POINTERS_HPP
#define POINTERS_HPP
#include <dxgi.h>

namespace base::memory {
  class Pointers {
  public:
    Pointers();
    ~Pointers();

    IDXGISwapChain** swap_chain_{};
  };

  // skipcq: CXX-W2009
  inline Pointers* kPOINTERS{};
}
#endif //POINTERS_HPP
