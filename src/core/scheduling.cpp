#include "core/scheduling.hpp"
#include <thread>
#include <pthread.h>
#include <sched.h>

namespace core {

bool Scheduler::setThreadRealtimePriority(int priority) {
    // Set SCHED_FIFO schedule policy
    sched_param sch_params;
    sch_params.sched_priority = priority;
    return pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch_params) == 0;
}

void Scheduler::sleepUntil(const std::chrono::steady_clock::time_point& time_point) {
    // Deterministic sleep
    std::this_thread::sleep_until(time_point);
}

} // namespace core
