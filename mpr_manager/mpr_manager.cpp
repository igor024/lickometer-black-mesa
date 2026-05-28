#include "mpr_manager.hpp" 
#include "constants.hpp"

mpr_manager::mpr_manager(
    MPR121& mpr,
    MPR121::DeviceAddress addresses[]
): mpr121(mpr) {
    for (int addr = 0; addr < constants::breakout_count; ++addr) {
        breakouts[addr] = new mpr_manager::breakout{addresses[addr]};

        for (int sen = 0; sen < constants::channel_count; ++sen) {
            breakouts[addr]->sensors[sen] = new cap_sensor(
                sen,
                addresses[addr]
            );
        }

        mpr121.addDevice(addresses[addr]);
        mpr121.setupDevice(addresses[addr]);
    } 
}
