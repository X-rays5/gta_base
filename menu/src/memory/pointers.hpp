//
// Created by X-ray on 02/12/2023.
//

#ifndef POINTERS_HPP
#define POINTERS_HPP
#include <d3d12.h>
#include <dxgi1_4.h>
#include <rage/script/program.hpp>

namespace base::menu::memory {
  class Pointers {
  public:
    Pointers();
    ~Pointers();

    IDXGISwapChain3** swap_chain_{};
    ID3D12CommandQueue** command_queue_{};
    void(*init_native_tables_)(rage::script::Program*);
    void* run_script_threads_{};
  };

  inline Pointers* kPOINTERS{};
}
#endif //POINTERS_HPP
