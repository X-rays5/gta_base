//
// Created by X-ray on 25/12/2023.
//

#ifndef SIGNAL_HPP
#define SIGNAL_HPP
#include "../util/common.hpp"

namespace base::win32 {
  class Signal {
    public:
      Signal() { signal_h_ = CreateEvent(nullptr, false, false, (std::to_string(util::common::GetTimeStamp()) + "signal_helper").c_str()); }

      ~Signal() {
        SetEvent(signal_h_);
        CloseHandle(signal_h_);
      }

      void Notify() const { SetEvent(signal_h_); }

      void Wait() const { WaitForSingleObject(signal_h_, INFINITE); }

      void Wait(const std::int32_t timeout) const { WaitForSingleObject(signal_h_, timeout); }

    private:
      HANDLE signal_h_;
  };
}
#endif //SIGNAL_HPP
