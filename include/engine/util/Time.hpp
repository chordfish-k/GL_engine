#pragma once

#include <chrono>
#include <cstdint>
class Time {
public:
    static std::int64_t timeStarted;

    static std::int64_t NanoTime() {
        auto currentTime = std::chrono::system_clock::now();
        auto currentTime_ns =
            std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime);
        return currentTime_ns.time_since_epoch().count();
    }

    static float GetGameTime() {
        return (float)(NanoTime() - timeStarted) * 1e-9;
    }
};

inline std::int64_t Time::timeStarted = Time::NanoTime();