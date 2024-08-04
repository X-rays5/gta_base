//
// Created by X-ray on 04/08/2024.
//

#ifndef RICH_PRESENCE_HPP_08155936
#define RICH_PRESENCE_HPP_08155936
#include "../util/spinlock.hpp"

namespace base::discord {
   struct Activity {
      std::string state;
      std::string details = fmt::format("Playing GTA V with {}", globals::kBASE_NAME);
      std::string large_image_key = "gta-logo";
      std::string large_image_text;
      std::string small_image_key;
      std::string small_image_text;
      std::int32_t party_size;
      std::int32_t party_max;
   };

   class RichPresence {
   public:
      RichPresence();
      ~RichPresence();

      void Tick();

      void SetState(const char* state) {
         util::ScopedSpinlock lock(lock_);
         activity_.state = state;
      }

      std::string_view GetState() {
         util::ScopedSpinlock lock(lock_);
         return activity_.state;
      }

      void SetDetails(const char* details) {
         util::ScopedSpinlock lock(lock_);
         activity_.details = details;
      }

      std::string_view GetDetails() {
         util::ScopedSpinlock lock(lock_);
         return activity_.details;
      }

      void SetLargeImageKey(const char* large_image_key) {
         util::ScopedSpinlock lock(lock_);
         activity_.large_image_key = large_image_key;
      }

      std::string_view GetLargeImageKey() {
         util::ScopedSpinlock lock(lock_);
         return activity_.large_image_key;
      }

      void SetLargeImageText(const char* large_image_text) {
         util::ScopedSpinlock lock(lock_);
         activity_.large_image_text = large_image_text;
      }

      std::string_view GetLargeImageText() {
         util::ScopedSpinlock lock(lock_);
         return activity_.large_image_text;
      }

      void SetSmallImageKey(const char* small_image_key) {
         util::ScopedSpinlock lock(lock_);
         activity_.small_image_key = small_image_key;
      }

      std::string_view GetSmallImageKey() {
         util::ScopedSpinlock lock(lock_);
         return activity_.small_image_key;
      }

      void SetSmallImageText(const char* small_image_text) {
         util::ScopedSpinlock lock(lock_);
         activity_.small_image_text = small_image_text;
      }

      std::string_view GetSmallImageText() {
         util::ScopedSpinlock lock(lock_);
         return activity_.small_image_text;
      }

      void SetPartySize(std::int32_t party_size) {
         util::ScopedSpinlock lock(lock_);
         activity_.party_size = party_size;
      }

      std::int32_t GetPartySize() {
         util::ScopedSpinlock lock(lock_);
         return activity_.party_size;
      }

      void SetPartyMax(std::int32_t party_max) {
         util::ScopedSpinlock lock(lock_);
         activity_.party_max = party_max;
      }

      std::int32_t GetPartyMax() {
         util::ScopedSpinlock lock(lock_);
         return activity_.party_max;
      }

      void SetActivity(const Activity& activity) {
         util::ScopedSpinlock lock(lock_);
         activity_ = activity;
      }

   private:
      util::Spinlock lock_;
      Activity activity_{};
      std::uint64_t start_time_;
   };

   inline RichPresence* kRICH_PRESENCE{};
}
#endif //RICH_PRESENCE_HPP_08155936
