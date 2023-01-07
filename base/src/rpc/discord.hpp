//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_DISCORD_HPP
#define GTABASE_DISCORD_HPP
#include <memory>
#include <string>

namespace gta_base::rpc {
class Discord {
 public:
  Discord();
  ~Discord();

  void Tick();

  void SetStatus(const std::string &status) {
    state_ = status;
  }

  void SetDetails(const std::string &details) {
    details_ = details;
  }

  void SetLargeImage(const std::string &image) {
    large_image_key_ = image;
  }

  void SetLargeImageText(const std::string &text) {
    large_image_text_ = text;
  }

  void SetSmallImage(const std::string &image) {
    small_image_key_ = image;
  }

  void SetSmallImageText(const std::string &text) {
    small_image_text_ = text;
  }
 private:
  std::string state_;
  std::string details_;
  std::string large_image_key_;
  std::string large_image_text_;
  std::string small_image_key_;
  std::string small_image_text_;
  std::int64_t start_time_;
};
inline Discord *kDISCORD{};
}
#endif //GTABASE_DISCORD_HPP
