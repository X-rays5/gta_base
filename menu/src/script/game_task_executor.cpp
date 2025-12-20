//
// Created by X-ray on 20/12/2025.
//

#include "game_task_executor.hpp"
#include "../natives/natives_gen9.hpp"

namespace base::menu::script {
  GameTaskExecutor::GameTaskExecutor() : ScriptBase("GameTaskExecutor") {
    kGAME_TASK_EXECUTOR = this;
  }

  GameTaskExecutor::~GameTaskExecutor() {
    kGAME_TASK_EXECUTOR = nullptr;
  }

  std::future<void> GameTaskExecutor::QueueTask(const std::function<void(GameTask* task)>& cb) {
    GameTask task(main_fiber_, cb);
    auto future = task.GetFuture();
    tasks_.emplace_back(std::move(task));
    return future;
  }

  void GameTaskExecutor::OnInit() {
    main_fiber_ = ConvertThreadToFiber(nullptr);
  }

  void GameTaskExecutor::OnTick() {
    for (auto& task : tasks_) {
      task.Tick();
    }

    natives::PLAYER::GET_PLAYER_INDEX();

    tasks_.erase(std::ranges::remove_if(tasks_,
                                        [](const GameTask& task) {
                                          return task.IsDone();
                                        }).begin(),
                 tasks_.end());
  }

  GameTaskExecutor::GameTask::GameTask(const LPVOID main_fiber, const std::function<void(GameTask* task)>& cb) {
    main_fiber_ = main_fiber;
    cb_ = cb;
    promise_ = std::make_shared<std::promise<void>>();
    fiber_ = CreateFiber(0, [](const LPVOID param) {
      auto* task = static_cast<GameTask*>(param);
      task->cb_(task);
      task->done_ = true;
      task->promise_->set_value();
      SwitchToFiber(task->main_fiber_);
    }, this);
  }

  GameTaskExecutor::GameTask::~GameTask() {
    if (fiber_) {
      DeleteFiber(fiber_);
      fiber_ = nullptr;
    }
  }

  void GameTaskExecutor::GameTask::Tick() const {
    if (done_)
      return;

    const auto now = std::chrono::steady_clock::now();
    if (now < wake_time_)
      return;

    SwitchToFiber(fiber_);
  }

  void GameTaskExecutor::GameTask::Yield() {
    Yield(std::chrono::milliseconds(0));
  }

  void GameTaskExecutor::GameTask::Yield(const std::chrono::milliseconds duration) {
    wake_time_ = std::chrono::steady_clock::now() + duration;
    SwitchToFiber(main_fiber_);
  }

  bool GameTaskExecutor::GameTask::IsDone() const {
    return done_;
  }

  std::future<void> GameTaskExecutor::GameTask::GetFuture() const {
    return promise_->get_future();
  }
}
