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
        
        ui::View* active_view;

        ui::View* start_view;
        ui::View* main_menu_view;
        ui::View* settings_view;
        ui::View* experiment_view;

        // UI Elements that might need updating
        ui::TextView* exp_name_view;
        ui::TextView* b0_bouts_view;
        ui::TextView* b0_licks_view;
        ui::TextView* b1_bouts_view;
        ui::TextView* b1_licks_view;
        ui::Keyboard* kb_view;

        uint32_t start_screen_timer;
        bool in_start_screen;

        void setup_ui();

    public:
        screen_manager(Adafruit_FT6206& touch_sense, Adafruit_ILI9341& scr);
        ~screen_manager();

        void process_touch();
        void draw();
        void tick(); // To handle timers like start screen timeout
        void switch_to_view(ui::View* new_view);

        // Methods to update data
        void update_experiment_name(String name);
        void update_bottle_stats(int bottle_index, int bouts, int licks);
};
