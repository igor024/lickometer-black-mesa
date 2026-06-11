#pragma once

#include "view.hpp"
#include "button.hpp"
#include "text_view.hpp"
#include "../../../constants.hpp"
#include <Arduino.h>

namespace ui {

// TODO: Fix formatting

class Keyboard : public View {
private:
    String input_text;
    TextView* display_view;
    void (*on_submit_callback)(String, void*);
    void* callback_context;
    
    // Internal callback for key presses
    static void key_pressed(void* context) {
        // We need a way to know which key was pressed.
        // We'll pass a struct or maybe just encode the char in the context if we could, 
        // but `context` is void*. 
        // A simple trick: pass the Button* as context, but we need the Keyboard* too.
        // Let's create a small struct for context, or since we don't have std::bind,
        // we can store the char in the button itself? No, Button doesn't store a char.
        // Let's just use the button's label in a global/static way? No.
    }

    struct KeyContext {
        Keyboard* kb;
        char key;
    };

    KeyContext key_contexts[30];
    uint8_t key_ctx_count = 0;

    static void handle_key_press(void* ctx) {
        KeyContext* kctx = static_cast<KeyContext*>(ctx);
        kctx->kb->add_char(kctx->key);
    }

    static void handle_backspace(void* ctx) {
        Keyboard* kb = static_cast<Keyboard*>(ctx);
        kb->remove_char();
    }

    static void handle_submit(void* ctx) {
        Keyboard* kb = static_cast<Keyboard*>(ctx);
        kb->submit();
    }

    static void handle_cancel(void* ctx) {
        Keyboard* kb = static_cast<Keyboard*>(ctx);
        kb->cancel();
    }

    void add_char(char c) {
        if (input_text.length() < 15) {
            input_text += c;
            display_view->set_text(input_text);
        }
    }

    void remove_char() {
        if (input_text.length() > 0) {
            input_text.remove(input_text.length() - 1);
            display_view->set_text(input_text);
        }
    }

    void submit() {
        if (on_submit_callback) {
            on_submit_callback(input_text, callback_context);
        }
        set_visible(false);
    }

    void cancel() {
        set_visible(false);
    }

public:
    Keyboard(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        void (*on_submit)(String, void*),
        void* context
    ) : View(x, y, w, h),
        input_text(""),
        on_submit_callback(on_submit),
        callback_context(context)
    {
        is_visible = false; // Hidden by default

        // Background
        // We will just let draw() handle the background

        display_view = new TextView(x + 10, y + 10, w - 20, 30, "", constants::color_text, 2);
        add_child(display_view);

        const char* row1 = "QWERTYUIOP";
        const char* row2 = "ASDFGHJKL";
        const char* row3 = "ZXCVBNM";

        int16_t start_y = y + 50;
        int16_t key_w = 28;
        int16_t key_h = 35;
        int16_t spacing = 3;

        // Row 1
        int16_t current_x = x + 5;
        for (int i = 0; i < 10; ++i) {
            key_contexts[key_ctx_count] = {this, row1[i]};
            add_child(new Button(
                current_x, start_y, key_w, key_h, String(row1[i]), 
                handle_key_press, &key_contexts[key_ctx_count]
            ));
            current_x += key_w + spacing;
            key_ctx_count++;
        }

        // Row 2
        current_x = x + 15;
        start_y += key_h + spacing;

        for (int i = 0; i < 9; ++i) {
            key_contexts[key_ctx_count] = {this, row2[i]};

            add_child(new Button(
                current_x, start_y, key_w, key_h, String(row2[i]), 
                handle_key_press, &key_contexts[key_ctx_count],
                constants::color_primary,
                constants::color_text,
                1
            ));

            current_x += key_w + spacing;
            key_ctx_count++;
        }

        // Row 3
        current_x = x + 30;
        start_y += key_h + spacing;

        for (int i = 0; i < 7; ++i) {
            key_contexts[key_ctx_count] = {this, row3[i]};
            add_child(new Button(
                current_x,
                start_y,
                key_w,
                key_h,
                String(row3[i]),
                handle_key_press,
                &key_contexts[key_ctx_count],
                constants::color_primary,
                constants::color_text,
                1
            ));
            current_x += key_w + spacing;
            key_ctx_count++;
        }

        // Bottom row (Cancel, Space, Backspace, Submit)
        start_y += key_h + spacing;
        current_x = x + 5;
        
        add_child(
            new Button(
                current_x,
                start_y,
                60,
                key_h,
                "Cancel",
                handle_cancel,
                this,
                constants::color_primary,
                constants::color_text,
                1
            )
        );

        current_x += 60 + spacing;
        
        key_contexts[key_ctx_count] = {this, ' '};

        add_child(
            new Button(
                current_x,
                start_y,
                100,
                key_h,
                "Space",
                handle_key_press,
                &key_contexts[key_ctx_count],
                constants::color_primary,
                constants::color_text,
                1
            )
        );

        key_ctx_count++;

        current_x += 100 + spacing;
        
        add_child(
            new Button(
                current_x,
                start_y,
                40,
                key_h,
                "Del",
                handle_backspace,
                this,
                constants::color_primary,
                constants::color_text,
                1
            )
        );

        current_x += 40 + spacing;
        
        add_child(
            new Button(
                current_x,
                start_y,
                80,
                key_h,
                "Submit",
                handle_submit,
                this,
                constants::color_primary,
                constants::color_text,
                1
            )
        );
    }

    void show(String current_text) {
        input_text = current_text;
        display_view->set_text(input_text);
        set_visible(true);
    }

    void draw(Adafruit_ILI9341& screen) override {
        if (!is_visible) return;
        screen.fillRect(x, y, width, height, constants::color_keyboard_bg);
        screen.drawRect(x, y, width, height, constants::color_border);
        View::draw(screen);
    }
};

} // namespace ui
