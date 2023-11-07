//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <memory>
#include <ThreadPool/ThreadPool.h>
#include "render/manager.hpp"

std::atomic<bool> base::globals::kRUNNING = false;

int base::menu_main() {
  hmthrp::ThreadPool thread_pool(std::thread::hardware_concurrency());
  LOG_INFO("[INIT] Created thread pool with {} threads", std::thread::hardware_concurrency());

  std::thread render_thread(&base::render::Thread::RenderMain);
  LOG_INFO("[INIT] Created render thread");

  auto manager_inst = std::make_unique<render::Manager>();
  LOG_INFO("[INIT] render manager");

  while (!render::kTHREAD) {
    std::this_thread::yield();
  }

  render::kTHREAD->AddRenderCallback([](render::DrawQueueBuffer* buffer) {
    buffer->AddCommand(render::Text({0.5, 0.5}, ImColor(255, 0, 0), "Hello World!", false, true, 0.02f));
  });

  while (globals::kRUNNING) {
    if (GetAsyncKeyState(VK_END))
      break;
  }

  // Most likely already shutdown but just in case
  if (render_thread.joinable())
    render_thread.join();
  LOG_INFO("[SHUTDOWN] render thread");

  manager_inst.reset();
  LOG_INFO("[SHUTDOWN] render manager");

  thread_pool.shutdown();
  LOG_INFO("[SHUTDOWN] Thread pool has been shutdown");

  return 0;
}