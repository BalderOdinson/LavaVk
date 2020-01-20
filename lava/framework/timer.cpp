//
// Created by dorian on 07. 12. 2019..
//

#include "timer.h"

namespace LavaVk
{
    Timer::Timer() :
            startTime{Clock::now()},
            previousTick{Clock::now()}
    {
    }

    void Timer::start()
    {
        if (!running)
        {
            running = true;
            startTime = Clock::now();
        }
    }

    void Timer::lap()
    {
        lapping = true;
        lapTime = Clock::now();
    }

    bool Timer::is_running() const
    {
        return running;
    }

    std::type_index Timer::getType() const
    {
        return typeid(Timer);
    }
}