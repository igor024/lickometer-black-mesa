#include <Arduino.h>
#include "cap_sensor.hpp"
#include "../../constants.hpp"

#define T_INCREMENT 20

bool cap_sensor::touching() {
    uint16_t touch_status = mpr121->getTouchStatus(breakout_addr);
    return mpr121->deviceChannelTouched(touch_status, pin);
}

void cap_sensor::set_thresholds() {
    mpr121->setChannelThresholds(pin, touch_threshold, release_threshold);
}

void cap_sensor::calibrate() {
    touch_threshold = constants::default_touch_threshold;
    release_threshold = constants::default_release_threshold;

    set_thresholds();

    delay(10);

    if (!touching()) {
        return;
    }

    while (touching() && touch_threshold < (255 - T_INCREMENT)) {
        Serial.print(touch_threshold); Serial.println(" is too low!");

        touch_threshold += T_INCREMENT;  
        release_threshold += T_INCREMENT;  
        set_thresholds();

        delay(10);
    }

    uint8_t inc = min(255-touch_threshold, T_INCREMENT/2);

    touch_threshold += inc;
    release_threshold += inc;

    set_thresholds();

    Serial.print("New touch threshold: "); Serial.println(touch_threshold);
    Serial.print("for sensor: "); Serial.println(pin);
}
