#include "MPR121.h"

#pragma once

class mpr_manager {
    private:
        struct breakout_pair {
            MPR121::DeviceAddress addr;
            MPR121 dev;
        };

        sensor_pair breakouts[];

        mpr_manager(int s_num, )
};
