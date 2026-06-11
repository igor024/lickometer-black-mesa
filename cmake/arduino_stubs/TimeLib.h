#pragma once

#include <ctime>

using std::time_t;

inline int month(time_t) {
    return 1;
}

inline int day(time_t) {
    return 1;
}

inline int year(time_t) {
    return 1970;
}

inline int hour(time_t) {
    return 0;
}

inline int minute(time_t) {
    return 0;
}

inline int second(time_t) {
    return 0;
}
