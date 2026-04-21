# Real-Time Vision-Enhanced Autonomous Control System

## Overview
A high-performance, real-time autonomous control system built for the Raspberry Pi 5. This C++ project implements a deterministic multithreaded architecture on Embedded Linux, combining sensor fusion, machine vision, and low-level hardware control.

## System Features
*   **Real-Time Sub-10ms Latency:** Engineered with `SCHED_FIFO` threads and `std::chrono` scheduling to minimize scheduling jitter and achieve deterministic performance.
*   **Sensor Fusion (MPU6050):** Uses a complementary filter to incorporate accelerometer and gyroscope telemetry for stable, real-world state estimation.
*   **Deterministic FSM:** Finite State Machine governing operational phases, seamlessly adapting to sensor triggers.
*   **Low-Level Hardware Drivers:** Interacts directly via I2C and GPIO for interrupt-driven inputs.
*   **Actuator Control:** PWM-based servo and DC motor control interface.
*   **Event-Triggered Vision Logging:** Incorporates the Raspberry Pi Camera module for capturing timestamped images automatically acting upon critical events.
*   **Robust Fault Handling:** Assures safe actuator shutdown upon fault conditions, auto-recovery execution paths, and error detection.

## Directory Structure
```text
.
├── src/                # Implementation files (.cpp)
├── include/            # Header files (.hpp)
│   ├── fsm/            # State machine state structures and transitions
│   ├── sensors/        # Sensor implementations like the MPU6050
│   ├── actuators/      # Motor and servo behavior interfaces
│   ├── camera/         # Camera module interaction classes
│   ├── core/           # Master controllers, threading, fault management
│   └── drivers/        # Fundamental I2C/GPIO interfaces for hardware access
├── tests/              # Test suites for validation (fault injection, FSM tests)
└── CMakeLists.txt      # Build configuration file
```

## Getting Started

### Prerequisites
*   Raspberry Pi 5 with an Embedded Linux environment.
*   GCC/Clang compiler with C++17 support or higher.
*   CMake (>= 3.10)
*   MPU6050 Sensor, PWM-controlled Servo & DC Motor, Raspberry Pi Camera Module.

### Building
```bash
mkdir build
cd build
cmake ..
make
```

### Validation & Testing
System behavior validation includes structured state transition tests and explicit edge-case fault injections. To run the tests after building:
```bash
./Tests_FSM_Faults
```
