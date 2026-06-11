#pragma once

#include <MPR121.h>
#include "../cap_sensor/cap_sensor.hpp"
#include "../../constants.hpp"

class mpr_manager {
    private:
        struct breakout {
            MPR121::DeviceAddress addr;
            cap_sensor* sensors[constants::pin_count];

            /*
             * frees the sensors owned by this breakout board
             */
            ~breakout();
        };

        breakout* breakouts[constants::breakout_count];
    public:
        MPR121& mpr121; // make this private after debug
        
        mpr_manager(MPR121& mpr, MPR121::DeviceAddress addresses[]);

        /*
         * frees all breakout boards and their sensors, probably unnecessary
         */
        ~mpr_manager();

        /*
         * returns whether the requested breakout pin is 
         * currently being touched
         */
        bool is_pin_pressed(uint8_t breakout_idx, uint8_t pin_idx);

        /*
         * returns the sensor object for the requested breakout pin
         */
        cap_sensor* get_sensor(uint8_t breakout_idx, uint8_t pin_idx);

        /*
         * calibrates all the sensors that belong to the mpr_manager
         */
        void calibrate_sensors();
};
