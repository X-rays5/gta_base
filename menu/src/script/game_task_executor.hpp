//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <future>
#include <memory>
#include <vector>
#include "script_base.hpp"
#include "../natives/natives_gen9.hpp"

#undef Yield

namespace base::menu::script {
  class GameTaskExecutor : public ScriptBase {
  public:
    class GameTask {
    public:
      friend class GameTaskExecutor;

      explicit GameTask(LPVOID main_fiber, const std::function<void(GameTask* task)>& cb);
      ~GameTask();

      GameTask(const GameTask&) = delete;
      GameTask(GameTask&&) = delete;
      GameTask& operator=(const GameTask&) = delete;
      GameTask& operator=(GameTask&&) = delete;


      void Yield();
      void Yield(std::chrono::milliseconds duration);

      bool IsDone() const;
      std::future<void> GetFuture() const;

    protected:
      void Tick() const;

    private:
      std::function<void(GameTask* task)> cb_;
      LPVOID fiber_{nullptr};
      LPVOID main_fiber_{nullptr};
      std::chrono::steady_clock::time_point wake_time_;
      bool done_{false};
      std::shared_ptr<std::promise<void>> promise_;
    };

  public:
    GameTaskExecutor();
    ~GameTaskExecutor() override;

    Type GetType() override {
      return Type::GameScript;
    }

    std::future<void> QueueTask(const std::function<void(GameTask* task)>& cb);

  protected:
    void OnInit() override;
    void OnTick() override;

  private:
    LPVOID main_fiber_{nullptr};
    std::vector<std::unique_ptr<GameTask>> tasks_;
  };
  inline GameTaskExecutor* kGAME_TASK_EXECUTOR{};
}
