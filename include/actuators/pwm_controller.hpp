#pragma once

namespace actuators {

/**
 * @brief Actuator interface dealing with PWM signals for Hardware.
 * Abstracts servo angle positions and DC Motor velocity envelopes.
 */
class PWMController {
public:
    PWMController(int gpio_pin);
    ~PWMController();

    /**
     * @brief Enable/Disable the signal safely. 
     */
    void enable();
    void disable();

    /**
     * @brief Set specific duty cycle for the Motor (0-100%).
     */
    void setDutyCycle(float percent);

    /**
     * @brief Position a Servo to a specific angular degree.
     */
    void setAngle(float degrees);

private:
    int hardware_pin_;
    bool is_enabled_;
    int lgpio_handle_;
};

} // namespace actuators
