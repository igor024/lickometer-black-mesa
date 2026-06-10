#pragma once
#include <Arduino.h>
#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>
#include "../mpr_manager/mpr_manager.hpp"
#include "ui/view.hpp"
#include "ui/text_view.hpp"
#include "ui/button.hpp"
#include "ui/keyboard.hpp"

class screen_manager {
    private:
        Adafruit_FT6206& touch_sensor;
        Adafruit_ILI9341& screen; 
        mpr_manager* sensor_manager;
        ui::View* root_view;

        // UI Elements that might need updating
        ui::TextView* exp_name_view;
        ui::TextView* b0_bouts_view;
        ui::TextView* b0_licks_view;
        ui::TextView* b1_bouts_view;
        ui::TextView* b1_licks_view;
        ui::Keyboard* kb_view;

        void setup_ui();

    public:
        screen_manager(Adafruit_FT6206& touch_sense, Adafruit_ILI9341& scr, mpr_manager* sensor_manager);
        ~screen_manager();

        void process_touch();
        void draw();

        // Methods to update data
        void update_experiment_name(String name);
        void update_bottle_stats(int bottle_index, int bouts, int licks);
};
