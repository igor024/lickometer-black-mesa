Arduino sketch generated from:
  /home/oleksandr/Documents/lickometer-black-mesa

Open this file in Arduino IDE:
  /home/oleksandr/Documents/lickometer-black-mesa/arduino_lickometer_black_mesa/arduino_lickometer_black_mesa.ino

Project source files were copied into:
  /home/oleksandr/Documents/lickometer-black-mesa/arduino_lickometer_black_mesa

Required libraries:
  - MPR121
  - TimeLib
  - SdFat, unless the code is changed to use Arduino's built-in SD library only

Known code fixes still needed before upload:
  - Remove the extra global setup() in sd_writer/sd_writer.cpp.
  - Make sd_writer.hpp and sd_writer.cpp function signatures match.
  - Use either SD.h or SdFat.h consistently.
  - Add Arduino.h includes where files directly use Arduino symbols like Serial, F(), or min().
