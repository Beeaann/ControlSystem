#include "drivers/gpio_interrupts.hpp"
#include <iostream>

namespace drivers {

GPIO::GPIO(int pin) : pin_(pin) {}

void GPIO::attachInterrupt(std::function<void()> isr) {
    std::cout << "[GPIO] Attached Interrupt Service Routine to Pin " << pin_ << "\n";
    // Sysfs edge triggers and poll logic belong here loop thread.
}

} // namespace drivers
