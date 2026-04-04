//
// Created by X-ray on 04/04/2026.
//

#include "lua_coroutine.hpp"

#include <base-coro/coroutine.hpp>

#include "../../script/game_task_executor.hpp"

namespace base::menu::lua {
  namespace {
    struct LuaFuture {
      std::future<void> fut;

      bool ready() const {
        return fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
      }
    };

    void yield() {
      minicoropp::this_coro::yield();
    }

    void sleep(const std::size_t ms) {
      minicoropp::this_coro::sleep_for(std::chrono::milliseconds(ms));
    }

    LuaFuture CreateNew(sol::function&& func) {
      LuaFuture lf{
        script::kGAME_TASK_EXECUTOR->QueueTask([func] {
            const auto res = func();
            if (!res.valid()) {
                const sol::error err = res;
                LOG_ERROR("Coroutine error: {}", err.what());
            }
        })
    };
      return lf;
    }
  }

  sol::table SetupCoroutine(sol::state& lua) {
    auto coro_table = lua.create_named_table("coroutine");
    coro_table.set_function("yield", sol::overload(yield, sleep));
    coro_table.set_function("create", CreateNew);

    sol::usertype<LuaFuture> lua_future = lua.new_usertype<LuaFuture>("Future");
    lua_future.set_function("ready", &LuaFuture::ready);

    return coro_table;
  }
}
