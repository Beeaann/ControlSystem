#include "actuators/pwm_controller.hpp"
#include <iostream>

namespace actuators {

PWMController::PWMController(int gpio_pin) : hardware_pin_(gpio_pin), is_enabled_(false) {}

void PWMController::enable() {
    is_enabled_ = true;
    std::cout << "[PWM] Pin " << hardware_pin_ << " enabled\n";
}

void PWMController::disable() {
    is_enabled_ = false;
    std::cout << "[PWM] Pin " << hardware_pin_ << " disabled\n";
}

void PWMController::setDutyCycle(float percent) {
    if (!is_enabled_) return;
    std::cout << "[PWM] Pin " << hardware_pin_ << " -> Duty Cycle: " << percent << "%\n";
}

void PWMController::setAngle(float degrees) {
    if (!is_enabled_) return;
    std::cout << "[PWM] Pin " << hardware_pin_ << " -> Angle: " << degrees << " deg\n";
}

} // namespace actuators
