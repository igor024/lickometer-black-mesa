#pragma once

#include "view.hpp"
#include "../../../constants.hpp"
#include <Arduino.h>

namespace ui {

enum class TextAlign {
    Left,
    Center
};

class TextView : public View {
private:
    String text;
    uint16_t color;
    uint8_t text_size;
    TextAlign align;

public:
    TextView(
        int16_t x,
        int16_t y,
        int16_t w,
        int16_t h,
        String text,
        uint16_t color = constants::color_text,
        uint8_t text_size = 1,
        TextAlign align = TextAlign::Left
    ): 
        View(x, y, w, h),
        text(text),
        color(color),
        text_size(text_size),
        align(align)
    {}

    void set_text(String new_text) {
        text = new_text;
    }

    void set_align(TextAlign new_align) {
        align = new_align;
    }

    void draw(Adafruit_ILI9341& screen) override {
        if (!is_visible) return;
        // Simple background clear (optional, might need better redraw logic depending on update frequency)
        screen.fillRect(x, y, width, height, constants::color_bg); 

        int16_t text_x = x;
        int16_t text_y = y;

        if (align == TextAlign::Center) {
            int16_t text_width = text.length() * 6 * text_size;
            int16_t text_height = 8 * text_size;
            int16_t offset_x = (width - text_width) / 2;
            int16_t offset_y = (height - text_height) / 2;
            text_x = x + (offset_x > 0 ? offset_x : 0);
            text_y = y + (offset_y > 0 ? offset_y : 0);
        }
        
        screen.setCursor(text_x, text_y);
        screen.setTextColor(color);
        screen.setTextSize(text_size);
        screen.print(text);
        
        View::draw(screen); // Draw children if any
    }
};

} // namespace ui
