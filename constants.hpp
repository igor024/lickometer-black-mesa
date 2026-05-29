#include <stdint.h>

#pragma once

namespace constants {
    constexpr uint8_t breakout_count = 3;
    constexpr uint8_t pin_count = 12;
    constexpr int max_bout_size = 100; //max licks in a bout
    constexpr int max_lick_duration = 3; //in seconds; will reset bout if lick goes over
    constexpr int max_time_between_licks = 3; //in seconds; will reset bout if licks are spaced further apart
}
