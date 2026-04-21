#include "actuators/pwm_controller.hpp"
#include <iostream>
#include <lgpio.h> // Official Raspberry Pi 5 GPIO library

namespace actuators {

PWMController::PWMController(int gpio_pin) : hardware_pin_(gpio_pin), is_enabled_(false), lgpio_handle_(-1) {
    // Open the RP1 GPIO chip
    lgpio_handle_ = lgGpiochipOpen(4); 
    if (lgpio_handle_ < 0) {
        std::cerr << "[PWM] Error: Could not open lgpio chip for Pi 5! Trying gpiochip0...\n";
        lgpio_handle_ = lgGpiochipOpen(0);
    }
}

PWMController::~PWMController() {
    if (lgpio_handle_ >= 0) {
        lgGpiochipClose(lgpio_handle_);
    }
}

void PWMController::enable() {
    is_enabled_ = true;
    if (lgpio_handle_ >= 0) {
        // Claim the pin explicitly as an output
        lgGpioClaimOutput(lgpio_handle_, 0, hardware_pin_, 0);
    }
    std::cout << "[PWM] Pin " << hardware_pin_ << " enabled via Pi 5 lgpio hardware driver\n";
}

void PWMController::disable() {
    is_enabled_ = false;
    if (lgpio_handle_ >= 0) {
        lgTxPwm(lgpio_handle_, hardware_pin_, 50, 0, 0, 0); // Disable pulses
    }
}

void PWMController::setDutyCycle(float percent) {
    if (!is_enabled_ || lgpio_handle_ < 0) return;
    lgTxPwm(lgpio_handle_, hardware_pin_, 50, percent, 0, 0); 
}

void PWMController::setAngle(float degrees) {
    if (!is_enabled_) return;
    if (lgpio_handle_ < 0) return;

    // Convert degrees to servo PWM duty cycle percentage 
    float mapped_duty = 5.0f + ((degrees + 90.0f) / 180.0f) * 5.0f; 
    
    // Prevent overlapping updates from resetting the pulse train
    static float last_duty = -1.0f;
    float diff = mapped_duty - last_duty;
    if (diff < 0) diff = -diff; 

    // Update if the requested angle shifted
    if (diff > 0.05f) {
        std::cout << "[PWM] Tilt Tracking -> Angle: " << (int)degrees << " deg\n";
        lgTxPwm(lgpio_handle_, hardware_pin_, 50, mapped_duty, 0, 0); 
        last_duty = mapped_duty;
    }
}

} // namespace actuators
