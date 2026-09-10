//
// Created by X-ray on 26/12/2023.
//

#ifndef THREAD_ID_HPP
#define THREAD_ID_HPP
#include "../conversion/cw2a.hpp"
#include "spdlog/pattern_formatter.h"

inline thread_local std::unordered_map<std::size_t, HANDLE> thread_handles = {};

class ThreadIdFormatter final : public spdlog::custom_flag_formatter {
public:
  virtual void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override {
    auto handle_kv = thread_handles.find(msg.thread_id);
    if (handle_kv == thread_handles.end() || handle_kv->second == nullptr) {
      handle_kv = thread_handles.insert_or_assign(msg.thread_id, OpenThread(THREAD_QUERY_INFORMATION, FALSE, (DWORD)msg.thread_id)).first;
    }

    std::string thread_id;
    if (handle_kv->second != nullptr) {
      PWSTR thread_description;
      if (const HRESULT hr = GetThreadDescription(handle_kv->second, &thread_description); SUCCEEDED(hr)) {
        thread_id = fmt::format("{}", std::string(base::common::conversion::CW2A(thread_description)));
        LocalFree(thread_description);
      }
    }

    if (thread_id.empty()) {
      thread_id = std::to_string(msg.thread_id);
    }

    dest.append(thread_id.data(), thread_id.data() + thread_id.size());
  }

  [[nodiscard]] virtual std::unique_ptr<custom_flag_formatter> clone() const override {
    return spdlog::details::make_unique<ThreadIdFormatter>();
  }
};
#endif //THREAD_ID_HPP
