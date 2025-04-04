//
// Created by X-ray on 08/04/2024.
//

#ifndef PLH_LOGGER_HPP
#define PLH_LOGGER_HPP
#include <polyhook2/ErrorLog.hpp>

namespace base::menu::logging::PLH {
  class Logger final : public ::PLH::Logger {
  public:
    virtual ~Logger() override = default;

    virtual void log(const std::string& msg, ::PLH::ErrorLevel level) override {
      switch (level) {
      case ::PLH::ErrorLevel::INFO:
        LOG_INFO(msg);
        break;
      case ::PLH::ErrorLevel::WARN:
        LOG_WARN(msg);
        break;
      case ::PLH::ErrorLevel::SEV:
        LOG_CRITICAL(msg);
      default:
        LOG_DEBUG(msg);
      }
    }
  };
}

#endif //PLH_LOGGER_HPP
