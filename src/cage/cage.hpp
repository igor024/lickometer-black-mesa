#pragma once

#include "../cap_sensor/cap_sensor.hpp"
#include "../cage_data/cage_data.hpp"


class cage {
    private:
        cap_sensor* sensor_left;
        cap_sensor* sensor_right;
        cage_data* data;
        sd_writer* writer;

    public:
        cage(
            cap_sensor* sensor_left, 
            cap_sensor* sensor_right,
            cage_data* data,
            sd_writer* writer
        ): 
            sensor_left(sensor_left), 
            sensor_right(sensor_right), 
            data(data),
            writer(writer)
        {};

        /*
         * measures the cage sensors and records any contact changes
         */
        bool measure_cap();

        /*
         * writes the cage's remaining recorded data to storage
         */
        void export_data();
        

};
