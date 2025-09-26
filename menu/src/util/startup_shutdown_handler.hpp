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

    struct Handler {
      std::string name;
      cb_t callback;
    };

  public:
    StartupShutdownHandler() = default;

    ~StartupShutdownHandler() {
      LOG_INFO("[SHUTDOWN] StartupShutdownHandler");
      for (auto it = cb_vec_.rbegin(); it != cb_vec_.rend(); ++it) {
        auto& [name, callback] = *it;
        LOG_INFO("[SHUTDOWN] {}", name);
        callback(Action::Shutdown);
        LOG_DEBUG("[SHUTDOWN] {} completed", name);
      }
    }

    void RunInit() const {
      LOG_INFO("[INIT] StartupShutdownHandler");
      for (const auto& [name, callback] : cb_vec_) {
        LOG_INFO("[INIT] {}...", name);
        callback(Action::Init);
        LOG_DEBUG("[INIT] {} completed", name);
      }
    }

    void AddCallback(const std::string& name, const cb_t& cb) {
      cb_vec_.push_back({name, cb});
    }

  private:
    std::vector<Handler> cb_vec_;
  };
}

#define GTA_BASE_DEFAULT_START_DOWN_HANDLER(inst, init_name, manager_var, ...) inst->AddCallback(xorstr_(init_name), [](::base::menu::util::StartupShutdownHandler::Action action) { \
if (action == ::base::menu::util::StartupShutdownHandler::Action::Init) { \
manager_var = std::make_unique<std::remove_reference_t<decltype(*manager_var)>>(__VA_ARGS__); \
} \
else { \
manager_var.reset(); \
} \
})

#endif //STARTUP_SHUTDOWN_HANDLER_HPP_05172732
