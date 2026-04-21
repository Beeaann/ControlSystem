# Autonomous Control System - Design Document

## System Architecture Objective
This codebase implements a deterministic, real-time control loop for the Raspberry Pi 5. Its primary goal is to safely manage physical hardware (motors and servos) based on extremely fast sensor polling, using a loop constraint (sub-10ms) that won't stutter. It also handles edge cases and captures photographic evidence via the Pi Camera if things go wrong.

---

## File & Module Breakdown

### 1. Core Runtime
* **`src/main.cpp`**: The primary entry point. It kicks off the application, acquires real-time OS privileges, instantiates the master state machine, and runs the "infinite" control loop using strict timing.
* **`src/core/scheduling.cpp`**: Wraps low-level POSIX thread APIs (`pthread_setschedparam`). It tells the Linux kernel "this program is more important than background tasks" to ensure hardware signals aren't missed.
* **`src/core/fault_handler.cpp`**: A centralized watchdog. If a sensor gets unplugged or physics calculations go haywire, this intercepts the error, forces a "safe shutdown" of all motors to prevent physical damage, and attempts to restart.

### 2. Logic Controller
* **`src/fsm/state_machine.cpp`**: The "brain" of the control loop. It is a Finite State Machine (FSM) that tracks what the robot is currently doing (`IDLE`, `ACTIVE_CONTROL`, or `FAULT`). All decisions on how actuators react flow through here depending on the current phase.

### 3. Sensors & Vision
* **`src/sensors/mpu6050.cpp`**: The physical state estimator. It takes raw, noisy I2C byte data from the motion sensor and applies a "Complementary Filter" (a math equation combining the accelerometer and gyroscope) to spit out clean, smooth Pitch and Roll angles.
* **`src/camera/camera_logger.cpp`**: An asynchronous vision wrapper. When the `fault_handler` or FSM detects an anomaly, it tells this class to fire the Pi Camera instantly to snag a timestamped picture for you to analyze later.

### 4. Actuators (Outputs)
* **`src/actuators/pwm_controller.cpp`**: A simplified layer over Pulse Width Modulation (PWM). Instead of fighting with raw electrical signal timings, this file gives you easy helper functions like `setAngle(90)` for servos or `setDutyCycle(50)` for motors.

### 5. Low-Level Drivers
* **`src/drivers/i2c_bus.cpp`**: The exact Linux interface (`/dev/i2c-x`) to talk directly over wire. It allows you to rapidly read & write register memory addresses to connected microchips.
* **`src/drivers/gpio_interrupts.cpp`**: Hardware triggers. Instead of the code constantly looping asking "Is the button pressed yet?", this ties a hardware pin physically to a function, interrupting the Pi the absolute millisecond a switch flips.

### 6. Validation
* **`tests/test_fsm.cpp`**: An automated, standalone validator. You run this without motors plugged in to artificially inject software crashes and guarantee the `fault_handler` properly traps safely before deploying code to the physical bot.
