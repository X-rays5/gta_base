#pragma once
#include <functional>
#include <vector>
#include "pattern.hpp"
#include "handle.hpp"
#include "range.hpp"
#include "fwdec.hpp"

namespace base::memory::scanner {
  class Batch {
    public:
      explicit Batch() = default;

      ~Batch() noexcept = default;

      void add(std::string name, char* pattern, std::function<void(Handle)> callback);

      void add(std::string name, Pattern pattern, std::function<void(Handle)> callback);

      void run(Range region);

      struct entry {
        std::string name_;
        Pattern pattern_;
        std::function<void(Handle)> cb_;

        explicit entry(std::string name, Pattern pattern, std::function<void(Handle)> callback)
          :
          name_(std::move(name)),
          pattern_(std::move(pattern)),
          cb_(std::move(callback)) {}
      };

    private:
      std::vector<entry> entries_;
  };
}
