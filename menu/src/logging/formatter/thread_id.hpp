//
// Created by X-ray on 26/12/2023.
//

#ifndef THREAD_ID_HPP
#define THREAD_ID_HPP
#include "spdlog/pattern_formatter.h"

class ThreadIdFormatter final : public spdlog::custom_flag_formatter {
  public:
    void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override {
      const std::string thread_id = std::to_string(msg.thread_id); // need to somehow get the thread id here
      dest.append(thread_id.data(), thread_id.data() + thread_id.size());
    }

    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override { return spdlog::details::make_unique<ThreadIdFormatter>(); }
};
#endif //THREAD_ID_HPP
