#include <Arduino.h>

void setup();
void loop();

TwoWire Wire;
SerialStub Serial;

void delay(uint32_t) {}

int main() {
    setup();
    loop();
    return 0;
}
