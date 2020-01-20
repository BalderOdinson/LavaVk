//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_TIMER_H
#define LAVAVK_TIMER_H

#include <chrono>
#include "lava/framework/object.h"

namespace LavaVk
{
    /// Encapsulates basic usage of chrono, providing a means to calculate float
    /// durations between time points via function calls.
    class Timer : public virtual Object
    {
    public:
        using Seconds      = std::chrono::seconds::period;
        using Milliseconds = std::chrono::milliseconds::period;
        using Microseconds = std::chrono::microseconds::period;
        using Nanoseconds  = std::chrono::nanoseconds::period;

        // Configure
        using Clock             = std::chrono::high_resolution_clock;
        using DefaultResolution = Seconds;

        Timer();

        /// Starts the timer, elapsed() now returns the duration since start().
        void start();

        /// Laps the timer, elapsed() now returns the duration since the last lap().
        void lap();

        /// Stops the timer, elapsed() now returns 0.
        /// \tparam T Resolution for time measuring.
        /// \return The total execution time between `start()` and `stop()`.
        template<typename T = DefaultResolution>
        double stop()
        {
            if (!running)
                return 0;

            running = false;
            lapping = false;
            auto duration = std::chrono::duration<double, T>(Clock::now() - startTime);
            startTime = Clock::now();
            lapTime = Clock::now();

            return duration.count();
        }

        /// Calculates the time difference between now and when the timer was started
        /// if lap() was called, then between now and when the timer was last lapped.
        /// \tparam T Resolution for time measuring.
        /// \return The duration between the two time points (default in milliseconds).
        template<typename T = DefaultResolution>
        double elapsed()
        {
            if (!running)
                return 0;

            Clock::time_point start = startTime;

            if (lapping)
                start = lapTime;

            return std::chrono::duration<double, T>(Clock::now() - start).count();
        }

        /// Calculates the time difference between now and the last time this function was called.
        /// \tparam T Resolution for time measuring.
        /// \return The duration between the two time points (default in seconds).
        template<typename T = DefaultResolution>
        double tick()
        {
            auto now = Clock::now();
            auto duration = std::chrono::duration<double, T>(now - previousTick);
            previousTick = now;
            return duration.count();
        }

        /// Check if the timer is running.
        /// \return True if the timer is running. False otherwise.
        bool is_running() const;

        std::type_index getType() const override;

    private:
        bool running{false};
        bool lapping{false};
        Clock::time_point startTime;
        Clock::time_point lapTime;
        Clock::time_point previousTick;
    };

    using UniqueTimer = std::unique_ptr<Timer>;
    using SharedTimer = std::shared_ptr<Timer>;
    using WeakTimer = std::weak_ptr<Timer>;
}

#endif //LAVAVK_TIMER_H
