#pragma once
#include "cap_sensor/cap_sensor.hpp"
#include "cage_data/cage_data.hpp"

using namespace std;

class cage {
    private:
        cap_sensor* sensor_left;
        cap_sensor* sensor_right;
        cage_data* cage_data;
        sd_writer* sd_writer;

    public:
        cage (
            cap_sensor* sensor_left, 
            cap_sensor* sensor_right,
            cage_data* cage_data,
            sd_writer* sd_writer,
        ) : 
            sensor_left(sensor_left), 
            sensor_right(sensor_right), 
            cage_data(cage_data),
            sd_writer(sd_writer)
        {};

        bool measure_cap();
        void export_data();
        

};
