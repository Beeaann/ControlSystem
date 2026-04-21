#pragma once

namespace fsm {

enum class SystemState {
    INIT,
    IDLE,
    ACTIVE_CONTROL,
    FAULT,
    SHUTDOWN
};

/**
 * @brief Deterministic Finite State Machine dictating operational phases of the
 * control system. Maintains thread safety across transition modifications.
 */
class StateMachine {
public:
    StateMachine();

    /**
     * @brief Advance to a new state and fire off respective entry/exit routines.
     */
    void transitionTo(SystemState next_state);

    /**
     * @brief Real-time FSM tick processing logic governing the current state's tasks.
     */
    void updateTick();

    SystemState getCurrentState() const;

private:
    SystemState current_state_;
};

} // namespace fsm
