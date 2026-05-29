#include "cap_sensor.hpp"

bool cap_sensor::touching() {
    return mpr121->deviceChannelTouched(pin, breakout_addr);
}
