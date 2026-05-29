#pragma once
#include <cstdint>
#include "cap_sensor/cap_sensor.hpp"
#include "cage_data/cage_data.hpp"

using namespace std;

class cage {
    private:
        cap_sensor* sensor_left;
        cap_sensor* sensor_right;
        cage_data* cage_data;

    public:
        cage (
            cap_sensor* sensor_left, 
            cap_sensor* sensor_right,
            cage_data* cage_data
        ) : 
            sensor_left(sensor_left), 
            sensor_right(sensor_right), 
            cage_data(cage_data)
        {};

        bool measure_cap();
        void getData();

}
