#include "fsm/state_machine.hpp"
#include <iostream>

namespace fsm {

StateMachine::StateMachine() : current_state_(SystemState::INIT) {}

void StateMachine::transitionTo(SystemState next_state) {
    std::cout << "[FSM] Transitioning to state: " << static_cast<int>(next_state) << "\n";
    current_state_ = next_state;
}

void StateMachine::updateTick() {
    // Process current state
    switch(current_state_) {
        case SystemState::INIT:
           transitionTo(SystemState::IDLE);
           break;
        case SystemState::IDLE:
           // Waiting for triggers
           break;
        case SystemState::ACTIVE_CONTROL:
           // Control motors/servos
           break;
        case SystemState::FAULT:
           // Handled externally or loops until recovery
           break;
        case SystemState::SHUTDOWN:
           break;
    }
}

SystemState StateMachine::getCurrentState() const {
    return current_state_;
}

} // namespace fsm
