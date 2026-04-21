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

// Example Sub-10ms Control Loop Target
constexpr auto TARGET_CYCLE_TIME = std::chrono::milliseconds(10);

int main() {
    std::cout << "Booting Vision-Enhanced Autonomous Control System..." << std::endl;

    // 1. Setup Scheduling Priorities (Requires root/matching capability sets)
    if (!core::Scheduler::setThreadRealtimePriority(95)) {
         std::cerr << "Warning: Could not set SCHED_FIFO! Running with standard scheduling..." << std::endl;
    }

    core::FaultHandler fault_handler;
    fsm::StateMachine fsm;
    actuators::PWMController servo(18); // Initialize our servo on GPIO 18
    drivers::I2CBus i2c_bus(1);         // Usually Pi's 40-pin header uses I2C bus 1
    sensors::MPU6050 mpu(i2c_bus, 0x68);
    drivers::GPIO emergency_button(17); // Our limit switch on GPIO 17
    camera::CameraLogger cam;
    
    // C++11 Lambda: When the button is pushed, safely alert the Fault Handler and instantly kill the system logic
    emergency_button.attachInterrupt([&fsm, &fault_handler, &cam]() {
        fault_handler.registerFault(core::InitialFaultLevel::FATAL, "Physical Emergency Button Triggered!");
        cam.captureEvent("Emergency_Override");
        fsm.transitionTo(fsm::SystemState::SHUTDOWN);
    });
    
    // Initialize state
    fsm.transitionTo(fsm::SystemState::INIT);
    servo.enable();
    i2c_bus.initialize();
    mpu.initialize();

    std::cout << "Entering Deterministic Core Loop..." << std::endl;

    // Outer RT-loop
    while (fsm.getCurrentState() != fsm::SystemState::SHUTDOWN) {
        auto step_start = std::chrono::steady_clock::now();

        fsm.updateTick();

        // 1. Force transition out of IDLE so the system actually "does something"
        if (fsm.getCurrentState() == fsm::SystemState::IDLE) {
            fsm.transitionTo(fsm::SystemState::ACTIVE_CONTROL);
        }

        // 2. Map Sensor logic directly to physical actuators
        if (fsm.getCurrentState() == fsm::SystemState::ACTIVE_CONTROL) {
            
            // Poll I2C sensor rapidly to get fresh physics data 
            mpu.readRawData();

            // Feed the live Pitch angle directly into the PWM driver!
            // When you physically tilt the board, the servo will mirror the tilt instantly.
            servo.setAngle(mpu.getPitch());
        }

        // Check if we need to sleep for jitter removal
        auto next_tick_time = step_start + TARGET_CYCLE_TIME;
        core::Scheduler::sleepUntil(next_tick_time);
    }

    std::cout << "System Shutdown Complete." << std::endl;
    return 0;
}
