#include <iostream>
#include <thread>
#include <chrono>

#include "core/scheduling.hpp"
#include "core/fault_handler.hpp"
#include "fsm/state_machine.hpp"
#include "actuators/pwm_controller.hpp"
#include "sensors/mpu6050.hpp"
#include "drivers/i2c_bus.hpp"
#include "drivers/gpio_interrupts.hpp"
#include "camera/camera_logger.hpp"

// Target loop timing constraint
constexpr auto TARGET_CYCLE_TIME = std::chrono::milliseconds(10);

int main() {
    std::cout << "Booting Vision-Enhanced Autonomous Control System..." << std::endl;

    if (!core::Scheduler::setThreadRealtimePriority(95)) {
         std::cerr << "Warning: Could not set SCHED_FIFO! Running with standard scheduling..." << std::endl;
    }

    core::FaultHandler fault_handler;
    fsm::StateMachine fsm;
    actuators::PWMController servo(18); // Servo on GPIO 18
    drivers::I2CBus i2c_bus(1);         // I2C bus 1
    sensors::MPU6050 mpu(i2c_bus, 0x68);
    drivers::GPIO emergency_button(17); // Button on GPIO 17
    camera::CameraLogger cam;
    
    // Interrupt handler: logs fault, snaps photo, and shuts down safely
    emergency_button.attachInterrupt([&fsm, &fault_handler, &cam]() {
        fault_handler.registerFault(core::InitialFaultLevel::FATAL, "Physical Emergency Button Triggered!");
        cam.captureEvent("Emergency_Override");
        fsm.transitionTo(fsm::SystemState::SHUTDOWN);
    });
    
    fsm.transitionTo(fsm::SystemState::INIT);
    servo.enable();
    i2c_bus.initialize();
    mpu.initialize();

    std::cout << "Entering Deterministic Core Loop..." << std::endl;

    // Main RT loop
    while (fsm.getCurrentState() != fsm::SystemState::SHUTDOWN) {
        auto step_start = std::chrono::steady_clock::now();

        fsm.updateTick();

        // Skip idle directly into active control
        if (fsm.getCurrentState() == fsm::SystemState::IDLE) {
            fsm.transitionTo(fsm::SystemState::ACTIVE_CONTROL);
        }

        // Active control logic mapping sensors to actuators
        if (fsm.getCurrentState() == fsm::SystemState::ACTIVE_CONTROL) {
            mpu.readRawData();
            servo.setAngle(mpu.getPitch());
        }

        // Suspend thread to prevent loop jitter
        auto next_tick_time = step_start + TARGET_CYCLE_TIME;
        core::Scheduler::sleepUntil(next_tick_time);
    }

    std::cout << "System Shutdown Complete." << std::endl;
    return 0;
}
