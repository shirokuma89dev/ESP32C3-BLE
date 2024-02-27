#include "led.h"

void Led::init() {
    led.begin();
    led.setBrightness(Brightness);
    led.fill(Blank);

    led.show();
}

bool Led::setBLE_Status(bool status) {
    if (status) {
        led.setPixelColor(0, Cyan);
    } else {
        led.setPixelColor(0, Red);
    }

    return status;
}

void Led::show(void) {
    led.show();
}