//
// Created by X-ray on 3/6/2022.
//

#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "logger/logger.hpp"
#include "common/common.hpp"
#include "hooking/hooking.hpp"
#include "memory/pointers.hpp"
#include "threads/threads.hpp"
#include "scripts/game_main/game_script.hpp"
#include "scripts/render_main/render_script.hpp"
#include "scripts/scripting_main/main_script.hpp"
#include "rpc/discord.hpp"


std::atomic<bool> gta_base::common::globals::running = true;
void BaseMain() {
  using namespace gta_base;

  kLOGGER = std::make_unique<Logger>();
  memory::kPOINTERS = std::make_unique<memory::Pointers>();

  kHOOKING = std::make_unique<Hooking>();

  kTHREADS = std::make_unique<Threads>();

  kTHREADS->AddScript(std::make_shared<scripts::Game>());
  kTHREADS->AddScript(std::make_shared<scripts::Render>());
  kTHREADS->AddScript(std::make_shared<scripts::Main>());

  rpc::kDISCORD = std::make_unique<rpc::Discord>();

  std::thread([]{
    while(common::globals::running)
      kTHREADS->Tick(threads::ThreadType::kScripting);
  }).detach();

  while (common::globals::running) {
    rpc::kDISCORD->Tick();

    if (common::KeyState(VK_ESCAPE))
      common::globals::running = false;

    Sleep(500);
  }

  rpc::kDISCORD.reset();

  kTHREADS.reset();

  kHOOKING.reset();

  memory::kPOINTERS.reset();
  LOG_INFO("successful unload");
  gta_base::kLOGGER.reset();
}
