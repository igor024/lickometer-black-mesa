#include "./cage_data.hpp"
#include <TimeLib.h>

void cage_data::record_contact_in() {
    current_lick.start_time = timelib_get();
}

void cage_data::record_contact_out() {
    current_lick.end_time = timelib_get();

    timelib_t dur = current_lick.end_time - current_lick.start_time;

}
