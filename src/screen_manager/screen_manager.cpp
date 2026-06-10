#include "screen_manager.hpp"
#include "../../constants.hpp"
#include "../mpr_manager/mpr_manager.hpp"

screen_manager::screen_manager(
    Adafruit_FT6206& touch_sense,
    Adafruit_ILI9341& scr,
    mpr_manager* sensor_manager
): 
    touch_sensor(touch_sense), 
    screen(scr), 
    sensor_manager(sensor_manager),
    root_view(nullptr)
{
    touch_sensor.begin(constants::screen_sensitivity);
    screen.begin();
    screen.setRotation(1); // Adjust rotation as needed
    screen.fillScreen(constants::color_bg);

    setup_ui();
}

screen_manager::~screen_manager() {
    if (root_view) {
        delete root_view;
    }
}

void screen_manager::setup_ui() {
    // Screen size is typically 320x240 in landscape
    root_view = new ui::View(
        0, 
        0, 
        320, 
        240
    );

    // Title / Experiment Name
    exp_name_view = new ui::TextView(
        10, 
        10, 
        300, 
        25, 
        "Exp: Default", 
        constants::color_accent, 
        2
    );
    root_view->add_child(exp_name_view);

    // Bottle 0 Stats (Left)
    root_view->add_child(new ui::TextView(
        10, 
        50, 
        140, 
        20, 
        "Bottle 0 (Left)", 
        constants::color_primary, 
        1
    ));
    b0_bouts_view = new ui::TextView(
        10, 
        70, 
        140, 
        20, 
        "Bouts: 0", 
        constants::color_text, 
        1
    );
    b0_licks_view = new ui::TextView(
        10, 
        90, 
        140, 
        20, 
        "Licks: 0", 
        constants::color_text, 
        1
    );
    root_view->add_child(b0_bouts_view);
    root_view->add_child(b0_licks_view);

    // Bottle 1 Stats (Right)
    root_view->add_child(new ui::TextView(
        160, 
        50, 
        140, 
        20, 
        "Bottle 1 (Right)", 
        constants::color_primary, 
        1
    ));
    b1_bouts_view = new ui::TextView(
        160, 
        70, 
        140, 
        20, 
        "Bouts: 0", 
        constants::color_text, 
        1
    );
    b1_licks_view = new ui::TextView(
        160, 
        90, 
        140, 
        20, 
        "Licks: 0", 
        constants::color_text, 
        1
    );
    root_view->add_child(b1_bouts_view);
    root_view->add_child(b1_licks_view);

    // Keyboard view
    kb_view = new ui::Keyboard(
        5, 5, 310, 230,
        [](String text, void* context) {
            screen_manager* sm = static_cast<screen_manager*>(context);
            sm->update_experiment_name(text);
            sm->draw(); // redraw after closing keyboard
        },
        this
    );

    // Button to open keyboard
    ui::Button* name_btn = new ui::Button(
        10, 
        150, 
        100, 
        40, 
        "Set Name", 
        [](void* context) {
            screen_manager* sm = static_cast<screen_manager*>(context);
            sm->kb_view->show("NewExp");
            sm->draw(); // draw the keyboard
        },
        this,
        constants::color_secondary, 
        constants::color_text
    );
    
    root_view->add_child(name_btn);

    // Add keyboard last so it sits on top
    root_view->add_child(kb_view);
}

void screen_manager::process_touch() {
    if (touch_sensor.touched()) {
        Serial.println("Calibrating");

        sensor_manager->calibrate_sensors();

        TS_Point p = touch_sensor.getPoint();
        
        // Map touch coordinates to screen coordinates if rotation is used
        // Assuming rotation 1 (Landscape) for typical ILI9341 mapping
        int16_t mapped_x = 320 - p.y;
        int16_t mapped_y = p.x;

        if (root_view) {
            if (root_view->handle_click(mapped_x, mapped_y)) {
                // If a click was handled (e.g. key pressed), redraw
                draw();
                // small debounce
                delay(150);
            }
        }
    }
}

void screen_manager::draw() {
    if (root_view) {
        screen.fillScreen(constants::color_bg); // Clear background before redraw
        root_view->draw(screen);
    }
}

void screen_manager::update_experiment_name(String name) {
    if (exp_name_view) {
        exp_name_view->set_text("Exp: " + name);
        // We might want a mechanism to only redraw dirtied views
        // For now, a simple draw call will re-render everything
        draw(); 
    }
}

void screen_manager::update_bottle_stats(int bottle_index, int bouts, int licks) {
    if (bottle_index == 0) {
        if (b0_bouts_view) b0_bouts_view->set_text("Bouts: " + String(bouts));
        if (b0_licks_view) b0_licks_view->set_text("Licks: " + String(licks));
    } else if (bottle_index == 1) {
        if (b1_bouts_view) b1_bouts_view->set_text("Bouts: " + String(bouts));
        if (b1_licks_view) b1_licks_view->set_text("Licks: " + String(licks));
    }
    draw(); // Might cause flickering if called too frequently, consider optimized redraw
}


