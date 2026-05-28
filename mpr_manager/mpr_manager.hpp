#include "MPR121.h"
#include "constants.hpp"

#pragma once

class mpr_manager {
    private:
        struct breakout {
            MPR121::DeviceAddress addr;
            MPR121 dev;
        };
        
        breakout breakouts[constants::breakout_count];


    public:
        mpr_manager();

};
