#include <SdFat.h>
#include <TimeLib.h>  
#include "../../constants.hpp"
#include "./sd_writer.hpp"
#include "../data/bout.hpp"
#include "../data/lick.hpp"
#include "../logger.hpp"

#define SD_FAT_TYPE 2
// Store error strings in flash to save RAM.

sd_writer::sd_writer() {
    bouts_written = 0;
}


bool sd_writer::setup() {
    int attempts = 0;
    bool mounted = false;

    // Try to mount the SD card up to 5 times
    while (attempts < constants::SD_MOUNT_RETRIES && !mounted) {
        if (sd.begin(constants::CHIP_SELECT_PIN)) {
            mounted = true;
        } else {
            WARN(F("sd mount failed. retrying..."));
            attempts++;

            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
            digitalWrite(LED_BUILTIN, LOW);
            delay(500);
        }
    }

    if (!mounted) {
        WARN(F("sd_writer::setup failed to initialize"));
        return false; 
    }

    if(!sd.exists(constants::data_dir_path)) {
        if(!sd.chdir()) {
            WARN(F("sd_writer::setup failed to enter root directory"));
            return false;
        }
        
        if(!sd.mkdir(constants::data_dir_path)) {
            WARN(F("sd_writer::setup failed to create data directory"));
            return false;
        }
    }

    if(!sd.chdir(constants::data_dir_path)) {
        WARN(F("sd_writer::setup failed to enter data directory"));
        return false;
    }
    
    return true;
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

    if (recording.isOpen()) {
        recording.close();
    }

    return recording.open(filename, FILE_WRITE);
}

bool sd_writer::write_bout(bout& b) {
    if(!recording.isOpen()) {
        WARN(F("failed to write: file is not open"));
        return false;
    }

    for(size_t i = 0; i < b.count; ++i) {
        recording.print(b.licks[i].start_time); 
        recording.print(F(", ")); //F() is used to store string literal in flash memory rather than sram
    }

    recording.println();

    if (recording.getWriteError()) {
        WARN(F("failed to write bout"));
        return false;
    }

    bouts_written++;

    if(bouts_written >= constants::BOUTS_TO_SYNC) {
        recording.sync();
        bouts_written = 0;
    }

    return true;
}

bool sd_writer::end_recording() {
    if(!recording.isOpen()) {
        WARN(F("failed to end recording: file is not open"));
        return false;
    }

    recording.sync();

    return recording.close();
}


