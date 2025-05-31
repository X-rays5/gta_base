//
// Created by X-ray on 04/08/2024.
//

#ifndef RICH_PRESENCE_HPP_08155936
#define RICH_PRESENCE_HPP_08155936
#include <string>
#include <base-common/globals.hpp>
#include <base-common/concurrency/spinlock.hpp>
#include "../scripts/base_script.hpp"

namespace base::menu::discord {
   struct Activity {
      std::string state;
      std::string details = fmt::format("Playing GTA V with {}", common::globals::kBASE_NAME);
      std::string large_image_key = "gta-logo";
      std::string large_image_text;
      std::string small_image_key;
      std::string small_image_text;
      std::int32_t party_size;
      std::int32_t party_max;
   };

   class RichPresence : public scripts::BaseScript {
   public:
      RichPresence();
      virtual ~RichPresence() override;

      virtual void Tick() override;

      virtual void Init() override;

      virtual Type GetScriptType() const override {
         return Type::MainScriptThread;
      }

      void SetState(const char* state) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.state = state;
      }

      std::string_view GetState() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.state;
      }

      void SetDetails(const char* details) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.details = details;
      }

      std::string_view GetDetails() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.details;
      }

      void SetLargeImageKey(const char* large_image_key) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.large_image_key = large_image_key;
      }

      std::string_view GetLargeImageKey() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.large_image_key;
      }

      void SetLargeImageText(const char* large_image_text) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.large_image_text = large_image_text;
      }

      std::string_view GetLargeImageText() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.large_image_text;
      }

      void SetSmallImageKey(const char* small_image_key) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.small_image_key = small_image_key;
      }

      std::string_view GetSmallImageKey() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.small_image_key;
      }

      void SetSmallImageText(const char* small_image_text) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.small_image_text = small_image_text;
      }

      std::string_view GetSmallImageText() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.small_image_text;
      }

      void SetPartySize(std::int32_t party_size) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.party_size = party_size;
      }

      std::int32_t GetPartySize() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.party_size;
      }

      void SetPartyMax(std::int32_t party_max) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_.party_max = party_max;
      }

      std::int32_t GetPartyMax() {
         common::concurrency::ScopedSpinlock lock(lock_);
         return activity_.party_max;
      }

      void SetActivity(const Activity& activity) {
         common::concurrency::ScopedSpinlock lock(lock_);
         activity_ = activity;
      }

   private:
      common::concurrency::Spinlock lock_;
      Activity activity_{};
      std::uint64_t start_time_;
   };

   inline RichPresence* kRICH_PRESENCE{};
}
#endif //RICH_PRESENCE_HPP_08155936
