//
// Created by X-ray on 31/05/2025.
//

#ifndef STARTUP_SHUTDOWN_HANDLER_HPP_05172732
#define STARTUP_SHUTDOWN_HANDLER_HPP_05172732
#include <functional>

namespace base::menu::util {
  class StartupShutdownHandler {
  public:
    enum class Action {
      Init,
      Shutdown
    };

    using cb_t = std::function<void(Action)>;

  public:
    StartupShutdownHandler() = default;

    ~StartupShutdownHandler() {
      LOG_INFO("[SHUTDOWN] StartupShutdownHandler");
      for (auto it = cb_vec_.rbegin(); it != cb_vec_.rend(); ++it)
        (*it)(Action::Shutdown);
    }

    void RunInit() const {
      LOG_INFO("[INIT] StartupShutdownHandler");
      for (auto& cb : cb_vec_)
        cb(Action::Init);
    }

    void AddCallback(const cb_t& cb) {
      cb_vec_.push_back(cb);
    }

  private:
    std::vector<cb_t> cb_vec_;
  };
}

#define GTA_BASE_DEFAULT_START_DOWN_HANDLER(inst, init_name, manager_var, ...) inst->AddCallback([](::base::menu::util::StartupShutdownHandler::Action action) { \
if (action == ::base::menu::util::StartupShutdownHandler::Action::Init) { \
manager_var = std::make_unique<std::remove_reference_t<decltype(*manager_var)>>(__VA_ARGS__); \
LOG_INFO("[INIT] {}", xorstr_(init_name)); \
} \
else { \
manager_var.reset(); \
LOG_INFO("[SHUTDOWN] {}", xorstr_(init_name)); \
} \
})

#endif //STARTUP_SHUTDOWN_HANDLER_HPP_05172732
