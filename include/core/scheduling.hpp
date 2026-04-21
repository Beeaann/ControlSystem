#pragma once

#include <chrono>

namespace core {

/**
 * @brief Thread scheduling utilities to help assign SCHED_FIFO policies
 * and establish deterministic run-loops using std::chrono.
 */
class Scheduler {
public:
    Scheduler() = default;

    /**
     * @brief Apply SCHED_FIFO to the current running thread with strict priority.
     * @param priority Priority value for the real-time thread (e.g. 1-99 on Linux).
     * @return True if successful, false otherwise (e.g. lack of permissions).
     */
    static bool setThreadRealtimePriority(int priority);

    /**
     * @brief Sleep deterministically until a specified absolute time point.
     * Minimizes jitter compared to relative sleep paths.
     */
    static void sleepUntil(const std::chrono::steady_clock::time_point& time_point);
};

} // namespace core
