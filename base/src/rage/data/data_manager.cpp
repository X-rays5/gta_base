//
// Created by X-ray on 11/03/22.
//

#include "data_manager.hpp"

namespace rage::data {
  Manager::Manager() {
    kDATA_MANAGER = this;
  }

  Manager::~Manager() {
    kDATA_MANAGER = nullptr;
  }

  void Manager::SetState(State state) {
    state_ = state;
  }

  Manager::State Manager::GetState() const {
    return state_;
  }

  std::recursive_mutex* Manager::GetLoaderMtx() {
    return &loader_mtx_;
  }
}