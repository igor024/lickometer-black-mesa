#pragma once

#include <Arduino.h>
#include <cstdint>

class MPR121 {
public:
    enum DeviceAddress : uint8_t {
        ADDRESS_5A = 0x5A,
        ADDRESS_5B = 0x5B,
        ADDRESS_5C = 0x5C,
        ADDRESS_5D = 0x5D,
    };

    enum ProximityMode : uint8_t {
        PROXIMITY_MODE_DISABLED = 0,
    };

    enum BaselineTracking : uint8_t {
        BASELINE_TRACKING_INIT_10BIT = 0,
    };

    enum ChargeDischargeTime : uint8_t {
        CHARGE_DISCHARGE_TIME_HALF_US = 0,
    };

    enum FirstFilterIterations : uint8_t {
        FIRST_FILTER_ITERATIONS_34 = 0,
    };

    enum SecondFilterIterations : uint8_t {
        SECOND_FILTER_ITERATIONS_10 = 0,
    };

    enum SamplePeriod : uint8_t {
        SAMPLE_PERIOD_1MS = 0,
    };

    void setWire(TwoWire&, bool) {}
    void addDevice(DeviceAddress) {}
    void setupDevice(DeviceAddress) {}
    void setAllDeviceChannelsThresholds(DeviceAddress, uint8_t, uint8_t) {}
    void setDebounce(DeviceAddress, uint8_t, uint8_t) {}
    void setBaselineTracking(DeviceAddress, BaselineTracking) {}
    void setChargeDischargeCurrent(DeviceAddress, uint8_t) {}
    void setChargeDischargeTime(DeviceAddress, ChargeDischargeTime) {}
    void setFirstFilterIterations(DeviceAddress, FirstFilterIterations) {}
    void setSecondFilterIterations(DeviceAddress, SecondFilterIterations) {}
    void setSamplePeriod(DeviceAddress, SamplePeriod) {}
    void startChannels(DeviceAddress, uint8_t, ProximityMode) {}
    uint16_t getTouchStatus(DeviceAddress) { return 0; }
    bool deviceChannelTouched(uint16_t touch_status, uint8_t pin) {
        return (touch_status & (1u << pin)) != 0;
    }
};
