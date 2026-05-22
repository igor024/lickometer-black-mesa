#pragma once
#include "Wire.h"

class MPR121 {
    public:
        void setWire(Wire wire, bool mode);

        enum DeviceAddress {ADDRESS_5A};
};
