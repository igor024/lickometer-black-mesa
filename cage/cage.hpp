#pragma once

#include "cap_sensor.hpp"
#include "cage_data.hpp"


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

        bool measure_cap();
        void export_data();
        

};
