#include "MPR121.h"
#include "cap_sensor.hpp"
#include "constants.hpp"

#pragma once

class mpr_manager {
    private:
        MPR121& mpr121;

        struct breakout {
            MPR121::DeviceAddress addr;
            cap_sensor* sensors[constants::channel_count];
        };

        breakout* breakouts[constants::breakout_count];
    public:
        mpr_manager(MPR121& mpr, MPR121::DeviceAddress addresses[]);

        // add a free call
};
