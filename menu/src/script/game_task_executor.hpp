//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <future>
#include "script_base.hpp"

#undef Yield

namespace base::menu::script {
  class GameTaskExecutor : public ScriptBase {
  public:
    class GameTask {
    public:
      explicit GameTask(LPVOID main_fiber, const std::function<void(GameTask* task)>& cb);
      ~GameTask();

      void Tick() const;
      void Yield();
      void Yield(std::chrono::milliseconds duration);

      bool IsDone() const;
      std::future<void> GetFuture() const;

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
    std::vector<GameTask> tasks_;
  };
  inline GameTaskExecutor* kGAME_TASK_EXECUTOR{};
}
