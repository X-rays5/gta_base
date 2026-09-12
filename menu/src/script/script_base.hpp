//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <base-common/concurrency/spinlock.hpp>

namespace base::menu::script {
  class ScriptBase {
  public:
    /**
     * Which loop ticks a host, and therefore which thread its code runs on.
     *
     * `General` has a loop of its own - see ScriptManager - which exists to give work somewhere to go
     * that is neither the menu's nor the game's: it runs continuously and parks while it is idle, so a
     * host of this type may block without costing a frame.
     */
    enum class Type {
      Menu,
      GameScript,
      General
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