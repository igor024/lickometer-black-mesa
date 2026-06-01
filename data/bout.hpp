#pragma once

#include <constants.hpp>
#include <data/lick.hpp>

struct bout {
    lick licks[constants::max_bout_size];
    size_t count = 0; // Tracks how many licks are actually valid in this array
};
