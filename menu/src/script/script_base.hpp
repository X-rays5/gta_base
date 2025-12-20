//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <base-common/concurrency/spinlock.hpp>

namespace base::menu::script {
  class ScriptBase {
  public:
    enum class Type {
      Menu,
      GameScript
    };

  public:
    virtual ~ScriptBase() = default;

    std::string GetName() const { return name_; }

    virtual Type GetType() = 0;
    void Tick() {
      common::concurrency::ScopedSpinlock guard(lock_);
      if (!initialized_) {
        LOG_INFO("Initializing script: {}", name_);
        OnInit();
        initialized_ = true;
      }

      OnTick();
    }

  protected:
    explicit ScriptBase(const std::string& name) : name_(name) {}

    virtual void OnInit() = 0;
    virtual void OnTick() = 0;

  private:
    std::string name_;
    common::concurrency::Spinlock lock_;
    bool initialized_{ false };
  };
}