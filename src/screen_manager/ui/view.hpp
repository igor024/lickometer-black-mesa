#pragma once

#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <Arduino.h>

namespace ui {

constexpr uint8_t MAX_CHILDREN = 40;

class View {
    protected:
        int16_t x, y, width, height;
        View* children[MAX_CHILDREN];
        uint8_t child_count;
        bool is_visible;

    public:
        View(
            int16_t x,
            int16_t y,
            int16_t w,
            int16_t h
        ): 
            x(x),
            y(y),
            width(w),
            height(h),
            child_count(0),
            is_visible(true)
        {
            for (uint8_t i = 0; i < MAX_CHILDREN; ++i) {
                children[i] = nullptr;
            }
        } 

        virtual ~View() {
            for (uint8_t i = 0; i < child_count; i++) {
                delete children[i];
            }
        }

        void set_visible(bool visible) {
            is_visible = visible;
        }

        bool get_visible() const {
            return is_visible;
        }

        void add_child(View* child) {
            if (child_count < MAX_CHILDREN) {
                children[child_count++] = child;
            }
        }

        virtual void draw(Adafruit_ILI9341& screen) {
            if (!is_visible) return;
            for (uint8_t i = 0; i < child_count; i++) {
                if (children[i]) {
                    children[i]->draw(screen);
                }
            }
        }

        // Returns true if the click was handled by this view or its children
        virtual bool handle_click(int16_t click_x, int16_t click_y) {
            if (!is_visible) return false;
            if (contains(click_x, click_y)) {
                // Check children recursively (reverse order so top-most handles it first)
                for (int8_t i = child_count - 1; i >= 0; --i) {
                    if (children[i] && children[i]->handle_click(click_x, click_y)) {
                        return true;
                    }
                }
                return on_click(click_x, click_y);
            }
            return false;
        }

        bool contains(int16_t px, int16_t py) const {
            return (px >= x && px <= x + width && py >= y && py <= y + height);
        }

    protected:
        // Override this in subclasses to handle clicks specifically for that view
        virtual bool on_click(int16_t click_x, int16_t click_y) {
            return false; // By default, base View doesn't handle clicks itself, just passes to children
        }
};

} // namespace ui
