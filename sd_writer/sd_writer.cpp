#pragma once

#include <SdFat.h>
#include <TimeLib.h>  
#include <constants.hpp>
#include "sd_writer/sd_writer.hpp"
#include "sd_writer.hpp"
#include <data/bout.hpp>
#include <data/lick.hpp>

SdFat SD; // Replaces the standard SD object

void setup() {
    // Starts the SD card just like normal
    if (!SD.begin(CS_PIN)) {
        // error handling
    }
}

bool sd_writer::begin() {
    if(!SD.begin()) {
        return false;
    }

    if(!SD.exists(constants::data_dir_name)) {
        SD.mkdir(constants::data_dir_name);
    }

    return true;
}

bool sd_writer::new_recording(time_t start_tme) {
    char filename[32];

    snprintf(filename, sizeof(filename), "%02d-%02d-%04d_%02d-%02d-%02d.txt", 
            month(start_time), 
            day(start_time), 
            year(start_time), 
            hour(start_time), 
            minute(start_time), 
            second(start_time));

    this->recording = SD.open(filename, FILE_WRITE);
    return this->recording != false;
}

bool sd_writer::write_bout(bout) {
    for(size_t i = 0; i < b.count; ++i) {
        this->recording.print(b.licks[i].start_time); 
        this->recording.print(", ");
    }
}

