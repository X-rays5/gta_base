//
// Created by X-ray on 20/12/2025.
//

#include "game_task_executor.hpp"

namespace base::menu::script {
  GameTaskExecutor::GameTaskExecutor() : TaskExecutor("GameTaskExecutor") {
    kGAME_TASK_EXECUTOR = this;
  }

  GameTaskExecutor::~GameTaskExecutor() {
    kGAME_TASK_EXECUTOR = nullptr;
  }
}
