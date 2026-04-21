#pragma once

#include <functional>
#include <thread>
#include <atomic>

namespace drivers {

/**
 * @brief Provides interrupt-driven GPIO inputs using an isolated debouncing 
 * thread natively on the Raspberry Pi 5.
 */
class GPIO {
public:
    GPIO(int pin);
    ~GPIO();

    /**
     * @brief Attaches a fast Interrupt Service Routine (ISR) wrapper.
     */
    void attachInterrupt(std::function<void()> isr);

private:
    int pin_;
    int lgpio_handle_;
    std::function<void()> callback_;
    std::thread poll_thread_;
    std::atomic<bool> running_;
};

} // namespace drivers
