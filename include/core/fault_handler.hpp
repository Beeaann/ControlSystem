#pragma once

#include <string>

namespace core {

enum class InitialFaultLevel {
    WARNING,
    CRITICAL,
    FATAL
};

/**
 * @brief Detects error conditions, safely manages actuator shutdown routines,
 * and handles automated recovery pathways.
 */
class FaultHandler {
public:
    FaultHandler();

    /**
     * @brief Assert a fault across the multi-threaded pipeline.
     */
    void registerFault(InitialFaultLevel level, const std::string& description);

    /**
     * @brief Safely depowers actuators and brings the system to a halt.
     */
    void safeShutdown();

    /**
     * @brief Attempt a controlled recovery sequence to return to an active FSM state.
     */
    bool attemptRecovery();

private:
    bool systemHealthy_;
};

} // namespace core
