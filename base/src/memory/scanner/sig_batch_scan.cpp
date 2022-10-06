#include "sig_batch_scan.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../logger/logger.hpp"

namespace gta_base::memory::scanner {
      void Batch::add(std::string name, char* pattern, std::function<void(Handle)> callback) {
        add(std::move(name), Pattern(pattern), std::move(callback));
      }

      void Batch::add(std::string name, Pattern pattern, std::function<void(Handle)> callback) {
        entries_.emplace_back(std::move(name), std::move(pattern), std::move(callback));
      }

      void Batch::run(Range region) {
        bool all_found = true;
        for (auto& entry : entries_) {
          if (auto result = region.scan(entry.pattern_)) {
            if (entry.cb_) {
              std::invoke(std::move(entry.cb_), result);
#ifndef NDEBUG
              LOG_DEBUG("Found '{}' GTA5.exe+{:x}", entry.name_, (result.as<DWORD64>() - region.begin().as<DWORD64>()));
#else
              LOG_INFO("Found '{}'", entry.name_);
#endif
            }
            else {
              all_found = false;
              LOG_WARN("Failed to find '{}'.", entry.name_);
            }
          }
          else {
            all_found = false;
            LOG_WARN("Failed to find '{}'.", entry.name_);
          }
        }

        entries_.clear();
        if (!all_found) {
#ifndef NDEBUG
          __debugbreak();
#endif
          LOG_CRITICAL("Failed to find some patterns.");
        }
      }
    }