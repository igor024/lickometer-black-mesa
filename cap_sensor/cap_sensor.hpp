#include <cstdint>
#include "MPR121.h"

class cap_sensor {
    private:
        uint8_t pin;
        MPR121& breakout;

        cap_sensor(uint8_t p, MPR121& b): pin(p), breakout(b) {};

    public:
        bool touching();

};
