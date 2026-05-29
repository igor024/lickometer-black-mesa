#pragma once

#include <TimeLib.h>  
#include <constants.hpp>
#include "sd_writer/sd_writer.hpp"
#include "sd_writer.hpp"
#include <data/bout.hpp>

bool sd_writer::begin() {
    if(!SD.begin()) {
        return false;
    }

    if(!SD.exists(constants::data_dir_name)) {
        SD.mkdir(constants::data_dir_name);
    }

    return true;
}

bool sd_writer::new_recording() {
    time_t t = now();
    SD.open(
        month(t) + '/' + 
        day(t) + '/' +
        year(t) + '_' +
        hour(t) + ':' +
        minute(t) + ':' +
        second(t)
    );
}

bool sd_writer::write_bout(bout) {


