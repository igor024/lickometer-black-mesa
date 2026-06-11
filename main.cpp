#include "constants.hpp"
#include "logger.hpp"
#include "src/cage/cage.hpp"
#include "src/cage_data/cage_data.hpp"
#include "src/mpr_manager/mpr_manager.hpp"
#include "src/screen_manager/screen_manager.hpp"
#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

MPR121 mpr121;

Adafruit_FT6206 screen_cap_touch = Adafruit_FT6206();
Adafruit_ILI9341 screen =
    Adafruit_ILI9341(constants::tft_cs_pin, constants::tft_dc_pin);

screen_manager *scr_manager;

MPR121::DeviceAddress sensor_boards[] = {
    MPR121::ADDRESS_5A,
};

mpr_manager *sensor_manager;

cage *cages[constants::cage_count];

void setup() {
    Serial.begin(constants::baud);
    while (!Serial) { delay(10); } // make sure serial is running

    sensor_manager = new mpr_manager(mpr121, sensor_boards);

    scr_manager = new screen_manager(screen_cap_touch, screen, sensor_manager);

    cap_sensor *s0 = sensor_manager->get_sensor(0, 0);
    cap_sensor *s1 = sensor_manager->get_sensor(0, 1);
    sd_writer *writer = new sd_writer();
    cage_data *c_data = new cage_data(writer);

    cages[0] = new cage(s0, s1, c_data, writer);

    scr_manager->draw();

    sensor_manager->calibrate_sensors();
}

void loop() {
    // mpr121.startAllChannels();

    // delay(100);

    scr_manager->process_touch();

    WARN(sensor_manager->is_pin_pressed(0, 0));

    WARN(mpr121.getDeviceChannelBaselineData(sensor_boards[0], 0));
    WARN(mpr121.getDeviceChannelFilteredData(sensor_boards[0], 0));

    // mpr121.stopAllChannels();

    delay(constants::loop_delay);

    Serial.println("stopped");
}
