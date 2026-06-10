#pragma once

#include "view.hpp"
#include "../../../constants.hpp"
#include <Arduino.h>

namespace ui {

class TextView : public View {
private:
    String text;
    uint16_t color;
    uint8_t text_size;

public:
    TextView(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        String text,
        uint16_t color = constants::color_text,
        uint8_t text_size = 1
    ): 
        View(x, y, w, h),
        text(text),
        color(color),
        text_size(text_size)
    {}

    void set_text(String new_text) {
        text = new_text;
    }

    void draw(Adafruit_ILI9341& screen) override {
        if (!is_visible) return;
        // Simple background clear (optional, might need better redraw logic depending on update frequency)
        screen.fillRect(x, y, width, height, constants::color_bg); 
        
        screen.setCursor(x, y);
        screen.setTextColor(color);
        screen.setTextSize(text_size);
        screen.print(text);
        
        View::draw(screen); // Draw children if any
    }
};

} // namespace ui
