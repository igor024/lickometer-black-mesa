#pragma once


#include <SdFat.h>
#include <TimeLib.h>  
#include <cstdio>
#include "../../constants.hpp"
#include "./sd_writer.hpp"
#include "../data/bout.hpp"
#include "../data/lick.hpp"
#include "cmake/arduino_stubs/Arduino.h"

#define sd_FAT_TYPE 2
// Store error strings in flash to save RAM.
#define error(s) sd.errorHalt(&Serial, F(s))

void sd_writer::setup() {
    if(!sd.begin())
        error("sd_writer::setup failed to initialize");

    if(!sd.exists(constants::data_dir_path)) {
        if(!sd.chdir()) 
            error("sd_writer::setup failed to enter root directory");
        
        if(!sd.mkdir(constants::data_dir_path)) 
            error("sd_writer::setup failed to create data directory");
    }

    if(!sd.chdir(constants::data_dir_path))
        error("sd_writer::setup failed to enter data directory");
}

bool sd_writer::new_recording(timelib_t start_time) {
    char timestamp[20];
    char filename[28]; //leaves space for a 3 digit duplicate file count

    //convert time to readable format
    struct timelib_tm tinfo;
    timelib_break(start_time, &tinfo);

    //create file name
    snprintf(
        timestamp, 
        sizeof(timestamp), 
        "%02d-%02d-%04d_%02d-%02d-%02d", 
        tinfo.tm_mon,
        tinfo.tm_mday,
        tinfo.tm_year + 2000, //year is offset by 2000
        tinfo.tm_hour,
        tinfo.tm_min,
        tinfo.tm_sec
    );

    snprintf(filename, sizeof(filename), "%s.txt", timestamp);

    //append a number to avoid duplicate files
    int dup_count = 1;
    while(sd.exists(filename)) {
        snprintf(filename, sizeof(filename), "%s-%d.txt", timestamp, dup_count);
        dup_count++;
    }

    return recording.open(filename, FILE_WRITE);
}

bool sd_writer::write_bout(bout b) {
    if(!recording.isOpen()) {
        Serial.println("failed to write: file is not open");
        return false;
    }

    for(size_t i = 0; i < b.count; ++i) {
        recording.print(b.licks[i].start_time); 
        recording.print(F(", ")); //F() is used to store string literal in flash memory rather than sram
    }

    recording.println();

    if (recording.getWriteError()) {
        Serial.println(F("failed to write bout"));
        return false;
    }

    return true;
}

