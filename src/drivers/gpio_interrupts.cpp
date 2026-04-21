#include "drivers/gpio_interrupts.hpp"
#include <iostream>
#include <lgpio.h>

namespace drivers {

GPIO::GPIO(int pin) : pin_(pin), lgpio_handle_(-1), running_(false) {
    lgpio_handle_ = lgGpiochipOpen(4);
    if (lgpio_handle_ < 0) lgpio_handle_ = lgGpiochipOpen(0);
    
    if (lgpio_handle_ >= 0) {
        // Configure pin as an input with an internal pull-up resistor
        lgGpioClaimInput(lgpio_handle_, LG_SET_PULL_UP, pin_);
    }
}

GPIO::~GPIO() {
    running_ = false;
    if (poll_thread_.joinable()) poll_thread_.join();
    if (lgpio_handle_ >= 0) lgGpiochipClose(lgpio_handle_);
}

void GPIO::attachInterrupt(std::function<void()> isr) {
    if (lgpio_handle_ < 0) return;
    
    callback_ = isr;
    running_ = true;
    
    std::cout << "[GPIO] Monitoring Pin " << pin_ << " for physical interrupts in background...\n";
    
    // Background polling thread to catch pin drops
    poll_thread_ = std::thread([this]() {
        int last_state = 1;
        while (running_) {
            int state = lgGpioRead(lgpio_handle_, pin_);
            
            // Trigger dynamic lambda on the falling edge
            if (state == 0 && last_state == 1) {
                if (callback_) callback_();
            }
            last_state = state;
            
            // Analog debouncing and CPU preservation
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
        }
    });
}

} // namespace drivers
