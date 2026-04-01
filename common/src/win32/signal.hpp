//
// Created by X-ray on 25/12/2023.
//

#ifndef SIGNAL_HPP_02232550
#define SIGNAL_HPP_02232550
#include <string>
#include <Windows.h>
#include "../util/time.hpp"

namespace base::win32 {
  /**
   * \brief A class for thread synchronization using Windows events
   */
  class Signal {
  public:
    Signal() {
      signal_h_ = CreateEvent(nullptr, false, false, (std::to_string(common::util::time::GetTimeStamp()) + "signal_helper").c_str());
    }

    ~Signal() {
      SetEvent(signal_h_);
      CloseHandle(signal_h_);
    }

    /**
     * \brief Notify all threads currently waiting on the signal
     */
    void Notify() const {
      SetEvent(signal_h_);
    }

    /**
     * \brief Infinitely wait for the signal to be notified
     */
    std::size_t Wait() const {
      return Wait(INFINITE);
    }

    /**
     * \brief Wait for a signal notify to occur within a set timeout
     * \param timeout Timeout in milliseconds
     */
    std::size_t Wait(const std::int32_t timeout) const {
      return WaitForSingleObject(signal_h_, timeout);
    }

  private:
    HANDLE signal_h_;
  };
}
#endif //SIGNAL_HPP_02232550