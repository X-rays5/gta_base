//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_THREADS_HPP
#define GTABASE_THREADS_HPP
#include <memory>
#include <vector>
#include <mutex>
#include "base_script.hpp"

namespace gta_base {
  class Threads {
  public:
    Threads();
    ~Threads();

    void Tick(threads::ThreadType type);
    void AddScript(const std::shared_ptr<threads::BaseScript>& script);
    void RemoveScript(const std::shared_ptr<threads::BaseScript>& script);

  private:
    std::mutex mtx_;
    std::vector<std::shared_ptr<threads::BaseScript>> scripts_;

  private:

  };
  inline Threads* kTHREADS{};
}

#endif //GTABASE_THREADS_HPP
