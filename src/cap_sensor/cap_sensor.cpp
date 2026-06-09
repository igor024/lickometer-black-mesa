#include "cap_sensor.hpp"

bool cap_sensor::touching() {
    uint16_t touch_status = mpr121->getTouchStatus(breakout_addr);
    return mpr121->deviceChannelTouched(touch_status, pin);
}
