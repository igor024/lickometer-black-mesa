#pragma once

#include "../data/bout.hpp"
#include <SdFat.h>
#include <TimeLib.h>  

class sd_writer {
private:
    SdExFat sd; 
    ExFile recording;
    uint8_t bouts_written;

public:
    sd_writer(); 

    /*
     * creates a directory for writing data
    */
    bool setup();

    /*
     * initializes the SD card and creates a new recording for the start time
     */
    bool new_recording(timelib_t start_time);

    /*
     * flushes buffer and closes recording
     */
    bool end_recording();


    /*
     * writes each lick in a bout to the current recording file
     */
    bool write_bout(bout& b);
};
