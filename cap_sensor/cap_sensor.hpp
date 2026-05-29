#include <stdint.h>
#include "MPR121.h"

class cap_sensor {
    private:
        uint8_t pin;
        MPR121::DeviceAddress breakout_addr;
        MPR121* mpr121;

    public:
        cap_sensor(uint8_t p, MPR121::DeviceAddress b, MPR121* m): 
            pin(p),
            breakout_addr(b),
            mpr121(m)
        {};

        bool touching();
};
