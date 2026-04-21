#include "drivers/gpio_interrupts.hpp"
#include <iostream>
#include <lgpio.h> // Physical hardware wrapper

namespace drivers {

GPIO::GPIO(int pin) : pin_(pin), lgpio_handle_(-1), running_(false) {
    // Open communication to 40-pin header
    lgpio_handle_ = lgGpiochipOpen(4);
    if (lgpio_handle_ < 0) lgpio_handle_ = lgGpiochipOpen(0);
    
    if (lgpio_handle_ >= 0) {
        // Claim the pin explicitly as an INPUT with an internal PULL-UP resistor!
        // This stops the pin from "floating" and acting like an antenna grabbing random voltages.
        lgGpioClaimInput(lgpio_handle_, LG_SET_PULL_UP, pin_);
    }
}

GPIO::~GPIO() {
    running_ = false;
    // Wait safely for the background thread to exit before deleting the class
    if (poll_thread_.joinable()) poll_thread_.join();
    if (lgpio_handle_ >= 0) lgGpiochipClose(lgpio_handle_);
}

void GPIO::attachInterrupt(std::function<void()> isr) {
    if (lgpio_handle_ < 0) return;
    
    callback_ = isr;
    running_ = true;
    
    std::cout << "[GPIO] Monitoring Pin " << pin_ << " for physical interrupts in background...\n";
    
    // Spin up an isolated background thread so the main Sub-10ms logic loop is never starved 
    poll_thread_ = std::thread([this]() {
        int last_state = 1; // It starts HIGH due to our pull-up
        while (running_) {
            int state = lgGpioRead(lgpio_handle_, pin_);
            
            // If it abruptly changed from HIGH to LOW (Falling Edge logic)
            if (state == 0 && last_state == 1) {
                if (callback_) callback_(); // Execute the specific lambda passed dynamically
            }
            last_state = state;
            
            // Prevent burning unneeded CPU cycles and inherently provide 5ms of analog debouncing
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
        }
    });
}

} // namespace drivers
