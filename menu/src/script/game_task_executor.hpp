//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <future>
#include <memory>
#include <vector>
#include <base-common/coro/coroutine.hpp>
#include "script_base.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::script {
  class GameTaskExecutor : public ScriptBase {
  public:
    class GameTask {
    public:
      friend class GameTaskExecutor;

      explicit GameTask(const std::function<void()>& cb);
      ~GameTask() = default;

      GameTask(const GameTask&) = delete;
      GameTask(GameTask&&) = delete;
      GameTask& operator=(const GameTask&) = delete;
      GameTask& operator=(GameTask&&) = delete;

      bool IsDone() const;
      std::future<void> GetFuture() const;

    protected:
      void Tick();

    private:
      std::function<void()> cb_;
      bool done_{false};
      std::shared_ptr<std::promise<void>> promise_;
      std::unique_ptr<common::coroutine::Coroutine> coro_;
    };

  public:
    GameTaskExecutor();
    ~GameTaskExecutor() override;

    Type GetType() override {
      return Type::GameScript;
    }

    std::future<void> QueueTask(const std::function<void()>& cb);

  protected:
    void OnInit() override;
    void OnTick() override;

  private:
    std::vector<std::unique_ptr<GameTask>> tasks_;
  };
  inline GameTaskExecutor* kGAME_TASK_EXECUTOR{};
}
