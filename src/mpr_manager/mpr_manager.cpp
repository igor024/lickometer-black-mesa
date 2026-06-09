#include "mpr_manager.hpp" 
#include "../../logger.hpp"
#include "../cap_sensor/cap_sensor.hpp"
#include "../../constants.hpp"

mpr_manager::mpr_manager(
    MPR121& mpr,
    MPR121::DeviceAddress addresses[]
): mpr121(mpr) {
    mpr121.setWire(*constants::i2c_wire, constants::fast_mode);

    for (int addr = 0; addr < constants::breakout_count; ++addr) {
        Serial.print("SETUP BEGAN FOR BREAKOUT ");
        Serial.println(addr);

        breakouts[addr] =
            new mpr_manager::breakout{addresses[addr], {nullptr}};

        for (int sen = 0; sen < constants::pin_count; ++sen) {
            breakouts[addr]->sensors[sen] = new cap_sensor(
                sen,
                addresses[addr],
                &mpr121
            );
        }

        mpr121.addDevice(addresses[addr]);
        mpr121.setupDevice(addresses[addr]);

        mpr121.setAllDeviceChannelsThresholds(
            addresses[addr],
            constants::default_touch_threshold,
            constants::default_release_threshold
        );

        mpr121.setDebounce(
            addresses[addr],
            constants::touch_debounce,
            constants::release_debounce
        );

        mpr121.setBaselineTracking(
            addresses[addr],
            constants::baseline_tracking
        );

        mpr121.setChargeDischargeCurrent(
            addresses[addr],
            constants::charge_discharge_current
        );

        mpr121.setChargeDischargeTime(
            addresses[addr],
            constants::charge_discharge_time
        );

        mpr121.setFirstFilterIterations(
            addresses[addr],
            constants::first_filter_iterations
        );
        
        mpr121.setSecondFilterIterations(
            addresses[addr],
            constants::second_filter_iterations
        );

        mpr121.setSamplePeriod(addresses[addr], constants::sample_period);

        mpr121.startChannels(
            addresses[addr],
            constants::pin_count,
            constants::proximity_mode
        );

        Serial.print("SETUP COMPLETE FOR BREAKOUT ");
        Serial.println(addr);
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
    if (breakout_idx > constants::breakout_count - 1) {
        WARN("breakout idx exceeds number of breakout boards");
    }

    if (pin_idx > constants::pin_count - 1) {
        WARN("pin idx exceeds number of pins on the breakout");
    }

    breakout_idx = min(breakout_idx, constants::breakout_count - 1);
    pin_idx = min(pin_idx, constants::pin_count - 1);

    return breakouts[breakout_idx]->sensors[pin_idx]->touching();
}

cap_sensor* mpr_manager::get_sensor(uint8_t breakout_idx, uint8_t pin_idx) {
    if (breakout_idx > constants::breakout_count - 1) {
        WARN("breakout idx exceeds number of breakout boards");
    }

    if (pin_idx > constants::pin_count - 1) {
        WARN("pin idx exceeds number of pins on the breakout");
    }
    
    breakout_idx = min(breakout_idx, constants::breakout_count - 1);
    pin_idx = min(pin_idx, constants::pin_count - 1);

    return breakouts[breakout_idx]->sensors[pin_idx];
}
