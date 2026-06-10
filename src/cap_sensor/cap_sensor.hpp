#pragma once

#include "../../constants.hpp"
#include <stdint.h>
#include <MPR121.h>

class cap_sensor {
    private:
        uint8_t pin;
        MPR121::DeviceAddress breakout_addr;
        MPR121* mpr121;
        
        uint8_t touch_threshold{constants::default_touch_threshold};
        uint8_t release_threshold{constants::default_release_threshold};

    public:
        cap_sensor(uint8_t p, MPR121::DeviceAddress b, MPR121* m): 
            pin(p),
            breakout_addr(b),
            mpr121(m)
        {};

        /*
         * returns whether this capacitive sensor is currently being touched
         */
        bool touching();

        /*
         * sets the mpr121 touch threshold for this pin
         */
        void set_thresholds();

        /*
         * calibrates the current capacitive sensor
         */
        void calibrate();

};
