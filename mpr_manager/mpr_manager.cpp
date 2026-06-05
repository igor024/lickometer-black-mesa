#include "mpr_manager.hpp" 
#include "cap_sensor.hpp"
#include "constants.hpp"

mpr_manager::mpr_manager(
    MPR121& mpr,
    MPR121::DeviceAddress addresses[]
): mpr121(mpr) {
    for (int addr = 0; addr < constants::breakout_count; ++addr) {
        breakouts[addr] = new mpr_manager::breakout{addresses[addr]};

        for (int sen = 0; sen < constants::pin_count; ++sen) {
            breakouts[addr]->sensors[sen] = new cap_sensor(
                sen,
                addresses[addr],
                &mpr121
            );
        }

        mpr121.addDevice(addresses[addr]);
        mpr121.setupDevice(addresses[addr]);
    } 
}

mpr_manager::~mpr_manager() {
    for (int addr = 0; addr < constants::breakout_count; ++addr) {
        delete breakouts[addr];
    }
}

mpr_manager::breakout::~breakout() {
    for (int sen = 0; sen < constants::pin_count; ++sen) {
        delete sensors[sen];
    }
}

bool mpr_manager::is_pin_pressed(uint8_t breakout_idx, uint8_t pin_idx) {
    breakout_idx = min(breakout_idx, constants::breakout_count - 1);
    pin_idx = min(pin_idx, constants::pin_count - 1);

    return breakouts[breakout_idx]->sensors[pin_idx]->touching();
}

cap_sensor* mpr_manager::get_sensor(uint8_t breakout_idx, uint8_t pin_idx) {
    breakout_idx = min(breakout_idx, constants::breakout_count - 1);
    pin_idx = min(pin_idx, constants::pin_count - 1);

    return breakouts[breakout_idx]->sensors[pin_idx];
}
