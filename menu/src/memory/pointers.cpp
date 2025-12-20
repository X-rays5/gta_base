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

    BATCH_SCAN("init_native_tables", "EB 2A 0F 1F 40 00 48 8B 54 17 10", "", [this](const common::memory::Address* ptr) {
      init_native_tables_ = ptr->Sub(0x2A).As<decltype(init_native_tables_)>();
    });

    BATCH_SCAN("run_script_threads", "BE 40 5D C6 00", "", [this](const common::memory::Address* ptr) {
      run_script_threads_ = ptr->Sub(0xA).As<decltype(run_script_threads_)>();
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
