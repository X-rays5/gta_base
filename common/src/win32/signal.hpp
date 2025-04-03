//
// Created by X-ray on 25/12/2023.
//

#ifndef SIGNAL_HPP
#define SIGNAL_HPP
#include "../util/time.hpp"

namespace base::menu::win32 {
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
    void Wait() const {
      Wait(INFINITE);
    }

    /**
     * \brief Wait for a signal notify to occur within a set timeout
     * \param timeout Timeout in milliseconds
     */
    void Wait(const std::int32_t timeout) const {
      WaitForSingleObject(signal_h_, timeout);
    }

  private:
    HANDLE signal_h_;
  };
}
#endif //SIGNAL_HPP
