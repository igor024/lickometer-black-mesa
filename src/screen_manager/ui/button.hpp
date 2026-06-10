#pragma once

#include "view.hpp"
#include "../../../constants.hpp"
#include <Arduino.h>

namespace ui {

class Button : public View {
private:
    String label;
    uint16_t bg_color;
    uint16_t text_color;
    void (*on_click_callback)(void*);
    void* callback_context;

public:
    Button(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        String label,
        void (*callback)(void*),
        void* context,
        uint16_t bg_color = constants::color_primary,
        uint16_t text_color = constants::color_text
    ): 
        View(x, y, w, h),
        label(label),
        bg_color(bg_color),
        text_color(text_color),
        on_click_callback(callback),
        callback_context(context)
    {}

    void draw(Adafruit_ILI9341& screen) override {
        if (!is_visible) return;
        screen.fillRoundRect(x, y, width, height, 4, bg_color);
        screen.drawRoundRect(x, y, width, height, 4, constants::color_border); 
        
        // Simple text centering approximation
        int16_t text_x = x + (width - (label.length() * 6)) / 2; // Assuming text size 1 (6x8 pixels)
        int16_t text_y = y + (height - 8) / 2;

        screen.setCursor(text_x, text_y);
        screen.setTextColor(text_color);
        screen.setTextSize(1);
        screen.print(label);

        View::draw(screen); // Draw children if any
    }

protected:
    bool on_click(int16_t click_x, int16_t click_y) override {
        if (on_click_callback) {
            on_click_callback(callback_context);
            return true;
        }
        return false;
    }
};

} // namespace ui
