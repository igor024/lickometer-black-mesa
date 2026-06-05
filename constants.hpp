#pragma once

#include <stdint.h>

namespace constants {
    constexpr uint8_t breakout_count = 3; //num of current sensor breakouts
    constexpr uint8_t pin_count = 12; //num of pins per breakout
    constexpr char data_dir_name[] = "data"; 
    constexpr uint8_t max_bout_size = 100; //max licks in a bout
    constexpr uint8_t max_lick_duration = 3; //in seconds; will reset bout if lick goes over
    constexpr uint8_t max_time_between_licks = 3; //in seconds; will reset bout if licks are spaced further apart
    constexpr uint8_t cage_count = 1;
}
