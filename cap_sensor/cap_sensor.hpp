#include <stdint.h>
#include "MPR121.h"

class cap_sensor {
    private:
        uint8_t pin;
        MPR121::DeviceAddress breakout_addr;

    public:
        cap_sensor(uint8_t p, MPR121::DeviceAddress b): 
            pin(p),
            breakout_addr(b) 
        {};

        bool touching();
};
