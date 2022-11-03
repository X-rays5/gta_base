//
// Created by X-ray on 11/03/22.
//

#pragma once
#ifndef GTA_BASE_DATA_MANAGER_HPP
#define GTA_BASE_DATA_MANAGER_HPP
#include <mutex>

namespace rage::data {
  class Manager {
  public:
    enum State {
      kIDLE,
      kREBUILDING,
      kLOADING,
    };

  public:
    Manager();
    ~Manager();

    void SetState(State state);
    [[nodiscard]] State GetState() const;

    [[nodiscard]] std::recursive_mutex* GetLoaderMtx();
  private:
    std::recursive_mutex loader_mtx_;
    State state_ = kIDLE;


  private:

  };
  inline Manager* kDATA_MANAGER{};
}
#endif //GTA_BASE_DATA_MANAGER_HPP
