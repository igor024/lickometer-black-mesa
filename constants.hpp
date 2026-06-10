#pragma once

#include <stdint.h>
#include <MPR121.h>
#include <Adafruit_ILI9341.h>

namespace constants {
    constexpr uint8_t breakout_count = 1; // num of current sensor breakouts
    constexpr uint8_t pin_count = 12; // num of pins per breakout
    constexpr char data_dir_name[] = "data"; 
    constexpr uint8_t max_bout_size = 100; // max licks in a bout
    constexpr uint8_t max_lick_duration = 3; // in seconds; will reset bout if lick goes over
    constexpr uint8_t max_time_between_licks = 3; // in seconds; will reset bout if licks are spaced further apart
    constexpr uint8_t cage_count = 1;
    constexpr uint32_t loop_delay = 1000; // time between cycles
    constexpr uint32_t baud = 9600; // serial speed

    // MPR 121 CONFIG
    
    constexpr TwoWire* i2c_wire = &Wire;
    constexpr bool fast_mode = true;
    constexpr MPR121::ProximityMode proximity_mode =
        MPR121::PROXIMITY_MODE_DISABLED;

    constexpr uint8_t default_touch_threshold = 40;
    constexpr uint8_t default_release_threshold = 20;

    constexpr uint8_t touch_debounce = 1;
    constexpr uint8_t release_debounce = 1;

    constexpr MPR121::BaselineTracking baseline_tracking =
        MPR121::BASELINE_TRACKING_INIT_5BIT;
    constexpr uint8_t charge_discharge_current = 63;
    constexpr MPR121::ChargeDischargeTime charge_discharge_time =
        MPR121::CHARGE_DISCHARGE_TIME_HALF_US;
    constexpr MPR121::FirstFilterIterations first_filter_iterations =
        MPR121::FIRST_FILTER_ITERATIONS_34;
    constexpr MPR121::SecondFilterIterations second_filter_iterations =
        MPR121::SECOND_FILTER_ITERATIONS_10;
    constexpr MPR121::SamplePeriod sample_period = MPR121::SAMPLE_PERIOD_1MS;

    // ADAFRUIT 1941 TOUCH SHIELD CONFIG

    constexpr uint8_t screen_sensitivity = 40;
    constexpr uint8_t tft_cs_pin = 10;
    constexpr uint8_t tft_dc_pin = 9;

    // UI CONFIG
    //
    constexpr uint16_t color_bg = 0x18E3; // Dark grayish blue
    constexpr uint16_t color_primary = 0x03FF; // Light blue
    constexpr uint16_t color_secondary = 0x3A59; // Slate
    constexpr uint16_t color_text = ILI9341_WHITE;
    constexpr uint16_t color_accent = ILI9341_ORANGE;
    constexpr uint16_t color_border = 0x52AA; // Lighter slate
    constexpr uint16_t color_keyboard_bg = 0x2124; // Dark gray
}
