#include "screen_manager.hpp"
#include "../../constants.hpp"

screen_manager::screen_manager(
    Adafruit_FT6206& touch_sense,
    Adafruit_ILI9341& scr
): 
    touch_sensor(touch_sense), 
    screen(scr), 
    active_view(nullptr),
    start_view(nullptr),
    main_menu_view(nullptr),
    settings_view(nullptr),
    experiment_view(nullptr),
    in_start_screen(false)
{
    touch_sensor.begin(constants::screen_sensitivity);
    screen.begin();
    screen.setRotation(1); // Adjust rotation as needed
    screen.fillScreen(constants::color_bg);

    setup_ui();
}

screen_manager::~screen_manager() {
    if (start_view) delete start_view;
    if (main_menu_view) delete main_menu_view;
    if (settings_view) delete settings_view;
    if (experiment_view) delete experiment_view;
}

void screen_manager::switch_to_view(ui::View* new_view) {
    if (active_view != new_view) {
        active_view = new_view;
        draw();
    }
}

void screen_manager::tick() {
    if (in_start_screen && millis() - start_screen_timer > 3000) {
        in_start_screen = false;
        switch_to_view(main_menu_view);
    }
}

void screen_manager::setup_ui() {
    // 320x240 is standard landscape ILI9341
    start_view = new ui::View(0, 0, 320, 240);
    main_menu_view = new ui::View(0, 0, 320, 240);
    settings_view = new ui::View(0, 0, 320, 240);
    experiment_view = new ui::View(0, 0, 320, 240);

    // --- 1. START SCREEN ---
    // SUH LAB in all caps
    start_view->add_child(new ui::TextView(
        80, 100, 160, 40, 
        "SUH LAB", 
        constants::color_accent, 
        4
    ));

    // --- 2. MAIN MENU SCREEN ---
    ui::Button* new_exp_btn = new ui::Button(
        60, 100, 200, 40, 
        "NEW EXPERIMENT", 
        [](void* context) {
            screen_manager* sm = static_cast<screen_manager*>(context);
            sm->kb_view->show("NewExp");
            sm->draw();
        },
        this,
        constants::color_secondary, 
        constants::color_text
    );
    main_menu_view->add_child(new_exp_btn);

    kb_view = new ui::Keyboard(
        5, 5, 310, 230,
        [](String text, void* context) {
            screen_manager* sm = static_cast<screen_manager*>(context);
            sm->update_experiment_name(text);
            sm->switch_to_view(sm->settings_view);
        },
        this
    );
    main_menu_view->add_child(kb_view); // Keyboard sits on top

    // --- 3. SETTINGS SCREEN ---
    settings_view->add_child(new ui::TextView(
        10, 10, 300, 30, 
        "Settings", 
        constants::color_primary, 
        2
    ));
    
    ui::Button* calibrate_btn = new ui::Button(
        10, 60, 140, 40,
        "CALIBRATE",
        [](void* context) {
            // Placeholder for calibration logic
            // screen_manager* sm = static_cast<screen_manager*>(context);
        },
        this,
        constants::color_secondary,
        constants::color_text
    );
    settings_view->add_child(calibrate_btn);

    ui::Button* submit_btn = new ui::Button(
        100, 180, 120, 40, 
        "SUBMIT",
        [](void* context) {
            screen_manager* sm = static_cast<screen_manager*>(context);
            sm->switch_to_view(sm->experiment_view);
        },
        this,
        constants::color_accent, 
        constants::color_text
    );
    settings_view->add_child(submit_btn);

    // --- 4. EXPERIMENT SCREEN ---
    exp_name_view = new ui::TextView(
        10, 10, 180, 25, 
        "Exp: Default", 
        constants::color_accent, 
        2
    );
    experiment_view->add_child(exp_name_view);

    ui::Button* settings_toggle_btn = new ui::Button(
        220, 5, 90, 30, 
        "SETTINGS",
        [](void* context) {
            screen_manager* sm = static_cast<screen_manager*>(context);
            sm->switch_to_view(sm->settings_view);
        },
        this,
        constants::color_secondary, 
        constants::color_text
    );
    experiment_view->add_child(settings_toggle_btn);

    // Bottle 0 Stats (Left)
    experiment_view->add_child(new ui::TextView(
        10, 50, 140, 20, 
        "Bottle 0 (Left)", 
        constants::color_primary, 
        1
    ));
    b0_bouts_view = new ui::TextView(
        10, 70, 140, 20, 
        "Bouts: 0", 
        constants::color_text, 
        1
    );
    b0_licks_view = new ui::TextView(
        10, 90, 140, 20, 
        "Licks: 0", 
        constants::color_text, 
        1
    );
    experiment_view->add_child(b0_bouts_view);
    experiment_view->add_child(b0_licks_view);

    // Bottle 1 Stats (Right)
    experiment_view->add_child(new ui::TextView(
        160, 50, 140, 20, 
        "Bottle 1 (Right)", 
        constants::color_primary, 
        1
    ));
    b1_bouts_view = new ui::TextView(
        160, 70, 140, 20, 
        "Bouts: 0", 
        constants::color_text, 
        1
    );
    b1_licks_view = new ui::TextView(
        160, 90, 140, 20, 
        "Licks: 0", 
        constants::color_text, 
        1
    );
    experiment_view->add_child(b1_bouts_view);
    experiment_view->add_child(b1_licks_view);

    // Set initial state
    active_view = start_view;
    in_start_screen = true;
    start_screen_timer = millis();
}

void screen_manager::process_touch() {
    if (touch_sensor.touched()) {
        TS_Point p = touch_sensor.getPoint();
        
        // Map touch coordinates to screen coordinates if rotation is used
        // Assuming rotation 1 (Landscape) for typical ILI9341 mapping
        int16_t mapped_x = 320 - p.y;
        int16_t mapped_y = p.x;

        if (active_view) {
            if (active_view->handle_click(mapped_x, mapped_y)) {
                draw();
                delay(150); // debounce
            }
        }
    }
}

void screen_manager::draw() {
    if (active_view) {
        screen.fillScreen(constants::color_bg);
        active_view->draw(screen);
    }
}

void screen_manager::update_experiment_name(String name) {
    if (exp_name_view) {
        exp_name_view->set_text("Exp: " + name);
        // Do not force draw here because we transition to settings page immediately after
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
    
    // Only redraw if we are currently looking at the experiment view
    if (active_view == experiment_view) {
        draw(); 
    }
}


