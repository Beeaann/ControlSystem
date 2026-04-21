#pragma once

#include <functional>

namespace drivers {

/**
 * @brief Provides interrupt-driven GPIO inputs using epoll/sysfs or 
 * memory mapped GPIO hardware logic on the Raspberry Pi 5.
 */
class GPIO {
public:
    GPIO(int pin);

    /**
     * @brief Attaches a fast Interrupt Service Routine (ISR) wrapper.
     */
    void attachInterrupt(std::function<void()> isr);

private:
    int pin_;
};

} // namespace drivers
