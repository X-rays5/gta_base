//
// Created by X-ray on 02/12/2023.
//

#include "pointers.hpp"
#include "signature/cached_batch.hpp"

#define BATCH_SCAN(name, pattern, mod, cb) batch.Add(xorstr_(name), signature::Pattern(xorstr_(pattern), xorstr_(mod)), cb)

namespace base::menu::memory {
  // Warning irrelevant here
#pragma warning(push)
#pragma warning(disable : 4100)

  Pointers::Pointers() {
    signature::BatchScan batch;

    BATCH_SCAN("swap_chain", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", "", [this](const common::memory::Address* ptr) {
               swap_chain_ = ptr->Add(3).Rip().As<IDXGISwapChain**>();
               GTA_BASE_ASSERT(swap_chain_, "Invalid swap_chain pointer");
               });

    BATCH_SCAN("resolution", "66 0F 6E 0D ? ? ? ? 0F B7 3D", "", [this](const common::memory::Address* ptr) {
               screen_res_.x = *ptr->Sub(4).Rip().As<int*>();
               screen_res_.y = *ptr->Add(4).Rip().As<int*>();
               LOG_DEBUG("Resolution x: {}, y: {}", *ptr->Sub(4).Rip().As<int*>(), *ptr->Add(4).Rip().As<int*>());
               });

    BATCH_SCAN("online_version", "4C 8D 0D ? ? ? ? 48 8D 15 ? ? ? ? 48 8D 4C 24 ? 4C 8B C0 E8 ? ? ? ? 48 8D 4C 24 ? B2 ? E8 ? ? ? ? 33 C9", "", [this](const common::memory::Address* ptr) {
               std::string online_version_ = ptr->Add(3).Rip().As<const char*>();
               LOG_INFO("Running version: {}", online_version_);
               });

    BATCH_SCAN("build_version", "4C 8D 3D ? ? ? ? 80 38 ? 0F 84 ? ? ? ? 80 38", "", [this](const common::memory::Address* ptr) {
               std::string buildv_version_ = ptr->Add(3).Rip().As<const char*>();
               LOG_INFO("Build version: {}", buildv_version_);
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
