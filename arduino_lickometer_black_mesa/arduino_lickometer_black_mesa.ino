#include "src/cage/cage.hpp"
#include "src/cage_data/cage_data.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "src/mpr_manager/mpr_manager.hpp"

MPR121 mpr121;

MPR121::DeviceAddress sensor_boards[] = {
    MPR121::ADDRESS_5A,
};

mpr_manager* sensor_manager;
cage* cages[constants::cage_count];


void setup() {
    Serial.begin(constants::baud);
    Serial.println(F("testinggg"));

    sensor_manager = new mpr_manager(mpr121, sensor_boards);
    Serial.println(F("testinggg complete"));

    cap_sensor* s0 = sensor_manager->get_sensor(0, 0);
    cap_sensor* s1 = sensor_manager->get_sensor(0, 1); 
    sd_writer* writer = new sd_writer();
    cage_data* c_data = new cage_data(writer); 
    
    cages[0] = new cage(s0, s1, c_data, writer);
} 

void loop() {
    WARN(sensor_manager->is_pin_pressed(0, 0));
    delay(constants::loop_delay);
}
