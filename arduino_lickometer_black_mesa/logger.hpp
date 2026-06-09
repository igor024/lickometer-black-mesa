#pragma once

#include <Arduino.h>

#define DEBUG_MODE 1

#if DEBUG_MODE
    #define WARN(msg) do { Serial.print(F("WARN: ")); Serial.println((msg)); } while (false)
#else 
    #define WARN(msg)
#endif
