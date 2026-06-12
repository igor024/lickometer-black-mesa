#pragma once

#include "../data/bout.hpp"
#include <SdFat.h>

class sd_writer {
private:
    SdExFat sd; 
    ExFile recording;

public:
    sd_writer() {};

    /*
     * creates a directory for writing data
    */
    void setup();

    /*
     * initializes the SD card and prepares a recording for the start time
     */
    bool new_recording(timelib_t start_time);

    /*
     * writes each lick in a bout to the current recording file
     */
    bool write_bout(bout b);
};
