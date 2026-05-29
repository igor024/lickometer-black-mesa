#include "mpr_manager.hpp"

MPR121 mpr121;
MPR121::DeviceAddress sensor_boards[] = {
    MPR121::ADDRESS_5A,
    MPR121::ADDRESS_5B,
    MPR121::ADDRESS_5C
};

mpr_manager* sensor_manager;

void setup() {
    sensor_manager = new mpr_manager(mpr121, sensor_boards);
}

void loop() {

}
