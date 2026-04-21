#include <iostream>
#include <thread>
#include <chrono>

#include "core/scheduling.hpp"
#include "core/fault_handler.hpp"
#include "fsm/state_machine.hpp"
// #include "sensors/mpu6050.hpp"

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
    
    // Initialize state
    fsm.transitionTo(fsm::SystemState::INIT);

    std::cout << "Entering Deterministic Core Loop..." << std::endl;

    // Outer RT-loop
    while (fsm.getCurrentState() != fsm::SystemState::SHUTDOWN) {
        auto step_start = std::chrono::steady_clock::now();

        // -----------------------
        // Core Routine execution:
        // 1. Gather Interrupt/I2C Sensor Data
        // 2. Perform FSM Tick Logic + Controller evaluation
        // 3. Dispatch new actuator signals via PWM
        // 4. Capture Vision/Camera logs if triggered
        // -----------------------
        fsm.updateTick();

        // Check if we need to sleep for jitter removal
        auto next_tick_time = step_start + TARGET_CYCLE_TIME;
        core::Scheduler::sleepUntil(next_tick_time);
    }

    std::cout << "System Shutdown Complete." << std::endl;
    return 0;
}
