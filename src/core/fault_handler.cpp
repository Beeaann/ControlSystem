#include "core/fault_handler.hpp"
#include <iostream>

namespace core {

FaultHandler::FaultHandler() : systemHealthy_(true) {}

void FaultHandler::registerFault(InitialFaultLevel level, const std::string& description) {
    std::cerr << "[FAULT DETECTED] Level " << static_cast<int>(level) << ": " << description << "\n";
    if (level == InitialFaultLevel::CRITICAL || level == InitialFaultLevel::FATAL) {
        systemHealthy_ = false;
        safeShutdown();
    }
}

void FaultHandler::safeShutdown() {
    std::cout << "Safely shutting down all hardware components...\n";
    // Disengage PWM interfaces here
}

bool FaultHandler::attemptRecovery() {
    std::cout << "Attempting recovery mechanism...\n";
    systemHealthy_ = true;
    return true;
}

} // namespace core
