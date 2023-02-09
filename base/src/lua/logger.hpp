//
// Created by X-ray on 02/09/23.
//

#pragma once
#ifndef GTA_BASE_LOGGER_B4BDF766A617476AB3EE335FC5D4AAF7_HPP
#define GTA_BASE_LOGGER_B4BDF766A617476AB3EE335FC5D4AAF7_HPP
namespace gta_base::lua {
  class Logger {
  public:
    Logger(sol::state* L, std::string script_name);
    ~Logger();

    template<typename... Args>
    FORCE_INLINE void Trace(Args&& ... args) {
      logger_inst_->trace(std::forward<Args>(args)...);
    }

    template<typename... Args>
    FORCE_INLINE void Debug(Args&& ... args) {
      logger_inst_->debug(std::forward<Args>(args)...);
    }

    template<typename... Args>
    FORCE_INLINE void Info(Args&& ... args) {
      logger_inst_->info(std::forward<Args>(args)...);
    }

    template<typename... Args>
    FORCE_INLINE void Warn(Args&& ... args) {
      logger_inst_->warn(std::forward<Args>(args)...);
    }

    template<typename... Args>
    FORCE_INLINE void Error(Args&& ... args) {
      logger_inst_->error(std::forward<Args>(args)...);
    }

    template<typename... Args>
    FORCE_INLINE void Critical(Args&& ... args) {
      logger_inst_->critical(std::forward<Args>(args)...);
    }

    [[nodiscard]] spdlog::logger* GetLogger() {
      return logger_inst_.get();
    }

  private:
    std::string script_name_;
    sol::state* lua_state_;
    std::shared_ptr<spdlog::logger> logger_inst_;

  private:

    inline std::filesystem::path GetBasePath() {
      static const std::filesystem::path path = common::GetLogDir() / script_name_;
      if (!std::filesystem::exists(path))
        std::filesystem::create_directories(path);

      return path;
    }

    inline std::filesystem::path GetLogSaveDir() {
      static const std::filesystem::path path = GetBasePath() / "save";
      if (!std::filesystem::exists(path))
        std::filesystem::create_directories(path);

      return path;
    }

    inline std::filesystem::path GetLogFile() {
      static const std::filesystem::path path = GetBasePath() / fmt::format("{}.log", script_name_);

      return path;
    }

    inline void SaveLogFile() {
      if (!std::filesystem::exists(GetLogFile()))
        return;

      std::filesystem::rename(GetLogFile(), GetLogSaveDir() / fmt::format("{}_{}", common::GetEpoch(), GetLogFile().filename().string()));
    }
  };
}

#define LUA_LOGGER_LOG(logger, level, ...) (logger)->log(spdlog::source_loc{}, level, fmt::format("[{}/{}:{}] {}", GetScriptName(), GetCurrentFile(), GetCurrentLine(), __VA_ARGS__))

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define LUA_LOG_TRACE(logger_inst, ...) LUA_LOGGER_LOG(logger_inst.GetLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define LUA_LOG_TRACE(logger_inst, ...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define LUA_LOG_DEBUG(logger_inst, ...) LUA_LOGGER_LOG(logger_inst.GetLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define LUA_LOG_DEBUG(logger_inst, ...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define LUA_LOG_INFO(logger_inst, ...) LUA_LOGGER_LOG(logger_inst.GetLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define LUA_LOG_INFO(logger_inst, ...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define LUA_LOG_WARN(logger_inst, ...) LUA_LOGGER_LOG(logger_inst.GetLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define LUA_LOG_WARN(logger_inst, ...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define LUA_LOG_ERROR(logger_inst, ...) LUA_LOGGER_LOG(logger_inst.GetLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define LUA_LOG_ERROR(logger_inst, ...)  (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define LUA_LOG_CRITICAL(logger_inst, ...) LUA_LOGGER_LOG(logger_inst.GetLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define LUA_LOG_CRITICAL(logger_inst, ...)  (void)0
#endif

#endif //GTA_BASE_LOGGER_B4BDF766A617476AB3EE335FC5D4AAF7_HPP
