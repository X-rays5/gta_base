//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_DISCORD_HPP
#define GTABASE_DISCORD_HPP
#include <memory>
#include <atomic>

namespace gta_base {
  namespace rpc {
    class Discord {
    public:
      Discord();
      ~Discord();

      void Tick();

      void SetStatus(const char* status) {
        state_ = status;
      }

      void SetDetails(const char* details) {
        details_ = details;
      }

      void SetLargeImage(const char* image) {
        large_image_key_ = image;
      }

      void SetLargeImageText(const char* text) {
        large_image_text_ = text;
      }

      void SetSmallImage(const char* image) {
        small_image_key_ = image;
      }

      void SetSmallImageText(const char* text) {
        small_image_text_ = text;
      }
    private:
      std::atomic<const char*> state_ = "";
      std::atomic<const char*> details_ = "";
      std::atomic<const char*> large_image_key_ = "";
      std::atomic<const char*> large_image_text_ = "";
      std::atomic<const char*> small_image_key_ = "";
      std::atomic<const char*> small_image_text_ = "";
    };
    inline std::unique_ptr<Discord> kDISCORD;
  }
}
#endif //GTABASE_DISCORD_HPP
