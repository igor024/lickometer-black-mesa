#pragma once

#define DEBUG_MODE 1

#if DEBUG_MODE
    #define WARN(msg) { Serial.print(F("WARN: ")); Serial.println((msg)); }
#else 
    #define WARN(msg)
#endif
