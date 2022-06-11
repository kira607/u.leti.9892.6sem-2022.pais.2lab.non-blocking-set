#ifndef INC_2LAB_PAS_3_2_TIMER_H
#define INC_2LAB_PAS_3_2_TIMER_H

#include <chrono>

template<typename Clock=std::chrono::high_resolution_clock>
class Timer {
public:
    Timer();
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    int elapsed_ms();
private:
    std::chrono::time_point<Clock> _start_time;
};

template<class Clock>
Timer<Clock>::Timer() :
        _start_time(Clock::now()) {}

template<class Clock>
int Timer<Clock>::elapsed_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - _start_time).count();
}

#endif //INC_2LAB_PAS_3_2_TIMER_H
