#pragma once

#include <SD.h>
#include <data/bout.hpp>

class sd_writer {
    private:
        File recording;
    public:
        sd_writer();
        bool begin(time_t start_time);
        bool write_bout(bout b);
};

