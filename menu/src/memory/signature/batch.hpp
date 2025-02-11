//
// Created by X-ray on 13/04/2024.
//

#ifndef BATCH_HPP
#define BATCH_HPP
#include <ankerl/unordered_dense.h>
#include "pattern.hpp"

namespace base::menu::memory::signature {
  struct BatchJob {
    using cb_t = std::function<void(const Address*)>;

    BatchJob() = default;
    BatchJob(const std::string& name, Pattern pattern, const cb_t& cb) : name(name), pattern(std::make_shared<Pattern>(pattern)), cb(cb) {}

    std::string name;
    std::shared_ptr<Pattern> pattern;
    cb_t cb;
  };

  class BatchScan {
  public:
    void Add(const std::string& name, Pattern pattern, const BatchJob::cb_t& cb) {
      if (scanned_)
        return;

      patterns_[name] = std::move(BatchJob(name, pattern, cb));
    }

    void Scan();

  private:
    bool scanned_ = false;
    ankerl::unordered_dense::map<std::string, BatchJob> patterns_;
  };
}
#endif //BATCH_HPP
