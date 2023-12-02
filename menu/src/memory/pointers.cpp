//
// Created by X-ray on 02/12/2023.
//

#include "pointers.hpp"
#include "scanner/all.hpp"

#define BATCH_SCAN(batch, name, pattern, cb) batch.add(xorstr_(name), xorstr_(pattern), cb)

namespace base::memory {
  Pointers::Pointers() {
    scanner::Batch main_batch;

    BATCH_SCAN(main_batch, "swap_chain", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", [this](scanner::Handle ptr) {
               swap_chain_ = ptr.add(3).rip().as<IDXGISwapChain**>();
               });

    main_batch.run(scanner::Module(nullptr));

    kPOINTERS = this;
  }

  Pointers::~Pointers() { kPOINTERS = nullptr; };
}

#undef BATCH_SCAN
