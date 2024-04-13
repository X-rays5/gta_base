//
// Created by X-ray on 02/12/2023.
//

#include "pointers.hpp"
#include "signature/batch.hpp"
#include <chrono>

#define BATCH_SCAN(name, pattern, mod, cb) batch.Add(xorstr_(name), signature::Pattern(xorstr_(pattern), mod), cb)

namespace base::memory {
  Pointers::Pointers() {
    signature::BatchScan batch;

    BATCH_SCAN("swap_chain", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", "", [this](const Address* ptr) {
               swap_chain_ = ptr->Add(3).Rip().As<IDXGISwapChain**>();
               GTA_BASE_ASSERT(swap_chain_, "Invalid swap_chain pointer");
               });

    BATCH_SCAN("resolution", "66 0F 6E 0D ? ? ? ? 0F B7 3D", "", [this](const Address* ptr) {
               LOG_DEBUG("Resolution x: {}, y: {}", *ptr->Sub(4).Rip().As<int*>(), *ptr->Add(4).Rip().As<int*>());
               });

    batch.Scan();
    kPOINTERS = this;
  }

  Pointers::~Pointers() {
    kPOINTERS = nullptr;
  };
}

#undef BATCH_SCAN
