#pragma once

#include "../../constants.hpp"
#include <stdint.h>
#include "lick.hpp"

struct bout {
    lick licks[constants::max_bout_size];
    uint32_t count = 0; // Tracks how many licks are actually valid in this array
};
