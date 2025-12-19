//
// Created by X-ray on 25/12/2023.
//

#ifndef SIGNAL_HPP_02232550
#define SIGNAL_HPP_02232550
#include <chrono>
#include <semaphore>

namespace base::win32 {
  /**
   * \brief A class for thread synchronization using std::binary_semaphore
   */
  class Signal {
  public:
    Signal() : signal_(0) {}

    ~Signal() {
      (void)signal_.try_acquire();
      signal_.release();
    }

    /**
     * \brief Notify all threads currently waiting on the signal
     */
    void Notify() const {
      (void)signal_.try_acquire();
      signal_.release();
    }

    /**
     * \brief Infinitely wait for the signal to be notified
     */
    void Wait() const {
      Wait(-1);
    }

    /**
     * \brief Wait for a signal notify to occur within a set timeout
     * \param timeout Timeout in milliseconds
     */
    void Wait(const std::int32_t timeout) const {
      if (timeout < 0) {
        signal_.acquire();
      } else {
        (void)signal_.try_acquire_for(std::chrono::milliseconds(timeout));
      }
    }

  private:
    mutable std::binary_semaphore signal_;
  };
}
#endif //SIGNAL_HPP_02232550
