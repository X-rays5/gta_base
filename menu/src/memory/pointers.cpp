//
// Created by X-ray on 02/12/2023.
//

#include "pointers.hpp"

#include "signature/batch.hpp"

#define BATCH_SCAN(name, pattern, mod, cb) batch.Add(xorstr_(name), signature::Pattern(xorstr_(pattern), xorstr_(mod)), cb)

namespace base::menu::memory {
  // Warning irrelevant here
#pragma warning(push)
#pragma warning(disable : 4100)

  Pointers::Pointers() {
    signature::BatchScan batch;

    BATCH_SCAN("swap_chain", "72 C7 EB 02 31 C0 8B 0D", "", [this](const common::memory::Address* ptr) {
      swap_chain_ = ptr->Add(0x21).Add(3).Rip().As<IDXGISwapChain3**>();
      command_queue_ = ptr->Add(0x1A).Add(3).Rip().As<ID3D12CommandQueue**>();
    });

    batch.Scan();
    kPOINTERS = this;
  }

#pragma warning(pop)

  Pointers::~Pointers() {
    kPOINTERS = nullptr;
  };
}

#undef BATCH_SCAN
