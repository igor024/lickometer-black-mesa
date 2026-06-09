#pragma once

#include <SdFat.h>
#include "../data/bout.hpp"

class sd_writer {
    private:
        File recording;
    public:
        sd_writer() {};

        /*
         * initializes the SD card and prepares a recording for the start time
         */
        bool begin(time_t start_time);

        /*
         * writes each lick in a bout to the current recording file
         */
        bool write_bout(bout b);
};
