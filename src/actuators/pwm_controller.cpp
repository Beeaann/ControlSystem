#include "actuators/pwm_controller.hpp"
#include <iostream>
#include <lgpio.h> // The official Raspberry Pi 5 GPIO library

namespace actuators {

PWMController::PWMController(int gpio_pin) : hardware_pin_(gpio_pin), is_enabled_(false), lgpio_handle_(-1) {
    // Open the default RP1 GPIO chip on the Pi 5 (gpiochip4 usually houses the 40-pin header)
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
    std::cout << "[PWM] Pin " << hardware_pin_ << " enabled via Pi 5 lgpio hardware driver\n";
}

void PWMController::disable() {
    is_enabled_ = false;
    if (lgpio_handle_ >= 0) {
        lgTxPwm(lgpio_handle_, hardware_pin_, 50, 0, 0, 0); // 0% duty cycle
    }
}

void PWMController::setDutyCycle(float percent) {
    if (!is_enabled_ || lgpio_handle_ < 0) return;
    lgTxPwm(lgpio_handle_, hardware_pin_, 50, percent, 0, 0); 
}

void PWMController::setAngle(float degrees) {
    if (!is_enabled_) return;
    std::cout << "[PWM] Pin " << hardware_pin_ << " -> Actual Hardware Sweep: " << degrees << " deg\n";
    
    if (lgpio_handle_ < 0) return;

    // Standard Servos require a 50Hz frequency. 
    // They detect angle based on Duty Cycle (usually 5% is -90 deg, and 10% is +90 deg).
    float mapped_duty = 5.0f + ((degrees + 90.0f) / 180.0f) * 5.0f; 
    
    // Command: lgTxPwm(handle, pin, frequency, duty_percent, pulse_cycles, pulse_offset)
    lgTxPwm(lgpio_handle_, hardware_pin_, 50, mapped_duty, 0, 0); 
}

} // namespace actuators
