//
// Created by X-ray on 02/12/2023.
//

#ifndef POINTERS_HPP
#define POINTERS_HPP
#include <d3d12.h>
#include <dxgi1_4.h>

namespace base::menu::memory {
  class Pointers {
  public:
    Pointers();
    ~Pointers();

    IDXGISwapChain3** swap_chain_{};
    ID3D12CommandQueue** command_queue_{};
  };

  inline Pointers* kPOINTERS{};
}
#endif //POINTERS_HPP
