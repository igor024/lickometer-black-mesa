#include <stdint.h>
#include "MPR121.h"

class cap_sensor {
    private:
        uint8_t pin;
        MPR121* breakout;

    public:
        cap_sensor(uint8_t p, MPR121* b): pin(p), breakout(b) {};

        bool touching();


};
