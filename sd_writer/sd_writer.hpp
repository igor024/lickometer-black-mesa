#pragma once

#include <SD>
#include <data/bout.hpp>

class sd_writer {
    public:
        bool begin();
        bool write_bout(bout b);
};
