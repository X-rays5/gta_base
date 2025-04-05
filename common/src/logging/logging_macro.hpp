//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef GTA_BASE_LOGGING_MACRO_019BE36990514312A78AF967214A9577_HPP
#define GTA_BASE_LOGGING_MACRO_019BE36990514312A78AF967214A9577_HPP

#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL 1 // DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL 2 // INFO
#endif

#include <xorstr.hpp>
#include <spdlog/spdlog.h>
#include "../fmt/fs_formatter.hpp"

#ifndef NDEBUG
#define LOGGER_LOG(level, ...) if (auto __logger__ = spdlog::default_logger_raw(); __logger__) { __logger__->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__); } (void)0
#else
#define LOGGER_LOG(level, ...) if (auto __logger__ = spdlog::default_logger_raw(); __logger__) { __logger__->log(spdlog::source_loc{__FILE__, __LINE__, ""}, level, __VA_ARGS__); } (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define LOG_TRACE(...) LOGGER_LOG(spdlog::level::trace, __VA_ARGS__)
#define LOG_TRACE_CONDITIONAL(condition, ...) if (condition) {LOG_TRACE(__VA_ARGS__);}
#else
#define LOG_TRACE(...) (void)0
#define LOG_TRACE_CONDITIONAL(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define LOG_DEBUG(...) LOGGER_LOG(spdlog::level::debug, __VA_ARGS__)
#define LOG_DEBUG_CONDITIONAL(condition, ...) if (condition) {LOG_DEBUG(__VA_ARGS__);}
#else
#define LOG_DEBUG(...)  (void)0
#define LOG_DEBUG_CONDITIONAL(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define LOG_INFO(...) LOGGER_LOG(spdlog::level::info, __VA_ARGS__)
#define LOG_INFO_CONDITIONAL(condition, ...) if (condition) {LOG_INFO(__VA_ARGS__);}
#else
#define LOG_INFO(...)  (void)0
#define LOG_INFO_CONDITIONAL(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define LOG_WARN(...) LOGGER_LOG(spdlog::level::warn, __VA_ARGS__)
#define LOG_WARN_CONDITIONAL(condition, ...) if (condition) {LOG_WARN(__VA_ARGS__);}
#else
#define LOG_WARN(...)  (void)0
#define LOG_WARN_CONDITIONAL(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define LOG_ERROR(...) LOGGER_LOG(spdlog::level::err, __VA_ARGS__)
#define LOG_ERROR_CONDITIONAL(condition, ...) if (condition) {LOG_ERROR(__VA_ARGS__);}
#else
#define LOG_ERROR(...)  (void)0
#define LOG_ERROR_CONDITIONAL(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define LOG_CRITICAL(...) LOGGER_LOG(spdlog::level::critical, __VA_ARGS__)
#define LOG_CRITICAL_CONDITIONAL(condition, ...) if (condition) {LOG_CRITICAL(__VA_ARGS__);}
#else
#define LOG_CRITICAL(...)  (void)0
#define LOG_CRITICAL_CONDITIONAL(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define GTA_BASE_ASSERT(condition, ...) LOG_DEBUG_CONDITIONAL(!(condition), __VA_ARGS__); assert(condition)
#else
#define GTA_BASE_ASSERT(condition, ...) (void)0
#endif
#endif //GTA_BASE_LOGGING_MACRO_019BE36990514312A78AF967214A9577_HPP
