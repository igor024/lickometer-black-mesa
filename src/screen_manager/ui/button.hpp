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
    uint8_t text_size;

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
        uint16_t text_color = constants::color_text,
        uint8_t text_size = 2
    ): 
        View(x, y, w, h),
        label(label),
        bg_color(bg_color),
        text_color(text_color),
        on_click_callback(callback),
        callback_context(context),
        text_size(text_size)
    {}

    void draw(Adafruit_ILI9341& screen) override {
        if (!is_visible) return;
        screen.fillRoundRect(x, y, width, height, 4, bg_color);
        screen.drawRoundRect(x, y, width, height, 4, constants::color_border); 
        
        int16_t offset_x = (width - (label.length() * 6 * text_size)) / 2;
        int16_t offset_y = (height - 8 * text_size) / 2; 

        int16_t text_x = x + offset_x;
        int16_t text_y = y + offset_y;

        screen.setCursor(text_x, text_y);
        screen.setTextColor(text_color);
        screen.setTextSize(text_size);
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
